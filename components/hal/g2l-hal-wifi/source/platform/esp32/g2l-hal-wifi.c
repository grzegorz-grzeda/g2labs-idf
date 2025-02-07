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
#include "g2l-hal-wifi.h"
#include "esp_event.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "g2l-log.h"
#include "nvs_flash.h"

#include "simple-list.h"

#include <stdbool.h>
#include <string.h>

#define TAG "g2l-hal-wifi"

typedef struct {
    g2l_hal_wifi_event_handler_t handler;
    void* context;
} wifi_event_handler_t;

static bool first_time = true;
static volatile int retry_count = HAL_WIFI_RETRY_COUNT;
static char local_ssid[32] = "";
static char local_password[32] = "";

static simple_list_t* event_handlers = NULL;

static void execute_event_handlers(g2l_hal_wifi_event_t event, void* data) {
    for (simple_list_iterator_t* it = simple_list_begin(event_handlers); it;
         it = simple_list_next(it)) {
        wifi_event_handler_t* handler =
            (wifi_event_handler_t*)get_from_simple_list_iterator(it);
        handler->handler(handler->context, event, data);
    }
}

static void handle_started(void) {
    I(TAG, "STA started");
    execute_event_handlers(G2L_HAL_WIFI_STA_STARTED, NULL);
}

static void handle_on_connection_state_update(g2l_hal_wifi_event_t event) {
    if (event == G2L_HAL_WIFI_STA_CONNECTED) {
        I(TAG, "connected");
    } else if (event == G2L_HAL_WIFI_STA_DISCONNECTED) {
        I(TAG, "disconnected");
    } else if (event == G2L_HAL_WIFI_STA_RECONNECTING) {
        I(TAG, "reconnecting");
    }
    execute_event_handlers(event, NULL);
}

static void handle_scan(void) {
    g2l_hal_wifi_scan_t* scan = calloc(1, sizeof(g2l_hal_wifi_scan_t));
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&scan->count));
    I(TAG, "Found %d APs:", scan->count);

    if (scan->count == 0) {
        execute_event_handlers(G2L_HAL_WIFI_SCAN_DONE, scan);
        free(scan);
        return;
    }
    wifi_ap_record_t* ap_info = calloc(scan->count, sizeof(wifi_ap_record_t));
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&scan->count, ap_info));

    scan->entries = calloc(scan->count, sizeof(g2l_hal_wifi_scan_entry_t));

    for (int i = 0; i < scan->count; i++) {
        scan->entries[i].rssi = ap_info[i].rssi;
        scan->entries[i].is_secure = ap_info[i].authmode != WIFI_AUTH_OPEN;
        strncpy(scan->entries[i].ssid, (char*)ap_info[i].ssid,
                HAL_WIFI_SSID_MAX_LENGTH);
    }
    execute_event_handlers(G2L_HAL_WIFI_SCAN_DONE, scan);
    free(scan->entries);
    free(scan);
    free(ap_info);
}

static void on_wifi_event_handler(void* arg,
                                  esp_event_base_t event_base,
                                  int32_t event_id,
                                  void* event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        handle_started();
    } else if (event_base == WIFI_EVENT &&
               event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (retry_count > 0) {
            retry_count--;
            handle_on_connection_state_update(G2L_HAL_WIFI_STA_RECONNECTING);
            esp_wifi_connect();
        } else {
            I(TAG, "Connection retry count exceeded");
            handle_on_connection_state_update(G2L_HAL_WIFI_STA_DISCONNECTED);
        }
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*)event_data;
        I(TAG, "connected, IP=" IPSTR, IP2STR(&event->ip_info.ip));
        handle_on_connection_state_update(G2L_HAL_WIFI_STA_CONNECTED);
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_SCAN_DONE) {
        I(TAG, "Scan done");
        handle_scan();
    }
}

void g2l_hal_wifi_attach_event_handler(g2l_hal_wifi_event_handler_t handler,
                                       void* context) {
    if (!event_handlers) {
        event_handlers = create_simple_list();
    }
    if (!event_handlers) {
        return;
    }

    wifi_event_handler_t* handler_entry =
        calloc(1, sizeof(wifi_event_handler_t));
    if (!handler_entry) {
        return;
    }
    handler_entry->handler = handler;
    handler_entry->context = context;
    append_to_simple_list(event_handlers, handler_entry);
}

void g2l_hal_wifi_initialize(void) {
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        WIFI_EVENT, ESP_EVENT_ANY_ID, &on_wifi_event_handler, NULL,
        &instance_any_id));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        IP_EVENT, IP_EVENT_STA_GOT_IP, &on_wifi_event_handler, NULL,
        &instance_got_ip));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
}

void g2l_hal_wifi_scan(void) {
    static wifi_scan_config_t config = {
        .show_hidden = true,
        .scan_type = WIFI_SCAN_TYPE_ACTIVE,
        .scan_time =
            {
                .active =
                    {
                        .min = 100,
                        .max = 1000,
                    },
                .passive = 500,
            },
    };

    esp_wifi_scan_start(&config, false);
}

void g2l_hal_wifi_set(const char* ssid, const char* password) {
    strcpy(local_ssid, ssid);
    strcpy(local_password, password);
}

void g2l_hal_wifi_connect(void) {
    wifi_config_t wifi_config = {
        .sta =
            {
                .threshold.authmode = WIFI_AUTH_WPA2_PSK,
                .sae_pwe_h2e = WPA3_SAE_PWE_BOTH,
                .sae_h2e_identifier = "",
            },
    };
    strcpy(wifi_config.sta.ssid, local_ssid);
    strcpy(wifi_config.sta.password, local_password);
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    retry_count = HAL_WIFI_RETRY_COUNT;
    esp_wifi_connect();
}

void g2l_hal_wifi_disconnect(void) {
    retry_count = 0;
    esp_wifi_disconnect();
}
