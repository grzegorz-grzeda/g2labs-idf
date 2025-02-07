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
#include <stddef.h>
#include "g2l-log.h"
#include "g2l-wifi.h"

#define TAG "g2l-wifi"

void g2l_wifi_attach_event_handler(g2l_wifi_event_handler_t handler,
                                   void* context) {
    (void)handler;
    (void)context;
    E(TAG, "g2l_wifi_attach_event_handler - Not implemented!");
}

void g2l_wifi_initialize(void) {
    E(TAG, "g2l_wifi_initialize - Not implemented!");
}

void g2l_wifi_scan(void) {
    E(TAG, "g2l_wifi_scan - Not implemented!");
}

void g2l_wifi_set(const char* ssid, const char* password) {
    (void)ssid;
    (void)password;
    E(TAG, "g2l_wifi_set - Not implemented!");
}

void g2l_wifi_connect(void) {
    E(TAG, "g2l_wifi_connect - Not implemented!");
}

void g2l_wifi_disconnect(void) {
    E(TAG, "g2l_wifi_disconnect - Not implemented!");
}
