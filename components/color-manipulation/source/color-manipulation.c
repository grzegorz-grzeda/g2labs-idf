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
#include "color-manipulation.h"

#define COLOR_MANIPULATION_RGB_MAX_VALUE_FLOAT (2.55f)
#define COLOR_MANIPULATION_HSV_SATURATION_MAX_VALUE_FLOAT (100.0f)
#define COLOR_MANIPULATION_HSV_HUE_GROUP_SIZE (60)

static color_hsv_t clip_hsv_input_values(color_hsv_t hsv) {
    hsv.hue %= COLOR_MANIPULATION_HSV_HUE_MAX_VALUE;  // h -> [0,360)
    if (hsv.saturation > COLOR_MANIPULATION_HSV_SATURATION_MAX_VALUE) {
        hsv.saturation = COLOR_MANIPULATION_HSV_SATURATION_MAX_VALUE;
    }
    if (hsv.value > COLOR_MANIPULATION_HSV_VALUE_MAX_VALUE) {
        hsv.value = COLOR_MANIPULATION_HSV_VALUE_MAX_VALUE;
    }
    return hsv;
}

color_hsv_t convert_color_rgb_to_hsv(color_rgb_t rgb) {
    color_hsv_t hsv;
    uint8_t rgb_max = rgb.red;
    uint8_t rgb_min = rgb.red;
    if (rgb.green > rgb_max) {
        rgb_max = rgb.green;
    } else if (rgb.green < rgb_min) {
        rgb_min = rgb.green;
    }
    if (rgb.blue > rgb_max) {
        rgb_max = rgb.blue;
    } else if (rgb.blue < rgb_min) {
        rgb_min = rgb.blue;
    }

    uint8_t rgb_delta = rgb_max - rgb_min;
    hsv.value = rgb_max;
    if (rgb_max == 0) {
        hsv.saturation = 0;
        hsv.hue = 0;
        return hsv;
    }
    hsv.saturation =
        (rgb_delta * COLOR_MANIPULATION_HSV_SATURATION_MAX_VALUE) / rgb_max;
    if (hsv.saturation == 0) {
        hsv.hue = 0;
        return hsv;
    }
    if (rgb_max == rgb.red) {
        hsv.hue = 60 * (rgb.green - rgb.blue) / rgb_delta;
    } else if (rgb_max == rgb.green) {
        hsv.hue = 60 * (rgb.blue - rgb.red) / rgb_delta + 120;
    } else {
        hsv.hue = 60 * (rgb.red - rgb.green) / rgb_delta + 240;
    }
    if (hsv.hue < 0) {
        hsv.hue += COLOR_MANIPULATION_HSV_HUE_MAX_VALUE;
    }
    return hsv;
}

color_rgb_t convert_color_hsv_to_rgb(color_hsv_t hsv) {
    hsv = clip_hsv_input_values(hsv);
    uint32_t rgb_max = hsv.value * COLOR_MANIPULATION_RGB_MAX_VALUE_FLOAT;
    uint32_t rgb_min =
        rgb_max *
        (COLOR_MANIPULATION_HSV_SATURATION_MAX_VALUE - hsv.saturation) /
        COLOR_MANIPULATION_HSV_SATURATION_MAX_VALUE_FLOAT;
    uint32_t hue_color_group = hsv.hue / COLOR_MANIPULATION_HSV_HUE_GROUP_SIZE;
    uint32_t hue_color_group_difference =
        hsv.hue % COLOR_MANIPULATION_HSV_HUE_GROUP_SIZE;

    // RGB adjustment amount by hue
    uint32_t rgb_adjustment = (rgb_max - rgb_min) * hue_color_group_difference /
                              COLOR_MANIPULATION_HSV_HUE_GROUP_SIZE;

    color_rgb_t result;
    switch (hue_color_group) {
        case 0:
            result.red = rgb_max;
            result.green = rgb_min + rgb_adjustment;
            result.blue = rgb_min;
            break;
        case 1:
            result.red = rgb_max - rgb_adjustment;
            result.green = rgb_max;
            result.blue = rgb_min;
            break;
        case 2:
            result.red = rgb_min;
            result.green = rgb_max;
            result.blue = rgb_min + rgb_adjustment;
            break;
        case 3:
            result.red = rgb_min;
            result.green = rgb_max - rgb_adjustment;
            result.blue = rgb_max;
            break;
        case 4:
            result.red = rgb_min + rgb_adjustment;
            result.green = rgb_min;
            result.blue = rgb_max;
            break;
        default:
            result.red = rgb_max;
            result.green = rgb_min;
            result.blue = rgb_max - rgb_adjustment;
            break;
    }
    return result;
}
