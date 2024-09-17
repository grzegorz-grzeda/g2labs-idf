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

static void mqtt_event_handler(void* handler_args,
                               esp_event_base_t base,
                               int32_t event_id,
                               void* event_data) {
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    switch ((esp_mqtt_event_id_t)event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            msg_id = esp_mqtt_client_subscribe(client, "/topic/qos0", 0);
            ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            break;
        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
            printf("DATA=%.*s\r\n", event->data_len, event->data);
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            break;
        default:
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
    }
}

g2l_mqtt_client_t* g2l_mqtt_create(void) {
    g2l_mqtt_client_t* client =
        (g2l_mqtt_client_t*)malloc(sizeof(g2l_mqtt_client_t));
    if (!client) {
        G2L_LOGE(TAG, "Failed to allocate memory for MQTT client");
        return NULL;
    }
    client->event_handlers = create_simple_list();
    if (!client->event_handlers) {
        G2L_LOGE(TAG, "Failed to create event handlers list");
        free(client);
        return NULL;
    }
    return client;
}

void g2l_mqtt_destroy(g2l_mqtt_client_t* client) {
    if (!client) {
        return;
    }
    if (client->event_handlers) {
        simple_list_destroy(client->event_handlers);
    }
    free(client);
}

void g2l_mqtt_attach_event_handler(g2l_mqtt_client_t* client,
                                   g2l_mqtt_event_handler_t handler,
                                   void* context) {
    if (!client || !handler) {
        return;
    }
    mqtt_event_handler_t* event_handler =
        (mqtt_event_handler_t*)malloc(sizeof(mqtt_event_handler_t));
    if (!event_handler) {
        G2L_LOGE(TAG, "Failed to allocate memory for event handler");
        return;
    }
    event_handler->handler = handler;
    event_handler->context = context;
    append_to_simple_list(client->event_handlers, event_handler);
}

void g2l_mqtt_connect(g2l_mqtt_client_t* client,
                      g2l_mqtt_connection_t* connection) {
    esp_mqtt_client_config_t mqtt_cfg = {
        .uri = "mqtt://broker.hivemq.com",  // Replace with your MQTT broker URI
    };

    client->client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client->client, ESP_EVENT_ANY_ID,
                                   mqtt_event_handler, client);
    esp_mqtt_client_start(client->client);
}

void g2l_mqtt_disconnect(g2l_mqtt_client_t* client) {}

void g2l_mqtt_subscribe(g2l_mqtt_client_t* client, const char* topic) {}

void g2l_mqtt_unsubscribe(g2l_mqtt_client_t* client, const char* topic) {}

void g2l_mqtt_publish(g2l_mqtt_client_t* client,
                      const char* topic,
                      const char* message,
                      size_t message_len) {}