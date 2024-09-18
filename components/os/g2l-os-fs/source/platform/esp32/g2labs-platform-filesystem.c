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
#include "g2l-os-fs.h"

#include "g2l-log.h"

#define TAG "FS"

typedef char full_path_name[200];

static bool is_fs_initialized = false;

void g2l_os_fs_initialize(void) {
    if (is_fs_initialized) {
        return;
    }
    esp_vfs_spiffs_conf_t conf = {.base_path = "/fs",
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
    strcpy(full_path, "/fs/");
    strcat(full_path, name);
}

size_t g2l_os_fs_get_size_of_file(const char* file_name) {
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

size_t g2l_os_fs_store_file(const char* file_name,
                            const void* data,
                            size_t data_size) {
    full_path_name path;
    create_full_path_name(path, file_name);
    FILE* fp = fopen(path, "w");
    size_t result_size = 0;
    if (fp) {
        result_size = fwrite(data, 1, data_size, fp);
        fclose(fp);
    } else {
        E(TAG, "Failed to open file '%s' to write (error: %s)", path,
          strerror(errno));
    }
    return result_size;
}

size_t g2l_os_fs_load_file(const char* file_name,
                           void* data,
                           size_t max_data_size) {
    full_path_name path;
    create_full_path_name(path, file_name);
    FILE* fp = fopen(path, "r");
    size_t result_size = 0;
    if (fp) {
        result_size = fread(data, 1, max_data_size, fp);
        fclose(fp);
    } else {
        E(TAG, "Failed to open file '%s' to read (error: %s)", path,
          strerror(errno));
    }
    return result_size;
}