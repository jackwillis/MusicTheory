# CMakeList.txt : CMake project for CMakeProject1, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set (CMAKE_CXX_STANDARD 20)

# Boost settings see https://cliutils.gitlab.io/modern-cmake/chapters/packages/Boost.html
#
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.78 REQUIRED)
message(STATUS "Boost version: ${Boost_VERSION}")

# Main executable
#
add_executable (CMakeProject1 "CMakeProject1.cpp" "CMakeProject1.h" "Tuning.h" "Tuning.cpp")

# Link libraries
#
target_link_libraries(CMakeProject1 PRIVATE Boost::boost)
