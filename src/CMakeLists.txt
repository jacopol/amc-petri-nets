set_property(GLOBAL PROPERTY USE_FOLDERS On)

# ============================================================================ #
# Set up files for Petri Net "library"

set(HEADERS
  bdd.h
  pnml.h
  closure.h
  ctl.h
)

set(SOURCES
  bdd.cpp
  pnml.cpp
  closure.cpp
  ctl.cpp
)

add_library(petri STATIC ${HEADERS} ${SOURCES})

target_link_libraries(petri PUBLIC pugixml)
target_link_libraries(petri PUBLIC sylvan::sylvan)

target_include_directories(petri
  PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

# ============================================================================ #
# Set up file for execution
add_executable (main main.cpp)
target_link_libraries(main petri)
