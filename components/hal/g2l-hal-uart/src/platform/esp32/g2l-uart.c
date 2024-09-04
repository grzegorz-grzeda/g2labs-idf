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
#include <stdio.h>
#include "driver/uart.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "g2l-hal-uart.h"

#define ECHO_TEST_TXD (UART_PIN_NO_CHANGE)
#define ECHO_TEST_RXD (UART_PIN_NO_CHANGE)
#define ECHO_TEST_RTS (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS (UART_PIN_NO_CHANGE)

#define ECHO_UART_PORT_NUM (0)
#define ECHO_UART_BAUD_RATE (115200)
#define ECHO_TASK_STACK_SIZE (3500)

#define UART_BUF_SIZE (200)
#define UART_MAX_CONSECUTIVE_RX_SIZE (30)

#define ECHO_LOCAL_ECHO (true)

static g2l_hal_uart_receive_handler_t uart_receive_handler = NULL;

static void uart_data_receive_task(void* arg) {
    uint32_t baudrate = *(uint32_t*)arg;
    uart_config_t uart_config = {
        .baud_rate = baudrate,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    int intr_alloc_flags = 0;

    ESP_ERROR_CHECK(uart_driver_install(ECHO_UART_PORT_NUM, UART_BUF_SIZE * 2,
                                        0, 0, NULL, intr_alloc_flags));
    ESP_ERROR_CHECK(uart_param_config(ECHO_UART_PORT_NUM, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(ECHO_UART_PORT_NUM, ECHO_TEST_TXD,
                                 ECHO_TEST_RXD, ECHO_TEST_RTS, ECHO_TEST_CTS));

    uint8_t* data = (uint8_t*)malloc(UART_MAX_CONSECUTIVE_RX_SIZE);

    while (1) {
        int len = uart_read_bytes(ECHO_UART_PORT_NUM, data,
                                  (UART_MAX_CONSECUTIVE_RX_SIZE - 1),
                                  20 / portTICK_PERIOD_MS);
        if (len) {
            if (ECHO_LOCAL_ECHO) {
                uart_write_bytes(ECHO_UART_PORT_NUM, (const char*)data, len);
            }
            data[len] = '\0';
            if (uart_receive_handler) {
                uart_receive_handler(data, len);
            }
        }
    }
}

void g2l_hal_uart_initialize(uint32_t baudrate,
                             g2l_hal_uart_receive_handler_t handler) {
    uart_receive_handler = handler;
    xTaskCreate(uart_data_receive_task, "uart_data_receive_task",
                ECHO_TASK_STACK_SIZE, &baudrate, 10, NULL);
}

void g2l_hal_uart_send_data(const uint8_t* data, size_t size) {
    printf("%.*s", (int)size, (const char*)data);
}

void g2l_hal_uart_register_stdout(void) {}