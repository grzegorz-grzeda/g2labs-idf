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
#include "wifi-manager.h"
#include <string.h>
#include "g2l-hal-wifi.h"
#include "g2l-os-fs.h"
#include "simple-list.h"

#define WIFI_STORE_MAX_ENTRIES 10
#define WIFI_STORE_FILE_NAME "wifi_store"

typedef struct {
    const char* ssid;
    const char* password;
    int rssi;
    bool is_available;
} wifi_store_entry_t;

static simple_list_t wifi_store;

static wifi_manager_event_handler_t wifi_manager_event_handler;

static on_wifi_event(g2l_hal_wifi_event_t event, void* data) {
    switch (event) {
        case G2L_HAL_WIFI_STA_STARTED:
            g2l_hal_wifi_scan();
            break;
        case G2L_HAL_WIFI_STA_CONNECTED:
            if (wifi_manager_event_handler) {
                wifi_manager_event_handler(WIFI_MANAGER_STA_CONNECTED);
            }
            break;
        case G2L_HAL_WIFI_STA_DISCONNECTED:
            if (wifi_manager_event_handler) {
                wifi_manager_event_handler(WIFI_MANAGER_STA_DISCONNECTED);
            }
            break;
        case G2L_HAL_WIFI_SCAN_DONE:
            // compare against wifi_store
            // connect to the strongest available one
            break;
    }
}

static void save_wifi_store(void) {
    char buffer[1024];
    size_t buffer_size = 0;
    for (simple_list_iterator_t* it = simple_list_begin(wifi_store); it;
         it = simple_list_next(it)) {
        wifi_store_entry_t* entry = get_from_simple_list_iterator(it);
        buffer_size +=
            snprintf(buffer + buffer_size, sizeof(buffer) - buffer_size,
                     "%s,%s\n", entry->ssid, entry->password);
    }
    g2l_os_fs_store_file(WIFI_STORE_FILE_NAME, buffer, buffer_size);
}

static void add_to_wifi_store(const char* ssid, const char* password) {
    wifi_store_entry_t* entry = calloc(1, sizeof(wifi_store_entry_t));
    entry->ssid = calloc(1, strlen(ssid) + 1);
    entry->password = calloc(1, strlen(password) + 1);
    strcpy(entry->ssid, ssid);
    strcpy(entry->password, password);
    append_to_simple_list(wifi_store, entry);
}

static void load_wifi_store(void) {
    size_t file_size = g2l_os_fs_get_size_of_file(WIFI_STORE_FILE_NAME);

    if (!file_size) {
        save_wifi_store();
        return;
    }
    char file_content[1024];
    g2l_os_fs_load_file(WIFI_STORE_FILE_NAME, file_content,
                        sizeof(file_content));

    char* token = strtok(file_content, "\n");
    while (token != NULL) {
        char* ssid = strtok(token, ",");
        char* password = strtok(NULL, ",");

        add_to_wifi_store(ssid, password);

        token = strtok(NULL, "\n");
    }
}

void wifi_manager_initialize(wifi_manager_event_handler_t event_handler) {
    wifi_manager_event_handler = event_handler;
    wifi_store = create_simple_list();
    load_wifi_store();
    g2l_hal_wifi_initialize(on_wifi_event);
}

void wifi_manager_connect_to_strongest(void) {}

void wifi_manager_connect(const char* ssid) {}

void wifi_manager_disconnect(void) {
    g2l_hal_wifi_disconnect();
}

void wifi_manager_store(const char* ssid, const char* password) {
    add_to_wifi_store(ssid, password);
    save_wifi_store();
}