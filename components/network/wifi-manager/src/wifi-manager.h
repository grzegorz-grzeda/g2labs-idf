/*
 * MIT License
 *
 * Copyright (c) 2024 Grzegorz Grzęda
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
#ifndef G2L_IDF_WIFI_MANAGER_H
#define G2L_IDF_WIFI_MANAGER_H

#include <stdint.h>

typedef enum {
    WIFI_MANAGER_STA_CONNECTED,
    WIFI_MANAGER_STA_DISCONNECTED,
} wifi_manager_event_t;

typedef void (*wifi_manager_event_handler_t)(wifi_manager_event_t event);

void wifi_manager_initialize(wifi_manager_event_handler_t event_handler);

void wifi_manager_connect_to_strongest(void);

void wifi_manager_connect(const char* ssid);

void wifi_manager_disconnect(void);

void wifi_manager_store(const char* ssid,
                        const char* password,
                        uint8_t priority);

#endif  // G2L_IDF_WIFI_MANAGER_H