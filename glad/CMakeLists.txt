cmake_minimum_required(VERSION 3.26)
project("glad")

set(CMAKE_CXX_STANDARD "20")
set(CMAKE_C_STANDARD "11")

add_library("glad" STATIC "src/glad.c" "include/glad/glad.h")
target_include_directories("glad" PUBLIC "include")