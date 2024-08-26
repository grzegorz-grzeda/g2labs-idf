# MIT License
#
# Copyright (c) 2024 Grzegorz Grzęda
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
cmake_minimum_required(VERSION 3.25)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    message("Building G2Labs IDF framework itself")

    if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
        message(STATUS "CMAKE_TOOLCHAIN_FILE not defined, setting default testing toolchain")
        set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/cmake/toolchains/arduino-uno-toolchain.cmake" CACHE FILEPATH "The toolchain file" FORCE)
    endif()
endif()

project(g2labs-idf
    VERSION 0.0.1
    DESCRIPTION "G2 Labs IoT Development Framework"
    LANGUAGES C CXX ASM)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    include(${CMAKE_CURRENT_LIST_DIR}/cmake/g2l-idf.cmake)
    message(STATUS "Adding code examples")
    add_subdirectory(examples)
endif()

add_subdirectory(components)