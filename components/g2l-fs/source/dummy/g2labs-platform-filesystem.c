/*
 * MIT License
 *
 * Copyright (c) 2023 G2Labs Grzegorz Grzęda
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
#include "g2l-fs.h"

#include "g2l-log.h"

#define TAG "g2l-fs"

void g2l_fs_initialize(const char* base_path) {
    (void)base_path;
    E(TAG, "g2l_fs_initialize - Not implemented!");
}

size_t g2l_fs_file_size(const char* file_name) {
    E(TAG, "g2l_fs_file_size - Not implemented!");
    return 0;
}

g2l_fs_file_t* g2l_fs_file_open(const char* file_name, g2l_fs_mode_t mode) {
    (void)file_name;
    (void)mode;
    E(TAG, "g2l_fs_file_open - Not implemented!");
    return NULL;
}

void g2l_fs_file_close(g2l_fs_file_t* file) {
    (void)file;
    E(TAG, "g2l_fs_file_close - Not implemented!");
}

size_t g2l_fs_file_read(g2l_fs_file_t* file, void* data, size_t max_data_size) {
    (void)file;
    (void)data;
    (void)max_data_size;
    E(TAG, "g2l_fs_file_read - Not implemented!");
    return 0;
}

size_t g2l_fs_file_write(g2l_fs_file_t* file,
                         const void* data,
                         size_t data_size) {
    (void)file;
    (void)data;
    (void)data_size;
    E(TAG, "g2l_fs_file_write - Not implemented!");
    return 0;
}

bool g2l_fs_file_seek(g2l_fs_file_t* file,
                      g2l_fs_seek_mode_t mode,
                      size_t offset) {
    (void)file;
    (void)mode;
    (void)offset;
    E(TAG, "g2l_fs_file_seek - Not implemented!");
    return false;
}
