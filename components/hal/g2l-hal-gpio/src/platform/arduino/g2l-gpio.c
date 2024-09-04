/**
 * Copyright (C) G2Labs Grzegorz Grzęda - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Grzegorz Grzęda <grzegorz.grzeda@g2labs.pl>, 05.2023
 */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "g2l-hal-gpio.h"

#define PLATFORM_GPIO_PIN_NUMBER_MAX (48)

static g2l_hal_gpio_input_handler_t g2l_input_handler = NULL;

bool g2l_hal_gpio_initialize(g2l_gpio_input_handler_t input_handler) {
    return false;
}

bool g2l_hal_gpio_configure_input(uint8_t number,
                                  bool is_pull_up,
                                  g2l_gpio_input_edge_detect_t edge_detect) {
    return false;
}

bool g2l_hal_gpio_configure_output(uint8_t number,
                                   bool init_state,
                                   bool is_pull_up) {
    return g2l_gpio_update_output(number, init_state);
}

bool g2l_hal_gpio_update_output(uint8_t number, bool new_state) {
    return false;
}
