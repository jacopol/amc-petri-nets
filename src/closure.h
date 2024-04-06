#ifndef CLOSURE_H
#define CLOSURE_H

#include <vector>

#include <bdd.h>

////////////////////////////////////////////////////////////////////////////////
/// \brief Computes the entire reachable set of states.
///
/// \param init_states The initial set of states.
///
/// \param transitions Vector (i.e. an array) of each individual transition.
////////////////////////////////////////////////////////////////////////////////
bdd
closure(const bdd& init_states, const std::vector<bdd_transition>& transitions);

#endif // CLOSURE_H
