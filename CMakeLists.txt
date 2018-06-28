cmake_minimum_required(VERSION 3.10)
project(priority_queues)

# The version number.
set (priority_queues_VERSION_MAJOR 1)
set (priority_queues_VERSION_MINOR 0)

# variables used for in experiments,
# they define the number of trials to run in each experiment
set (SORT_TRIALS 8)
set (INSERT_MELD_TRIALS 8)
set (MELD_TRIALS 8)

set(CMAKE_CXX_STANDARD 14)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
        "${PROJECT_SOURCE_DIR}/config.h.in"
        "${PROJECT_BINARY_DIR}/config.h"
)

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")

# add the binary-heap-pq library?
option (USE_BINARY_HEAP_PQ "Use priority queue provided binary heap implementation" ON)
if (USE_BINARY_HEAP_PQ)
    include_directories ("${PROJECT_SOURCE_DIR}/binary-heap-pq")
    add_subdirectory (binary-heap-pq)
    set (EXTRA_LIBS ${EXTRA_LIBS} binary-heap-pq)
endif (USE_BINARY_HEAP_PQ)

# add the leftist-heap-pq library?
option (USE_LEFTIST_HEAP_PQ "Use priority queue provided leftist heap implementation" ON)
if (USE_LEFTIST_HEAP_PQ)
    include_directories ("${PROJECT_SOURCE_DIR}/leftist-heap-pq")
    add_subdirectory (leftist-heap-pq)
    set (EXTRA_LIBS ${EXTRA_LIBS} leftist-heap-pq)
endif (USE_LEFTIST_HEAP_PQ)

# add the skew-heap-pq library?
option (USE_SKEW_HEAP_PQ "Use priority queue provided skew heap implementation" ON)
if (USE_SKEW_HEAP_PQ)
    include_directories ("${PROJECT_SOURCE_DIR}/skew-heap-pq")
    add_subdirectory (skew-heap-pq)
    set (EXTRA_LIBS ${EXTRA_LIBS} skew-heap-pq)
endif (USE_SKEW_HEAP_PQ)

# add the binomial-pq library?
option (USE_BINOMIAL_PQ "Use priority queue provided binomial heap implementation" ON)
if (USE_BINOMIAL_PQ)
    include_directories ("${PROJECT_SOURCE_DIR}/binomial-pq")
    add_subdirectory (binomial-pq)
    set (EXTRA_LIBS ${EXTRA_LIBS} binomial-pq)
endif (USE_BINOMIAL_PQ)

# add the fibonacci-pq library?
option (USE_FIBONACCI_PQ "Use priority queue provided fibonacci heap implementation" ON)
if (USE_FIBONACCI_PQ)
    include_directories ("${PROJECT_SOURCE_DIR}/fibonacci-pq")
    add_subdirectory (fibonacci-pq)
    set (EXTRA_LIBS ${EXTRA_LIBS} fibonacci-pq)
endif (USE_FIBONACCI_PQ)

# add experiments library?
option(USE_EXPERIMENTS "Use experiments implementation" ON)
if (USE_EXPERIMENTS)
    include_directories ("${PROJECT_SOURCE_DIR}/experiments")
    add_subdirectory (experiments)
    set (EXTRA_LIBS ${EXTRA_LIBS} experiments)
endif (USE_EXPERIMENTS)

add_executable(priority_queues main.cpp priority-queue.h config.h.in)
target_link_libraries (priority_queues $(EXTRA_LIBS))

# add the install targets
install (TARGETS priority_queues DESTINATION bin)
install (FILES "${PROJECT_BINARY_DIR}/priority-queues-config.h" DESTINATION include)