cmake_minimum_required(VERSION 2.6)

project(liblcthw)

include_directories(${PROJECT_SOURCE_DIR}/src)

add_subdirectory(src/lcthw)
add_subdirectory(tests)

