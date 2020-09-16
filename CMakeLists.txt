cmake_minimum_required(VERSION 3.17)
project(Fluxion)

set(CMAKE_CXX_STANDARD 14)

add_library(Fluxion SHARED fluxion.cpp fluxion.h internals/Expression.cpp internals/Expression.h internals/util.cpp internals/util.h internals/Parser.cpp internals/Parser.h internals/debug.cpp internals/debug.h internals/Compiler.cpp internals/Compiler.h)
add_executable(FluxionREPL FluxionRepl.cpp)
target_link_libraries(FluxionREPL Fluxion)