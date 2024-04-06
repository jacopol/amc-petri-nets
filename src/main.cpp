#include <filesystem>
#include <getopt.h>
#include <iomanip>
#include <iostream>
#include <string>

#include <bdd.h>
#include <closure.h>
#include <ctl.h>
#include <ctl_test.h>
#include <pnml.h>

std::filesystem::path input_file;
long long memory_bytes = -1;

////////////////////////////////////////////////////////////////////////////////
/// \brief Parse input arguments.
////////////////////////////////////////////////////////////////////////////////
void
parse_args(int argc, char* argv[])
{
  bool error = false;
  int c;

  while ((c = getopt(argc, argv, "i:M:h")) != -1) {
    switch (c) {
    case '?':
    case 'h': {
      std::cerr << "Algorithmic Model Checking\n"
                << "  Petri Net Symbolic Model Checker\n"
                << "\n"
                << "  -h\n"
                << "     Prints this message and returns\n"
                << "\n"
                << "  -i <path>\n"
                << "     Path to input '.pnml' file\n"
                << "\n"
                << "  -M <int>\n"
                << "     Number of bytes for the BDD package\n"
                << std::endl;
      exit(0);
    }
    case 'i': {
      input_file = optarg;
      break;
    }
    case 'M': {
      memory_bytes = std::stoll(optarg) * 1024 * 1024;
      break;
    }
    case -1: {
      break;
    }
    }
  }

  if (input_file.empty()) {
    std::cerr << "input file is missing (parameter: '-i')\n";
    error = true;
  }
  if (memory_bytes < 0) {
    std::cerr << "memory is missing (parameter: '-M')\n";
    error = true;
  }
  if (error) { exit(-1); }
}

////////////////////////////////////////////////////////////////////////////////
/// \brief Main function, reading the model and running the relevant operations.
////////////////////////////////////////////////////////////////////////////////
int
main(int argc, char* argv[])
{
  parse_args(argc, argv);
  pnml input_pnml(input_file);
  std::cerr << "Read Petri Net: " 
            << input_pnml.places() 
            << " places," << input_pnml.transitions() << " transitions.\n";

  bdd_init(input_pnml.places(), memory_bytes);
  {
    // Convert initial marking in PNML to BDD
    const bdd init_states = bdd_convert(input_pnml.initial());
    std::cerr << "Initial states:   " << bdd_satcount(init_states) << "\n";

    // uncomment next block for debugging. Shows initial state BDD in DOT format
    // Note: state variables (places) are encoded using "even" variables
/*
    bdd_printdot(init_states);
*/

    // Convert each transition in PNML to a BDD Relation
    std::vector<bdd_transition> transitions;
    transitions.reserve(input_pnml.transitions());

    for (int t = 0; t < input_pnml.transitions(); ++t) {
      transitions.push_back(bdd_convert(input_pnml.transition(t)));
    }

    // uncomment next block for debugging. Shows transition BDDs in DOT format
    // Note: transitions are encoded using "even" variables (pre) and "odd" variables (post)
/*
    for (bdd_transition t : transitions) {
      bdd_printdot(t);
    }
*/
    
    // Compute reachable states
    std::cerr << "\nReachability result:\n---------------------\n";
    bdd reachable_states = closure(init_states, transitions);
    std::cerr << std::setprecision(std::numeric_limits<double>::digits10) 
              << "Reachable states: " << bdd_satcount(reachable_states) << "\n\n";

    // Compute CTL formulas
    init_CTL(transitions);
    std::vector<bdd> ctl_results = ctl_test_vector();
    std::cerr << "\nCTL results:\n------------\n";
    for (bdd ctl_result : ctl_results) {
      if (init_states <= ctl_result)
         std::cerr << "formula HOLDS in all initial states\n";
      else
         std::cerr << "formula DOES NOT HOLD in all initial states\n";
    }
  }
  bdd_deinit();

  return 0;
}
