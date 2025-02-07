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
#include <g2l-log.h>
#include <stdlib.h>
#include "mqtt_client.h"

#include "simple-list.h"

#define TAG "g2l-mqtt"

typedef struct {
    g2l_mqtt_event_handler_t handler;
    void* context;
} mqtt_event_handler_t;

typedef struct g2l_mqtt_client {
    simple_list_t* event_handlers;
    esp_mqtt_client_handle_t client;
    esp_mqtt_client_config_t mqtt_cfg;
} g2l_mqtt_client_t;

static void handle_mqtt_event(g2l_mqtt_client_t* client,
                              g2l_mqtt_event_t event) {
    if (!client) {
        return;
    }
    for (simple_list_iterator_t* it = simple_list_begin(client->event_handlers);
         it != NULL; it = simple_list_next(it)) {
        mqtt_event_handler_t* entry = get_from_simple_list_iterator(it);
        entry->handler(entry->context, client, event);
    }
}

static void handle_mqtt_connected_event(g2l_mqtt_client_t* client) {
    g2l_mqtt_event_t event = {
        .type = G2L_MQTT_EVENT_CONNECTED,
    };
    handle_mqtt_event(client, event);
}

static void handle_mqtt_disconnected_event(g2l_mqtt_client_t* client) {
    g2l_mqtt_event_t event = {
        .type = G2L_MQTT_EVENT_DISCONNECTED,
    };
    handle_mqtt_event(client, event);
}

static void handle_mqtt_subscribed_event(g2l_mqtt_client_t* client) {
    g2l_mqtt_event_t event = {
        .type = G2L_MQTT_EVENT_SUBSCRIBED,
    };
    handle_mqtt_event(client, event);
}

static void handle_mqtt_unsubscribed_event(g2l_mqtt_client_t* client) {
    g2l_mqtt_event_t event = {
        .type = G2L_MQTT_EVENT_UNSUBSCRIBED,
    };
    handle_mqtt_event(client, event);
}

static void handle_mqtt_published_event(g2l_mqtt_client_t* client) {
    g2l_mqtt_event_t event = {
        .type = G2L_MQTT_EVENT_MESSAGE_SENT,
    };
    handle_mqtt_event(client, event);
}

static void handle_mqtt_error_event(g2l_mqtt_client_t* client,
                                    g2l_mqtt_error_code_t error_code) {
    g2l_mqtt_event_t event = {
        .type = G2L_MQTT_EVENT_ERROR,
        .error_code = error_code,
    };
    handle_mqtt_event(client, event);
}

static void handle_mqtt_data_event(g2l_mqtt_client_t* client,
                                   const char* topic,
                                   size_t topic_len,
                                   const char* data,
                                   size_t data_len) {
    g2l_mqtt_event_t event = {
        .type = G2L_MQTT_EVENT_MESSAGE_RECEIVED,
        .message =
            {
                .topic = topic,
                .topic_len = topic_len,
                .message = data,
                .message_len = data_len,
            },
    };
    handle_mqtt_event(client, event);
}

static void mqtt_event_handler(void* handler_args,
                               esp_event_base_t base,
                               int32_t event_id,
                               void* event_data) {
    g2l_mqtt_client_t* client = handler_args;
    esp_mqtt_event_handle_t event = event_data;
    int msg_id;
    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            D(TAG, "MQTT_EVENT_CONNECTED");
            handle_mqtt_connected_event(client);
            break;
        case MQTT_EVENT_DISCONNECTED:
            D(TAG, "MQTT_EVENT_DISCONNECTED");
            handle_mqtt_disconnected_event(client);
            break;
        case MQTT_EVENT_SUBSCRIBED:
            D(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            handle_mqtt_subscribed_event(client);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            D(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            handle_mqtt_unsubscribed_event(client);
            break;
        case MQTT_EVENT_PUBLISHED:
            D(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            handle_mqtt_published_event(client);
            break;
        case MQTT_EVENT_DATA:
            D(TAG, "MQTT_EVENT_DATA");
            handle_mqtt_data_event(client, event->topic, event->topic_len,
                                   event->data, event->data_len);
            break;
        case MQTT_EVENT_ERROR:
            D(TAG, "MQTT_EVENT_ERROR");
            handle_mqtt_error_event(client, event->error_handle->error_type);
            break;
        default:
            D(TAG, "Other event id:%d", event->event_id);
            break;
    }
}

g2l_mqtt_client_t* g2l_mqtt_create(g2l_mqtt_connection_t* connection) {
    if (!connection) {
        return NULL;
    }
    g2l_mqtt_client_t* client =
        (g2l_mqtt_client_t*)calloc(1, sizeof(g2l_mqtt_client_t));
    if (!client) {
        E(TAG, "Failed to allocate memory for MQTT client");
        return NULL;
    }
    client->event_handlers = create_simple_list();
    if (!client->event_handlers) {
        E(TAG, "Failed to create event handlers list");
        free(client);
        return NULL;
    }
    client->mqtt_cfg.broker.address.uri = connection->host;
    client->mqtt_cfg.broker.address.port = connection->port;

    client->client = esp_mqtt_client_init(&client->mqtt_cfg);
    if (!client->client) {
        E(TAG, "Failed to initialize MQTT client");
        free(client);
        return NULL;
    }
    esp_err_t returnCode = esp_mqtt_client_register_event(
        client->client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    if (returnCode != ESP_OK) {
        E(TAG, "Failed to register MQTT main event handler");
        esp_mqtt_client_destroy(client->client);
        free(client);
        return NULL;
    }

    return client;
}

void g2l_mqtt_attach_event_handler(g2l_mqtt_client_t* client,
                                   g2l_mqtt_event_handler_t handler,
                                   void* context) {
    if (!client || !handler) {
        return;
    }
    mqtt_event_handler_t* event_handler =
        (mqtt_event_handler_t*)calloc(1, sizeof(mqtt_event_handler_t));
    if (!event_handler) {
        E(TAG, "Failed to allocate memory for event handler");
        return;
    }
    event_handler->handler = handler;
    event_handler->context = context;
    append_to_simple_list(client->event_handlers, event_handler);
}

void g2l_mqtt_connect(g2l_mqtt_client_t* client) {
    if (!client) {
        return;
    }
    esp_mqtt_client_start(client->client);
}

void g2l_mqtt_disconnect(g2l_mqtt_client_t* client) {}

void g2l_mqtt_subscribe(g2l_mqtt_client_t* client, const char* topic) {
    if (!client || !topic) {
        return;
    }
    esp_mqtt_client_subscribe(client->client, topic, 2);
}

void g2l_mqtt_unsubscribe(g2l_mqtt_client_t* client, const char* topic) {}

void g2l_mqtt_publish(g2l_mqtt_client_t* client,
                      const char* topic,
                      const char* message,
                      size_t message_len) {
    if (!client || !topic || !message) {
        return;
    }
    esp_mqtt_client_publish(client->client, topic, message, message_len, 0, 0);
}