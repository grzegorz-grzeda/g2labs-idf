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
#include "g2labs-platform-ota.h"
#include "esp_err.h"
#include "esp_ota_ops.h"
#include "esp_system.h"

#define G2LABS_LOG_MODULE_NAME "g2labs-platform-ota"
#define G2LABS_LOG_MODULE_LEVEL (G2LABS_LOG_MODULE_LEVEL_INFO)
#include <g2labs-log.h>

static esp_ota_handle_t update_handle = 0;
static const esp_partition_t* update_partition = NULL;

bool platform_ota_initialize(void) {
    I("initialize");
    update_partition = esp_ota_get_next_update_partition(NULL);
    esp_err_t err = esp_ota_begin(update_partition, OTA_SIZE_UNKNOWN, &update_handle);
    return (err == ESP_OK);
}

void platform_ota_abort(void) {
    E("abort");
    esp_ota_abort(update_handle);
}

bool platform_ota_finalize(void) {
    I("finalize");
    esp_err_t err = esp_ota_end(update_handle);
    if (err == ESP_OK) {
        err = esp_ota_set_boot_partition(update_partition);
        if (err == ESP_OK) {
            return true;
        } else {
            E("%s", esp_err_to_name(err));
            return false;
        }
        return (err == ESP_OK);
    } else {
        E("%s", esp_err_to_name(err));
        return false;
    }
}

bool platform_ota_process_chunk(const uint8_t* data, size_t data_size, size_t offset) {
    D("send chunk offset=%lu size=%lu", offset, data_size);
    esp_err_t err = esp_ota_write_with_offset(update_handle, (const void*)data, data_size, offset);
    if (err != ESP_OK) {
        E("%s", esp_err_to_name(err));
        return false;
    } else {
        return true;
    }
}