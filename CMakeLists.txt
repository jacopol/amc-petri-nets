cmake_minimum_required(VERSION 3.21)

project (AMC_PetriNet
  VERSION 0.1.0
  DESCRIPTION "Algorithmic Model Checking: Petri Net Model Checker"
  LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory (external/pugixml)
add_subdirectory (external/sylvan)

add_subdirectory (src)
