/**
 * MIT License
 * Copyright (c) 2023 Grzegorz Grzęda
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
#ifndef COLOR_MANIPULATION_H
#define COLOR_MANIPULATION_H

#include <stdint.h>

#define COLOR_MANIPULATION_HSV_HUE_MAX_VALUE (360)
#define COLOR_MANIPULATION_HSV_SATURATION_MAX_VALUE (100)
#define COLOR_MANIPULATION_HSV_VALUE_MAX_VALUE (100)


typedef struct color_rgb {
    uint8_t red;
    uint8_t blue;
    uint8_t green;
} color_rgb_t;

typedef struct color_hsv {
    uint16_t hue;
    uint8_t saturation;
    uint8_t value;
} color_hsv_t;

color_rgb_t convert_color_hsv_to_rgb(color_hsv_t hsv);

#endif // COLOR_MANIPULATION_H