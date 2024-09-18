#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "g2l-hal-wifi.h"
#include "g2l-log.h"
#include "g2l-mqtt.h"
#include "g2l-os-delay.h"
#include "g2l-os-mutex.h"

#define TAG "mqtt-connect"

#define WIFI_SSID "<SSID>"
#define WIFI_PASSWORD "<PASSWORD>"
#define MQTT_HOST "<BROKER URI"
#define MQTT_PORT 1883
#define MQTT_TOPIC_SUBSCRIBE "<TOPIC>"

static g2l_os_mutex_t* mutex = NULL;

static void on_wifi_event(void* ctx, g2l_hal_wifi_event_t event, void* data) {
    if (event == G2L_HAL_WIFI_STA_STARTED) {
        I(TAG, "STA started");
    } else if (event == G2L_HAL_WIFI_STA_CONNECTED) {
        I(TAG, "Connected to WiFi");
        g2l_os_mutex_unlock(mutex);
    } else if (event == G2L_HAL_WIFI_STA_DISCONNECTED) {
        I(TAG, "Disconnected from WiFi");
    }
}

static void on_mqtt_event(void* context,
                          g2l_mqtt_client_t* mqtt,
                          g2l_mqtt_event_t event) {
    if (event.type == G2L_MQTT_EVENT_CONNECTED) {
        I(TAG, "Connected to MQTT broker");
        g2l_mqtt_subscribe(mqtt, MQTT_TOPIC_SUBSCRIBE);
    } else if (event.type == G2L_MQTT_EVENT_DISCONNECTED) {
        I(TAG, "Disconnected from MQTT broker");
    } else if (event.type == G2L_MQTT_EVENT_ERROR) {
        E(TAG, "MQTT error: %d", event.error_code);
    } else if (event.type == G2L_MQTT_EVENT_MESSAGE_RECEIVED) {
        I(TAG, "Received message on topic '%.*s': '%.*s'",
          event.message.topic_len, event.message.topic,
          event.message.message_len, event.message.message);
    } else if (event.type == G2L_MQTT_EVENT_MESSAGE_SENT) {
        I(TAG, "Sent message");
    } else if (event.type == G2L_MQTT_EVENT_SUBSCRIBED) {
        I(TAG, "Subscribed to topic");
    } else if (event.type == G2L_MQTT_EVENT_UNSUBSCRIBED) {
        I(TAG, "Unsubscribed from topic");
    }
}

int main(int argc, char** argv) {
    g2l_hal_wifi_initialize();
    g2l_hal_wifi_attach_event_handler(on_wifi_event, NULL);
    g2l_hal_wifi_set(WIFI_SSID, WIFI_PASSWORD);

    g2l_mqtt_connection_t connection = {
        .host = MQTT_HOST,
        .port = MQTT_PORT,
        .client_id = "g2l-esp32-led0",
        .username = NULL,
        .password = NULL,
    };
    g2l_mqtt_client_t* mqtt = g2l_mqtt_create(&connection);
    g2l_mqtt_attach_event_handler(mqtt, on_mqtt_event, mqtt);

    mutex = g2l_os_mutex_create();
    g2l_os_mutex_lock(mutex);
    g2l_hal_wifi_connect();

    g2l_os_mutex_lock(mutex);
    I(TAG, "Connecting to MQTT broker");
    g2l_mqtt_connect(mqtt);

    while (1) {
        g2l_os_delay_ms(100);
    }
    return 0;
}