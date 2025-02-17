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
#include "g2str.h"
#include <string.h>

static char* find_substring_n(const char* haystack,
                              const char* needle,
                              size_t n) {
    size_t needle_len = strlen(needle);
    if (needle_len == 0) {
        return (char*)haystack;
    }

    for (size_t i = 0; i <= n - needle_len; i++) {
        if (strncmp(haystack + i, needle, needle_len) == 0) {
            return (char*)(haystack + i);
        }
    }

    return NULL;
}

g2str_t g2str(const char* str) {
    return g2nstr(str, strlen(str));
}

g2str_t g2nstr(const char* data, size_t size) {
    g2str_t g2str = {0};
    g2str._raw = (char*)data;
    g2str.data = (char*)data;
    g2str.size = size;
    return g2str;
}

bool g2str_eq(const g2str_t* str1, const g2str_t* str2) {
    if (!str1 || !str2) {
        return false;
    }
    if (str1->size != str2->size) {
        return false;
    }
    return (memcmp(str1->data, str2->data, str1->size) == 0);
}

g2str_t g2strstr(const g2str_t* str, const char* needle) {
    if (!str || !needle) {
        return g2nstr(NULL, 0);
    }
    char* found = find_substring_n(str->data, needle, str->size);
    if (!found) {
        return g2nstr(NULL, 0);
    }
    return g2nstr(found, strlen(needle));
}

g2str_t g2strseq(g2str_t* str, const char* sequence) {
    if (!str || !sequence) {
        return g2nstr(NULL, 0);
    }
    if (str->size < strlen(sequence)) {
        return g2nstr(NULL, 0);
    }
    char* found = find_substring_n(str->data, sequence, str->size);
    if (!found) {
        return g2nstr(NULL, 0);
    }
    return g2nstr(str->data, (size_t)(found - str->data));
}

void g2str_split(const g2str_t* str,
                 const char* needle,
                 g2str_t* left,
                 g2str_t* right) {
    if (!str || !needle || !left || !right) {
        return;
    }
    char* found = find_substring_n(str->data, needle, str->size);
    if (!found) {
        return;
    }
    *left = g2nstr(str->data, (size_t)(found - str->data));
    *right = g2nstr(found + strlen(needle),
                    str->size - (size_t)(found - str->data) - strlen(needle));
}

bool g2str_is_empty(const g2str_t* str) {
    if (!str) {
        return true;
    }
    return (str->size == 0);
}