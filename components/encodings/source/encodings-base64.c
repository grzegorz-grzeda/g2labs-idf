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
#include "encodings-base64.h"
#include <stdbool.h>
#include <string.h>

static bool is_padding_character(char letter) {
    return (letter == '=');
}

size_t encodings_base64_get_decode_buffer_size(const char* base64) {
    size_t encoded_size = strlen(base64);
    size_t decoded_size = encoded_size / 4;
    decoded_size *= 3;
    if (encoded_size > 2) {
        if (is_padding_character(base64[encoded_size - 1])) {
            decoded_size--;
        }
        if (is_padding_character(base64[encoded_size - 2])) {
            decoded_size--;
        }
    }
    return decoded_size;
}

static char base64_character_to_byte(char letter) {
    if ((letter >= 'A') && (letter <= 'Z')) {
        return (letter - 'A');
    } else if ((letter >= 'a') && (letter <= 'z')) {
        return (letter - 'a' + 26);
    } else if ((letter >= '0') && (letter <= '9')) {
        return ((letter - '0') + 52);
    } else if (letter == '+') {
        return 62;
    } else if (letter == '/') {
        return 63;
    } else {
        return 0;
    }
}

void encodings_base64_decode(const char* base64, char* decoded) {
    if (!base64 || !decoded) {
        return;
    }
    int tetrad_counter = 0;
    const char* ptr = base64;
    const char* end = ptr + strlen(base64);
    char input[4];
    char output[3];
    while (ptr < end) {
        memcpy(input, ptr, sizeof(input));
        memset(output, 0, sizeof(output));
        ptr += 4;

        size_t size_to_copy = 1;
        char first = base64_character_to_byte(input[0]);
        output[0] = (first << 2);
        char second = base64_character_to_byte(input[1]);
        output[0] |= (second >> 4);
        output[1] = ((second & 0xF) << 4);
        if (input[2] && !is_padding_character(input[2])) {
            size_to_copy = 2;
            char third = base64_character_to_byte(input[2]);
            output[1] |= (third >> 2);
            if (input[3] && !is_padding_character(input[3])) {
                size_to_copy = 3;
                char fourth = base64_character_to_byte(input[3]);
                output[2] = ((third & 0x3) << 6);
                output[2] |= fourth;
            }
        }

        memcpy(decoded, output, size_to_copy);
        decoded += 3;
    }
}
