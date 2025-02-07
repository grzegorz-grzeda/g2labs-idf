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
#ifndef G2L_FS_H
#define G2L_FS_H
#include <stdbool.h>
#include <stddef.h>

typedef enum g2l_fs_mode {
    G2L_FS_MODE_READ,
    G2L_FS_MODE_WRITE,
    G2L_FS_MODE_APPEND,
    G2L_FS_MODE_CREATE,
    G2L_FS_MODE_TRUNCATE,
} g2l_fs_mode_t;

typedef enum g2l_fs_seek_mode {
    G2L_FS_SEEK_SET,
    G2L_FS_SEEK_CUR,
    G2L_FS_SEEK_END,
} g2l_fs_seek_mode_t;

typedef struct g2l_fs_file g2l_fs_file_t;

void g2l_fs_initialize(const char* base_path);

size_t g2l_fs_file_size(const char* file_name);

g2l_fs_file_t* g2l_fs_file_open(const char* file_name, g2l_fs_mode_t mode);

void g2l_fs_file_close(g2l_fs_file_t* file);

size_t g2l_fs_file_read(g2l_fs_file_t* file, void* data, size_t max_data_size);

size_t g2l_fs_file_write(g2l_fs_file_t* file,
                         const void* data,
                         size_t data_size);

bool g2l_fs_file_seek(g2l_fs_file_t* file,
                      g2l_fs_seek_mode_t mode,
                      size_t offset);

#endif  // G2L_FS_H