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
#ifndef G2L_MQTT_H
#define G2L_MQTT_H

#include <stddef.h>

typedef struct g2l_mqtt_client g2l_mqtt_client_t;

typedef enum {
    G2L_MQTT_EVENT_CONNECTED,
    G2L_MQTT_EVENT_DISCONNECTED,
    G2L_MQTT_EVENT_SUBSCRIBED,
    G2L_MQTT_EVENT_UNSUBSCRIBED,
    G2L_MQTT_EVENT_MESSAGE_RECEIVED,
    G2L_MQTT_EVENT_MESSAGE_SENT,
    G2L_MQTT_EVENT_ERROR,
} g2l_mqtt_event_type_t;

typedef struct {
    const char* topic;
    size_t topic_len;
    const char* message;
    size_t message_len;
} g2l_mqtt_message_t;

typedef enum {
    G2L_MQTT_ERROR_CONNECTION_FAILED,
    G2L_MQTT_ERROR_SUBSCRIPTION_FAILED,
    G2L_MQTT_ERROR_UNSUBSCRIPTION_FAILED,
    G2L_MQTT_ERROR_PUBLISH_FAILED,
} g2l_mqtt_error_code_t;

typedef struct {
    g2l_mqtt_event_type_t type;
    union {
        g2l_mqtt_message_t message;
        g2l_mqtt_error_code_t error_code;
    };
} g2l_mqtt_event_t;

typedef struct {
    char* host;
    int port;
    const char* client_id;
    const char* username;
    const char* password;
} g2l_mqtt_connection_t;

typedef void (*g2l_mqtt_event_handler_t)(void* context,
                                         g2l_mqtt_client_t* mqtt,
                                         g2l_mqtt_event_t event);

g2l_mqtt_client_t* g2l_mqtt_create(g2l_mqtt_connection_t* connection);

void g2l_mqtt_attach_event_handler(g2l_mqtt_client_t* client,
                                   g2l_mqtt_event_handler_t handler,
                                   void* context);

void g2l_mqtt_connect(g2l_mqtt_client_t* client);

void g2l_mqtt_disconnect(g2l_mqtt_client_t* client);

void g2l_mqtt_subscribe(g2l_mqtt_client_t* client, const char* topic);

void g2l_mqtt_unsubscribe(g2l_mqtt_client_t* client, const char* topic);

void g2l_mqtt_publish(g2l_mqtt_client_t* client,
                      const char* topic,
                      const char* message,
                      size_t message_len);

#endif  // G2L_MQTT_H