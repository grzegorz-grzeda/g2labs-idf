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
#ifndef G2L_HTML_RENDER_H
#define G2L_HTML_RENDER_H

#include <stddef.h>

#define G2L_HTML_RENDER_PAGE_TOKEN_OPEN "[%"
#define G2L_HTML_RENDER_PAGE_TOKEN_CLOSE "%]"
#define G2L_HTML_RENDER_PAGE_KEY_MAX_SIZE (32)

typedef size_t (*g2l_html_render_entry_page_callback_t)(
    void* ctx,
    const char* template_key,
    int index,
    char* buffer,
    size_t buffer_size);

typedef struct g2l_html_render_page_configuration {
    const char* html_template;
    char* render_buffer;
    size_t render_buffer_size;
    g2l_html_render_entry_page_callback_t entry_callback;
    void* ctx;
} g2l_html_render_page_configuration_t;

typedef struct g2l_html_render_page g2l_html_render_page_t;

g2l_html_render_page_t* g2l_html_render_create_page(
    g2l_html_render_page_configuration_t* config);

void g2l_html_render_destroy_page(g2l_html_render_page_t* page);

size_t g2l_html_render_page(g2l_html_render_page_t* page);

#endif  // G2L_HTML_RENDER_H