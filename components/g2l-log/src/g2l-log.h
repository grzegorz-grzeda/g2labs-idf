/*
 * MIT License
 *
 * Copyright (c) 2024 Grzegorz Grzęda
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef G2L_IDF_LOG_H
#define G2L_IDF_LOG_H

#include <stdarg.h>

#define LOG_COLOR_BLACK "30"
#define LOG_COLOR_RED "31"
#define LOG_COLOR_GREEN "32"
#define LOG_COLOR_BROWN "33"
#define LOG_COLOR_BLUE "34"
#define LOG_COLOR_PURPLE "35"
#define LOG_COLOR_CYAN "36"
#define LOG_COLOR(COLOR) "\033[0;" COLOR "m"
#define LOG_BOLD(COLOR) "\033[1;" COLOR "m"
#define LOG_RESET_COLOR "\033[0m"
#define LOG_COLOR_E LOG_COLOR(LOG_COLOR_RED)
#define LOG_COLOR_W LOG_COLOR(LOG_COLOR_BROWN)
#define LOG_COLOR_I LOG_COLOR(LOG_COLOR_GREEN)
#define LOG_COLOR_D LOG_COLOR(LOG_COLOR_GREEN)

#define LOG_PREFIX_E LOG_COLOR_E "[E]"
#define LOG_PREFIX_W LOG_COLOR_W "[W]"
#define LOG_PREFIX_I LOG_COLOR_I "[I]"
#define LOG_PREFIX_D LOG_COLOR_D "[D]"
#define LOG_SUFFIX LOG_RESET_COLOR "\n"

#define D(tag, x, args...) _log(LOG_PREFIX_D, tag, LOG_SUFFIX, x, ##args)

#define I(tag, x, args...) _log(LOG_PREFIX_I, tag, LOG_SUFFIX, x, ##args)

#define W(tag, x, args...) _log(LOG_PREFIX_W, tag, LOG_SUFFIX, x, ##args)

#define E(tag, x, args...) _log(LOG_PREFIX_E, tag, LOG_SUFFIX, x, ##args)

void _log(const char* prefix,
          const char* tag,
          const char* suffix,
          const char* message,
          ...);

#endif  // G2L_IDF_LOG_H