cmake_minimum_required (VERSION 2.6)
project (sdlplot_test)
# The version number.
set (Tutorial_VERSION_MAJOR 1)
set (Tutorial_VERSION_MINOR 0)
# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/TutorialConfig.h.in"
  "${PROJECT_BINARY_DIR}/TutorialConfig.h"
  )
 

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")
 
# add the executable
add_executable(sdlplot_test tutorial.cxx sdlplot.cxx debug_print.cxx)


find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(Boost COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

target_link_libraries (sdlplot_test fftw3 ${SDL2_LIBRARIES} ${Boost_LIBRARIES})


