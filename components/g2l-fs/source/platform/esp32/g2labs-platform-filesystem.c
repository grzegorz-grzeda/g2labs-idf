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
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "esp_err.h"
#include "esp_spiffs.h"
#include "esp_vfs.h"
#include "g2l-fs.h"

#include "g2l-log.h"

#define TAG "FS"

typedef struct g2l_fs_file {
    FILE* file;
} g2l_fs_file_t;

typedef char full_path_name[200];

static char fs_base_path[20] = "/fs";

static bool is_fs_initialized = false;

void g2l_fs_initialize(const char* base_path) {
    if (is_fs_initialized) {
        return;
    }
    if (base_path) {
        strcpy(fs_base_path, base_path);
    }
    esp_vfs_spiffs_conf_t conf = {.base_path = fs_base_path,
                                  .partition_label = NULL,
                                  .max_files = 5,
                                  .format_if_mount_failed = true};
    esp_err_t err = esp_vfs_spiffs_register(&conf);
    if (err != ESP_OK) {
        E(TAG, "Failed to mount SPIFFS (%s)", esp_err_to_name(err));
    }
    size_t total = 0, used = 0;
    err = esp_spiffs_info(conf.partition_label, &total, &used);
    if (err != ESP_OK) {
        E(TAG, "Failed to get SPIFFS partition information (%s). Formatting...",
          esp_err_to_name(err));
        esp_spiffs_format(conf.partition_label);
    } else {
        I(TAG, "Partition size: total: %d, used: %d", total, used);
    }
    is_fs_initialized = true;
}

static void create_full_path_name(full_path_name full_path, const char* name) {
    strcpy(full_path, fs_base_path);
    strcat(full_path, "/");
    strcat(full_path, name);
}

size_t g2l_fs_file_size(const char* file_name) {
    full_path_name path;
    create_full_path_name(path, file_name);
    struct stat info;
    int result = stat(path, &info);
    if (result == 0) {
        return (size_t)info.st_size;
    } else {
        E(TAG, "Failed to get file '%s' information (error: %s)", path,
          strerror(errno));
    }
    return 0;
}

g2l_fs_file_t* g2l_fs_file_open(const char* file_name, g2l_fs_mode_t mode) {
    full_path_name path;
    create_full_path_name(path, file_name);
    char* mode_str = NULL;
    if (mode == G2L_FS_MODE_READ) {
        mode_str = "r";
    } else if (mode == G2L_FS_MODE_WRITE) {
        mode_str = "w";
    } else if (mode == G2L_FS_MODE_APPEND) {
        mode_str = "a";
    } else if (mode == G2L_FS_MODE_CREATE) {
        mode_str = "w+";
    } else if (mode == G2L_FS_MODE_TRUNCATE) {
        mode_str = "w";
    }
    FILE* fp = fopen(path, mode_str);
    if (fp) {
        g2l_fs_file_t* file = (g2l_fs_file_t*)calloc(1, sizeof(g2l_fs_file_t));
        file->file = fp;
        return file;
    } else {
        E(TAG, "Failed to open file '%s' (error: %s)", path, strerror(errno));
        return NULL;
    }
}

void g2l_fs_file_close(g2l_fs_file_t* file) {
    if (file) {
        fclose(file->file);
        free(file);
    }
}

size_t g2l_fs_file_read(g2l_fs_file_t* file, void* data, size_t max_data_size) {
    if (!file) {
        return 0;
    }
    return fread(data, 1, max_data_size, file->file);
}

size_t g2l_fs_file_write(g2l_fs_file_t* file,
                         const void* data,
                         size_t data_size) {
    if (!file) {
        return 0;
    }
    return fwrite(data, 1, data_size, file->file);
}

bool g2l_fs_file_seek(g2l_fs_file_t* file,
                      g2l_fs_seek_mode_t mode,
                      size_t offset) {
    if (!file) {
        return 0;
    }
    int whence = SEEK_SET;
    if (mode == G2L_FS_SEEK_CUR) {
        whence = SEEK_CUR;
    } else if (mode == G2L_FS_SEEK_END) {
        whence = SEEK_END;
    }
    return (fseek(file->file, (long)offset, whence) == 0);
}
