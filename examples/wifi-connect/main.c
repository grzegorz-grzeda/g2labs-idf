#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "cli.h"
#include "g2l-delay.h"
#include "g2l-hal-uart.h"
#include "g2l-hal-wifi.h"
#include "g2l-log.h"

#define TAG "wifi-connect"

void cli_print(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

static char ssid[32];
static char password[64];

void set_ssid(cli_print_t print, int argc, char** argv) {
    if (argc != 2) {
        print("Usage: set_ssid <ssid>\n");
        return;
    }
    strcpy(ssid, argv[1]);
    print("SSID set to: %s\n", argv[1]);
}

void set_password(cli_print_t print, int argc, char** argv) {
    if (argc != 2) {
        print("Usage: set_password <password>\n");
        return;
    }
    strcpy(password, argv[1]);
    print("Password set to: %s\n", argv[1]);
}

void connect(cli_print_t print, int argc, char** argv) {
    g2l_hal_wifi_set(ssid, password);
    g2l_hal_wifi_connect();
    printf("Connecting...\n");
}

void disconnect(cli_print_t print, int argc, char** argv) {
    g2l_hal_wifi_disconnect();
    print("Disconnecting...\n");
}

void wifi_state_handler(bool is_connected) {
    if (is_connected) {
        printf("Connected!\n");
    } else {
        printf("Disconnected!\n");
    }
}

void uart_receive_handler(const uint8_t* data, size_t len) {
    for (size_t i = 0; i < len; i++) {
        process_cli(data[i]);
    }
}

int main(int argc, char** argv) {
    g2l_hal_uart_initialize(115200, uart_receive_handler);
    g2l_hal_uart_register_stdout();
    initialize_cli(cli_print);
    g2l_hal_wifi_set_on_connected_handler(wifi_state_handler);
    g2l_hal_wifi_set_on_disconnected_handler(wifi_state_handler);
    cli_entry_t set_ssid_entry = {
        .execute = set_ssid,
        .name = "ssid",
        .help = "Set the SSID for the WiFi connection",
    };
    register_cli(&set_ssid_entry);
    cli_entry_t set_password_entry = {
        .execute = set_password,
        .name = "password",
        .help = "Set the password for the WiFi connection",
    };
    register_cli(&set_password_entry);
    cli_entry_t connect_entry = {
        .execute = connect,
        .name = "connect",
        .help = "Connect to the WiFi network",
    };
    register_cli(&connect_entry);

    cli_entry_t disconnect_entry = {
        .execute = disconnect,
        .name = "disconnect",
        .help = "Disconnect from the WiFi network",
    };
    register_cli(&disconnect_entry);

    char message[] = "Hello, World!";
    I(TAG, "Sending: %s", message);
    for (int i = 0; i < argc; i++) {
        I(TAG, "Arg %d: %s", i, argv[i]);
    }
    while (1) {
        g2l_delay_ms(20);
    }
    return 0;
}