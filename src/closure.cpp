#include "closure.h"

#include <stdexcept>

bdd
closure(const bdd& init_states, const std::vector<bdd_transition>& transitions)
{
  // NOTE: Both `init_states` and `transitions` are marked *const*. That is,
  //       they are immutable and read-only. If you want to manipulate them,
  //       then first assign them into a local variable:
  /*
  bdd x = init_states;
  */

  // NOTE: You can use a for-each loop to go through all transitions:
  /*
  for (const bdd_transition& t : transitions) {
    ...
  }
  */
  // Alternatively, you can also use the `.at(int)` function to get any
  // particular transition you need.

  std::cerr << "'closure()' has not yet been implemented!\n";
  return bdd_bot();
}
