#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "cli.h"
#include "g2l-hal-uart.h"
#include "g2l-hal-wifi.h"
#include "g2l-log.h"
#include "g2l-os-delay.h"

#define TAG "wifi-scan"

static void on_wifi_event(void* ctx, g2l_hal_wifi_event_t event, void* data) {
    if (event == G2L_HAL_WIFI_STA_STARTED) {
        I(TAG, "STA started");
        g2l_hal_wifi_scan();
    } else if (event == G2L_HAL_WIFI_STA_CONNECTED) {
        I(TAG, "Connected to WiFi");
    } else if (event == G2L_HAL_WIFI_STA_DISCONNECTED) {
        I(TAG, "Disconnected from WiFi");
    } else if (event == G2L_HAL_WIFI_SCAN_DONE) {
        g2l_hal_wifi_scan_t* scan = (g2l_hal_wifi_scan_t*)data;
        I(TAG, "Found %d APs:", scan->count);
        for (int i = 0; i < scan->count; i++) {
            I(TAG, "   SSID: '%s' @ RSSI: %d [%s]", scan->entries[i].ssid,
              scan->entries[i].rssi,
              scan->entries[i].is_secure ? "SECURE" : "OPEN");
        }
    }
}

int main(int argc, char** argv) {
    g2l_hal_wifi_attach_event_handler(on_wifi_event, NULL);
    g2l_hal_wifi_initialize();

    while (1) {
        g2l_os_delay_ms(100);
    }
    return 0;
}