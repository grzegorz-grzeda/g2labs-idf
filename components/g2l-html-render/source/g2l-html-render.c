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
#include "g2l-html-render.h"
#include <stdlib.h>
#include <string.h>
#include "g2l-log.h"

#define TAG "g2l-html-render"

typedef struct g2l_html_render_page {
    g2l_html_render_page_configuration_t config;
} g2l_html_render_page_t;

g2l_html_render_page_t* g2l_html_render_create_page(
    g2l_html_render_page_configuration_t* config) {
    if (!config || !config->html_template || !config->render_buffer ||
        !config->render_buffer_size || !config->entry_callback) {
        return NULL;
    }
    g2l_html_render_page_t* page = calloc(1, sizeof(g2l_html_render_page_t));
    if (!page) {
        return NULL;
    }
    memcpy(&page->config, config, sizeof(g2l_html_render_page_configuration_t));
    return page;
}

void g2l_html_render_destroy_page(g2l_html_render_page_t* page) {
    if (!page) {
        return;
    }
    free(page);
}

size_t g2l_html_render_page(g2l_html_render_page_t* page) {
    if (!page) {
        return 0;
    }
    char key[G2L_HTML_RENDER_PAGE_KEY_MAX_SIZE + 1] = {0};

    const char* read_ptr = page->config.html_template;
    char* write_ptr = page->config.render_buffer;
    size_t available_size = page->config.render_buffer_size;
    while (*read_ptr) {
        if (strncmp(read_ptr, G2L_HTML_RENDER_PAGE_TOKEN_OPEN,
                    sizeof(G2L_HTML_RENDER_PAGE_TOKEN_OPEN) - 1) == 0) {
            read_ptr += strlen(G2L_HTML_RENDER_PAGE_TOKEN_OPEN);
            char* end = strstr(read_ptr, G2L_HTML_RENDER_PAGE_TOKEN_CLOSE);
            if (end) {
                memset(key, 0, sizeof(key));
                memcpy(key, read_ptr, end - read_ptr);
                I(TAG, "Rendering for Key: %s", key);
                size_t write_size = page->config.entry_callback(
                    page->config.ctx, key, 0, write_ptr, available_size);
                read_ptr = end + strlen(G2L_HTML_RENDER_PAGE_TOKEN_CLOSE);
                write_ptr += write_size;
            } else {
                break;
            }
        } else {
            *write_ptr = *read_ptr;
            write_ptr++;
            read_ptr++;
        }
    }
    return (size_t)(write_ptr - page->config.render_buffer);
}