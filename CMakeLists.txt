cmake_minimum_required(VERSION 3.17)
project(Fluxion)

set(CMAKE_CXX_STANDARD 14)

add_library(Fluxion SHARED library.cpp library.h internals/Expression.cpp internals/Expression.h internals/util.cpp internals/util.h)