cmake_minimum_required(VERSION 3.24)
project(mamba)

set(CMAKE_CXX_STANDARD 14)

include_directories(Source)
include_directories(Source/Compiler)
include_directories(Source/Interpreter)
include_directories(Source/Standard)
include_directories(Source/Targets)

add_executable(mamba
        Source/Compiler/Compiler.hh
        Source/Compiler/Errors.cc
        Source/Compiler/Inclusion.hpp
        Source/Compiler/Lexer.cc
        Source/Compiler/Lexer.hh
        Source/Compiler/Linkage.cc
        Source/Compiler/Medium.cc
        Source/Compiler/Parser.cc
        Source/Compiler/Parser.hh
        Source/Compiler/Warnings.cc
        Source/Interpreter/Base.cc
        Source/Interpreter/Base.hh
        Source/Interpreter/Executor.cc
        Source/Interpreter/Executor.hh
        Source/Interpreter/Interpreter.hh
        Source/Interpreter/Region.cc
        Source/Interpreter/Region.hh
        Source/Standard/Functions.cpp
        Source/Standard/Functions.hh
        Source/Standard/math.cc
        Source/Standard/math.hh
        Source/Standard/os.hh
        Source/Standard/random.hh
        Source/Standard/re.hh
        Source/Standard/Standard.hh
        Source/Standard/string.hh
        Source/Standard/textwrap.hh
        Source/Standard/Types.cc
        Source/Targets/Linux.cc
        Source/Targets/Linux.hh
        Source/Targets/Windows.cc
        Source/Targets/Windows.hh
        Source/CLI.cc
        Source/Commons.cc
        Source/Exceptions.cc
        Source/Exceptions.hh
        Source/Functions.cc
        Source/Mamba.cc
        Source/Standard.cc
        Tests/Compiler/Lexer.cc)
