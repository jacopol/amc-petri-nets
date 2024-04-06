#include <ctl.h>

// Store the vector of BDD transitions once and for all.
std::vector<bdd_transition> model_transitions;
std::vector<bdd> fairness;

void 
init_CTL(const std::vector<bdd_transition>& transitions) {
  model_transitions = transitions;
}

///////////////////////////////////////////////////////////
// Can be used to declare fairness constraints
// - this is useful for the optional assignment on fairness
///////////////////////////////////////////////////////////
void 
fairness_constraint(const bdd& constraint) {
  fairness.push_back(constraint);
}

// ----------------------------------------------------------
// CTL OPERATORS (to be implemented)
// ----------------------------------------------------------

bdd
ATOM(int place) {
  return bdd_var(place, false);
}

bdd
NOT(const bdd& phi) {
  return ~phi;
}

bdd
AND(const bdd& phi, const bdd& psi) {
  std::cerr  << "CTL formula: AND(phi,psi) has not been implemented yet\n";
  return bdd_bot();
}

bdd
OR(const bdd& phi, const bdd& psi) {
  std::cerr  << "CTL formula: OR(phi,psi) has not been implemented yet\n";
  return bdd_bot();
};

bdd
EX(const bdd& phi) {
  bdd prev = bdd_bot();
  for (const bdd_transition& t : model_transitions) {
      prev += bdd_relprev(phi,t);
  }
  return prev;
}

bdd
EF(const bdd& phi) {
  // NOTE: EF is a derived operator
  return EU(bdd_top(),phi);
}

bdd
EG(const bdd& phi) {
  std::cerr << "CTL formula: EG(phi) has not been implemented yet\n";
  return bdd_bot();
}

bdd
EU(const bdd& phi, const bdd& psi) {
  std::cerr  << "CTL formula: EU(phi,psi) has not been implemented yet\n";
  return bdd_bot();
}

bdd AX(const bdd& phi) { 
  std::cerr  << "A-CTL not implemented yet\n";
  return bdd_bot();
}
bdd AF(const bdd& phi) { 
  std::cerr  << "A-CTL not implemented yet\n";
  return bdd_bot();
}
bdd AG(const bdd& phi) { 
  std::cerr  << "A-CTL not implemented yet\n";
  return bdd_bot(); 
}
bdd AU(const bdd& phi, const bdd& psi) { 
  std::cerr  << "A-CTL not implemented yet\n";
  return bdd_bot(); 
}
