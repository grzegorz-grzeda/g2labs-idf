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
get_property(__g2l_idf_env_set GLOBAL PROPERTY __G2L_IDF_ENV_SET)

if(NOT __g2l_idf_env_set)
    add_library(__g2l_idf_build_target STATIC IMPORTED GLOBAL)

    include(${CMAKE_CURRENT_LIST_DIR}/utilities.cmake)
    include(${CMAKE_CURRENT_LIST_DIR}/test.cmake)

    get_property(G2L_IDF_TARGET_PLATFORM_VALUE CACHE G2L_IDF_TARGET_PLATFORM PROPERTY VALUE)
    g2l_idf_build_set_property(G2L_IDF_TARGET_PLATFORM ${G2L_IDF_TARGET_PLATFORM_VALUE})

    get_property(G2L_IDF_TARGET_PLATFORM_VARIANT_VALUE CACHE G2L_IDF_TARGET_PLATFORM_VARIANT PROPERTY VALUE)
    g2l_idf_build_set_property(G2L_IDF_TARGET_PLATFORM_VARIANT ${G2L_IDF_TARGET_PLATFORM_VARIANT_VALUE})

    set_property(GLOBAL PROPERTY __G2L_IDF_ENV_SET 1)
endif()