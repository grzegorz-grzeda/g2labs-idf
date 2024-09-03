/**
 * Copyright (C) G2Labs Grzegorz Grzęda - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Grzegorz Grzęda <grzegorz.grzeda@g2labs.pl>, 03.2023
 */
#ifndef FONT_H
#define FONT_H
#include <stdint.h>

typedef struct font {
    uint8_t width;
    uint8_t height;
    union {
        const void* table;
        const uint8_t* u8_table;
        const uint16_t* u16_table;
        const uint32_t* u32_table;
    };
} font_t;

const font_t* font_get(const char* name);

const font_t* font_default(void);

uint8_t font_get_pixel(const font_t* font, uint16_t x, uint16_t y, char c);

#endif  // FONT_H