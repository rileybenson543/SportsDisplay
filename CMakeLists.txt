# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("SportsDisplay")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Include sub-projects.
add_subdirectory ("SportsDisplay")
