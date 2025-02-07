# MIT License
#
# Copyright (c) 2024 G2Labs Grzegorz Grzeda
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
#
function(g2l_idf_build_get_property var property)
    get_property(val TARGET __g2l_idf_build_target PROPERTY ${property})

    set(${var} ${val} PARENT_SCOPE)
endfunction()

function(g2l_idf_build_set_property property value)
    set_property(TARGET __g2l_idf_build_target PROPERTY ${property} ${value})
endfunction()

function(g2l_idf_add_platform_subdirectory)
    if(NOT DEFINED G2L_IDF_TARGET_PLATFORM)
        message(FATAL_ERROR "Platform not set")
    else()
        if(NOT DEFINED G2L_IDF_TARGET_PLATFORM_VARIANT OR NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${G2L_IDF_TARGET_PLATFORM}${G2L_IDF_TARGET_PLATFORM_VARIANT})
            if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${G2L_IDF_TARGET_PLATFORM})
                message(FATAL_ERROR "Platform subdirectory not found in ${CMAKE_CURRENT_SOURCE_DIR}")
            else()
                add_subdirectory(${G2L_IDF_TARGET_PLATFORM})
            endif()
        else()
            add_subdirectory(${G2L_IDF_TARGET_PLATFORM}${G2L_IDF_TARGET_PLATFORM_VARIANT})
        endif()
    endif()
endfunction()