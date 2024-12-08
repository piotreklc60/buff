# ---------------------------------------- LICENSE SECTION -------------------------------------------------------------------
#
# Copyright (c) 2024 Piotr Wojtowicz
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
# -----------------------------------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.22)

add_library(buff_lib INTERFACE)

# -----------------------------------------------------------------------------------------------------------------------------
# ------------------------------------- files paths ---------------------------------------------------------------------------
# -----------------------------------------------------------------------------------------------------------------------------
# files paths
set(BUFF_PATH ${CMAKE_CURRENT_LIST_DIR})

# -----------------------------------------------------------------------------------------------------------------------------
# ------------------------------------- BUFF CORE source files ----------------------------------------------------------------
# -----------------------------------------------------------------------------------------------------------------------------

target_include_directories(usbd_lib INTERFACE
    ${BUFF_PATH}/api/
)

target_sources(usbd_lib INTERFACE
    ${BUFF_PATH}/imp/src/buff.c
)