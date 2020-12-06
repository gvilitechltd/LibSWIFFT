cmake_minimum_required(VERSION 3.16.3)
project(swifft LANGUAGES C CXX VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(
  "-fPIC"
  "-Wall" "-Wpedantic" "-Wextra" "-fexceptions"
  "$<$<CONFIG:RELEASE>:-Ofast>"
  "$<$<CONFIG:DEBUG>:-O0;-g3;-ggdb;--coverage>"
)
add_link_options(
  "$<$<CONFIG:DEBUG>:--coverage>"
)

add_subdirectory(src)
add_subdirectory(test)
