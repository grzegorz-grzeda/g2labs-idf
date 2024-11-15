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
#include "g2l-mqtt.h"
#define TAG "g2l-mqtt"

typedef struct g2l_mqtt_client {
} g2l_mqtt_client_t;

g2l_mqtt_client_t* g2l_mqtt_create(g2l_mqtt_connection_t* connection) {
    return NULL;
}

void g2l_mqtt_attach_event_handler(g2l_mqtt_client_t* client,
                                   g2l_mqtt_event_handler_t handler,
                                   void* context) {}

void g2l_mqtt_connect(g2l_mqtt_client_t* client) {}

void g2l_mqtt_disconnect(g2l_mqtt_client_t* client) {}

void g2l_mqtt_subscribe(g2l_mqtt_client_t* client, const char* topic) {}

void g2l_mqtt_unsubscribe(g2l_mqtt_client_t* client, const char* topic) {}

void g2l_mqtt_publish(g2l_mqtt_client_t* client,
                      const char* topic,
                      const char* message,
                      size_t message_len) {}