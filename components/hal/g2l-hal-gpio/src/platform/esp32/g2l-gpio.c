/**
 * Copyright (C) G2Labs Grzegorz Grzęda - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Grzegorz Grzęda <grzegorz.grzeda@g2labs.pl>, 05.2023
 */
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "g2l-hal-gpio.h"

#define PLATFORM_GPIO_PIN_NUMBER_MAX (48)

#define ERR_RET(condition, return_value)           \
    do {                                           \
        int _ERR_RET_CONDITION = (int)(condition); \
        if (_ERR_RET_CONDITION == 0) {             \
            return (return_value);                 \
        }                                          \
    } while (0)

static g2l_hal_gpio_input_handler_t g2l_input_handler = NULL;

bool g2l_hal_gpio_initialize(g2l_hal_gpio_input_handler_t input_handler) {
    esp_err_t result = gpio_install_isr_service(0);
    ERR_RET(result == ESP_OK, false);
    g2l_input_handler = input_handler;
    return true;
}

static void IRAM_ATTR gpi_isr_handler(void* arg) {
    uint8_t number = (uint8_t)(uint32_t)arg;
    if (g2l_input_handler) {
        g2l_input_handler(number);
    }
}

bool g2l_hal_gpio_configure_input(
    uint8_t number,
    bool is_pull_up,
    g2l_hal_gpio_input_edge_detect_t edge_detect) {
    ERR_RET(number < PLATFORM_GPIO_PIN_NUMBER_MAX, false);
    ERR_RET(edge_detect < _PLATFORM_GPIO_INPUT_EDGE_DETECT_COUNT, false);
    gpio_config_t io_conf = {};
    if (edge_detect == PLATFORM_GPIO_INPUT_EDGE_DETECT_RISING) {
        io_conf.intr_type = GPIO_INTR_POSEDGE;
    } else if (edge_detect == PLATFORM_GPIO_INPUT_EDGE_DETECT_FALLING) {
        io_conf.intr_type = GPIO_INTR_NEGEDGE;
    } else {
        io_conf.intr_type = GPIO_INTR_ANYEDGE;
    }
    io_conf.pin_bit_mask = (uint64_t)(1 << number);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en =
        (is_pull_up ? GPIO_PULLUP_ENABLE : GPIO_PULLUP_DISABLE);
    esp_err_t result = gpio_config(&io_conf);
    ERR_RET(result == ESP_OK, false);
    result =
        gpio_isr_handler_add(number, gpi_isr_handler, (void*)(uint32_t)number);
    ERR_RET(result == ESP_OK, false);
    return true;
}

bool g2l_hal_gpio_configure_output(uint8_t number,
                                   bool init_state,
                                   bool is_pull_up) {
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (uint64_t)(1 << number);
    io_conf.pull_up_en = (is_pull_up ? 1 : 0);
    esp_err_t result = gpio_config(&io_conf);
    ERR_RET(result == ESP_OK, false);
    return g2l_hal_gpio_update_output(number, init_state);
}

bool g2l_hal_gpio_update_output(uint8_t number, bool new_state) {
    esp_err_t result = gpio_set_level(number, new_state);
    ERR_RET(result == ESP_OK, false);
    return true;
}
