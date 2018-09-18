cmake_minimum_required(VERSION 3.12)
project(mimir)

set(CMAKE_CXX_STANDARD 17)

include_directories(mimir)

add_subdirectory(mimir)
link_directories(mimir)
add_subdirectory(mimir-test)