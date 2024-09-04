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

#define G2LABS_LOG_MODULE_NAME "g2labs-platform-ota"
#define G2LABS_LOG_MODULE_LEVEL (G2LABS_LOG_MODULE_LEVEL_INFO)
#include <g2labs-log.h>

bool platform_ota_initialize(void) {
    I("initialize");
    return true;
}

void platform_ota_abort(void) {
    I("abort");
}

bool platform_ota_finalize(void) {
    I("finalize");
    return true;
}

bool platform_ota_process_chunk(const uint8_t* data, size_t data_size, size_t offset) {
    I("send chunk offset=%lu size=%lu", offset, data_size);
    return true;
}