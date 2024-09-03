/**
 * Copyright (C) G2Labs Grzegorz Grzęda - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Grzegorz Grzęda <grzegorz.grzeda@g2labs.pl>, 03.2023
 */
#include "font.h"
#include <stddef.h>
#include <string.h>
#include "fonts-used.h"

#define FONT_NAME_MATCHES(input, name) (strcmp((input), (name)) == 0)

extern const font_t _font_5x7_object;

static const font_t font_empty_object = {
    .height = 0,
    .width = 0,
    .table = NULL,
};

const font_t* font_get(const char* name) {
    if (FONT_NAME_MATCHES(name, "3x5")) {
        extern const font_t _font_3x5_object;
        return &_font_3x5_object;
    }
    if (FONT_NAME_MATCHES(name, "4x6")) {
        extern const font_t _font_4x6_object;
        return &_font_4x6_object;
    }
    if (FONT_NAME_MATCHES(name, "5x7")) {
        return &_font_5x7_object;
    }
    if (FONT_NAME_MATCHES(name, "6x8")) {
        extern const font_t _font_6x8_object;
        return &_font_6x8_object;
    }
    if (FONT_NAME_MATCHES(name, "8x8")) {
        extern const font_t _font_8x8_object;
        return &_font_8x8_object;
    }
    if (FONT_NAME_MATCHES(name, "8x9")) {
        return &font_empty_object;
    }
    if (FONT_NAME_MATCHES(name, "8x12")) {
        return &font_empty_object;
    }
    if (FONT_NAME_MATCHES(name, "8x16")) {
        return &font_empty_object;
    }
    if (FONT_NAME_MATCHES(name, "12x16")) {
        return &font_empty_object;
    }
    if (FONT_NAME_MATCHES(name, "14x24")) {
        return &font_empty_object;
    }
    if (FONT_NAME_MATCHES(name, "16x16")) {
        return &font_empty_object;
    }
    if (FONT_NAME_MATCHES(name, "16x22")) {
        return &font_empty_object;
    }
    if (FONT_NAME_MATCHES(name, "16x24")) {
        return &font_empty_object;
    }
    if (FONT_NAME_MATCHES(name, "16x32")) {
        return &font_empty_object;
    }
    if (FONT_NAME_MATCHES(name, "24x29")) {
        return &font_empty_object;
    }
    if (FONT_NAME_MATCHES(name, "24x32")) {
        extern const font_t _font_24x32_object;
        return &_font_24x32_object;
    }
    if (FONT_NAME_MATCHES(name, "32x24")) {
        return &font_empty_object;
    }
    if (FONT_NAME_MATCHES(name, "32x32")) {
        return &font_empty_object;
    }
    return &font_empty_object;
}

const font_t* font_default(void) {
    return &_font_5x7_object;
}

uint8_t font_get_pixel(const font_t* font, uint16_t x, uint16_t y, char c) {
    if (!font || !font->table || (c < ' ') || (c > '~') || (x >= font->width) || (y >= font->height)) {
        return 0;
    }

    uint32_t value = 0;
    uint32_t position = ((uint32_t)(c - ' '));
    position *= font->width;
    position += (uint32_t)x;
    if (font->height < 9) {
        value = font->u8_table[position];
    } else if (font->height < 17) {
        value = font->u16_table[position];
    } else {
        value = font->u32_table[position];
    }

    return ((value & (1 << y)) ? 1 : 0);
}