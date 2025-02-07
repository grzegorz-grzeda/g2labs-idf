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
#ifndef G2L_WIFI_H
#define G2L_WIFI_H

#include <stdbool.h>
#include <stdint.h>

#define G2L_WIFI_SSID_MAX_LENGTH 32
#define G2L_WIFI_RETRY_COUNT 10

typedef enum {
    G2L_WIFI_STA_STARTED,
    G2L_WIFI_STA_CONNECTED,
    G2L_WIFI_STA_RECONNECTING,
    G2L_WIFI_STA_DISCONNECTED,
    G2L_WIFI_SCAN_DONE,
} g2l_wifi_event_t;

typedef struct {
    char ssid[G2L_WIFI_SSID_MAX_LENGTH + 1];
    int rssi;
    bool is_secure;
} g2l_wifi_scan_entry_t;

typedef struct {
    g2l_wifi_scan_entry_t* entries;
    uint16_t count;
} g2l_wifi_scan_t;

typedef void (*g2l_wifi_event_handler_t)(void* context,
                                         g2l_wifi_event_t event,
                                         void* data);

void g2l_wifi_attach_event_handler(g2l_wifi_event_handler_t handler,
                                   void* context);

void g2l_wifi_initialize(void);

void g2l_wifi_scan(void);

void g2l_wifi_set(const char* ssid, const char* password);

void g2l_wifi_connect(void);

void g2l_wifi_disconnect(void);

#endif  // G2L_WIFI_H