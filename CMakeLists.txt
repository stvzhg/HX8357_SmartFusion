cmake_minimum_required(VERSION 3.6)
project(HX8537)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES hx8357.c hx8357.h)
add_library(HX8537 ${SOURCE_FILES})