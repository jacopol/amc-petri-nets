#include "bdd.h"

#include <sylvan.h>
#include <sylvan_table.h>
#include <sylvan_obj.hpp>

////////////////////////////////////////////////////////////////////////////////
// BDD Package                                                                //
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// \brief Number of BDD variables
////////////////////////////////////////////////////////////////////////////////
int bdd_varcount;

void
bdd_init(const int places, const int bytes)
{
  /// BDD package uses twice as many BDD variables as there are places, since
  /// each place is represented by a 'current' and 'next' state variable.
  bdd_varcount = places;

  // ---------------------------------------------------------------------------
  // Init LACE
  lace_start(0, 1000000);

  // ---------------------------------------------------------------------------
  // Init Sylvan

  /// 2^1 to 1 ratio between node table and cache
  const int cache_log = 1;

  /// Number of table doublings. The table will start this (power-of-two) times
  /// smaller than its final size (computed from `bytes`). Due to cache-misses,
  /// the table needs to not start out too big.
  ///
  /// As long as `bytes` is something between 1 and 16 GiB of memory, this value
  /// seems reasonable.
  const int table_doublings = 5;

  sylvan::sylvan_set_limits(bytes, cache_log, table_doublings);

  sylvan::sylvan_set_granularity(2);

  sylvan::sylvan_init_package();
  sylvan::sylvan_init_bdd();
}

void
bdd_deinit()
{
  // Deinit Sylvan
  sylvan::sylvan_quit();

  // Deinit LACE
  lace_stop();
}

////////////////////////////////////////////////////////////////////////////////
// BDD Constructors                                                           //
////////////////////////////////////////////////////////////////////////////////
bdd
bdd_bot()
{
  return sylvan::Bdd::bddZero();
}

bdd
bdd_top()
{
  return sylvan::Bdd::bddOne();
}

bdd
bdd_var(int p, bool is_prime)
{
  return sylvan::Bdd::bddVar(2*p + is_prime);
}

bdd
bdd_convert(const pnml::marking_type &marking)
{
  // Bottom-up, we add pre-state variables. If the variable is included in
  // `marking`, then the variable is added in its positive form. Otherwise, it
  // is added in its negative form.
  bdd vars = bdd_top();

  auto it = marking.rbegin();
  for (int p = bdd_varcount-1; 0 <= p; --p) {
    const bool is_marked = it != marking.rend() && *it == p;
    if (is_marked) { ++it; }

    vars &= is_marked ? bdd_var(p) : ~bdd_var(p);
  }
  return vars;
}

bdd_transition
bdd_convert(const pnml::transition_type &transition)
{
  // Bottom-up, we add pre- and post-state variables. To this end, we iterate
  // (in reverse) through the pre-markings in `transition.first` and the
  // post-markings in `transition.second`.
  auto pre_rbegin = transition.first.rbegin();
  auto pre_rend   = transition.first.rend();

  auto post_rbegin = transition.second.rbegin();
  auto post_rend   = transition.second.rend();

  bdd rel  = bdd_top();
  bdd vars = bdd_top();

  while (pre_rbegin != pre_rend || post_rbegin != post_rend) {
    // Obtain the next pre- and post- places that are marked
    const int pre_place  = pre_rbegin != pre_rend ? *pre_rbegin : -1;
    const int post_place = post_rbegin != post_rend ? *post_rbegin : -1;

    // Obtain the largest pre- and/or post- place that is marked. This is the
    // next BDD variable to be added to the transition relation.
    const int max_place = std::max(pre_place, post_place);
    assert(0 <= max_var);

    const bool pre_marked  = pre_place == max_place;
    const bool post_marked = post_place == max_place;
    assert(pre_marked || post_marked);

    // The pre-state variable
    // - Should be positive, if marked 'before'
    // - Otherwise, it can be whatever it wants to be
    bdd pre_bdd = pre_marked ? bdd_var(max_place, false) : bdd_top();

    // The post-state variable
    // - Should be positive, if marked 'after'
    // - Should be negative, if not marked 'after'
    bdd post_bdd = bdd_var(max_place, true);
    if (!post_marked) { post_bdd = ~post_bdd; }

    // Add `pre_bdd` and `post_bdd` to `rel`. Since we are doing it bottom-up,
    // this operation takes O(1) time.
    rel  &= pre_bdd & post_bdd;

    // Furthermore, add the BDD variable for `max_place` in O(1) time.
    vars &= bdd_var(max_place);

    // Go forward in iterators
    if (pre_place == max_place) { ++pre_rbegin; }
    if (post_place == max_place) { ++post_rbegin; }
  }

  return { rel, vars };
}

////////////////////////////////////////////////////////////////////////////////
// BDD Operations                                                             //
////////////////////////////////////////////////////////////////////////////////
bdd
bdd_not(const bdd& f)
{
  return ~f;
}

bdd
bdd_and(const bdd& f, const bdd& g)
{
  return f & g;
}

bdd
bdd_or(const bdd& f, const bdd& g)
{
  return f | g;
}

bdd
bdd_diff(const bdd& f, const bdd& g)
{
  return f - g;
}

bool
bdd_subset(const bdd& f, const bdd& g)
{
  return f <= g;
}

bdd
bdd_relnext(const bdd& s, const bdd_transition& t)
{
  return s.RelNext(t.relation, sylvan::BddSet(t.var_cube));
}

bdd
bdd_relprev(const bdd& s, const bdd_transition& t)
{
  return s.RelPrev(t.relation, sylvan::BddSet(t.var_cube));
}

double
bdd_satcount(const bdd& f)
{
  return f.SatCount(bdd_varcount);
}

////////////////////////////////////////////////////////////////////////////////
// BDD Debugging                                                              //
////////////////////////////////////////////////////////////////////////////////
void
bdd_printdot(const bdd& f, FILE* out)
{
  f.PrintDot(out);
}

void
bdd_printdot(const bdd& f, const std::filesystem::path& path)
{
  const std::string path_str = path.native();

  FILE *ptr_file = fopen(path.c_str(), "w");

  if (ptr_file != nullptr) {
    bdd_printdot(f, ptr_file);
  }
}

void
bdd_printdot(const bdd_transition& f, FILE* out)
{
  bdd_printdot(f.relation, out);
}

void
bdd_printdot(const bdd_transition& f, const std::filesystem::path& path)
{
  bdd_printdot(f.relation, path);
}
