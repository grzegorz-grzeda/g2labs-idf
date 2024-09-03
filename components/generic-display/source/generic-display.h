/*
 * MIT License
 *
 * Copyright (c) 2023 Grzegorz Grzęda
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef GENERIC_DISPLAY_H
#define GENERIC_DISPLAY_H

#include <stdint.h>

typedef enum generic_display_color {
    GENERIC_DISPLAY_COLOR_1B,
    GENERIC_DISPLAY_COLOR_MONO_8B,
    GENERIC_DISPLAY_COLOR_MONO_16B,
    GENERIC_DISPLAY_COLOR_MONO_32B,
    GENERIC_DISPLAY_COLOR_R2G3B2_8B,
    GENERIC_DISPLAY_COLOR_R5G6B5_16B,
    GENERIC_DISPLAY_COLOR_R8G8B8_24B,
    GENERIC_DISPLAY_COLOR_R8G8B8T8_32B,
} generic_display_color_t;

typedef struct generic_display_info {
    uint32_t width;
    uint32_t height;
    generic_display_color_t color_type;
} generic_display_info_t;

typedef struct generic_display generic_display_t;

typedef void (*generic_display_handler_t)(generic_display_t* display);
typedef void (*generic_display_fill_handler_t)(generic_display_t* display, uint32_t color);
typedef void (*generic_display_pixel_handler_t)(generic_display_t* display, uint32_t x, uint32_t y, uint32_t color);
typedef void (*generic_display_line_handler_t)(generic_display_t* display,
                                               uint32_t x0,
                                               uint32_t y0,
                                               uint32_t x1,
                                               uint32_t y1,
                                               uint32_t color);

typedef struct generic_display {
    void* context;
    generic_display_info_t info;
    generic_display_handler_t reset;
    generic_display_handler_t update;
    generic_display_handler_t destroy;
    generic_display_fill_handler_t fill;
    generic_display_pixel_handler_t pixel;
    generic_display_line_handler_t line;
} generic_display_t;

#endif  // GENERIC_DISPLAY_H