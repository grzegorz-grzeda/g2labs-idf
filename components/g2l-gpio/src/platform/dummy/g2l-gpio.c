/**
 * Copyright (C) G2Labs Grzegorz Grzęda - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Grzegorz Grzęda <grzegorz.grzeda@g2labs.pl>, 05.2023
 */
#include "g2l-gpio.h"

bool g2l_gpio_initialize(g2l_gpio_input_handler_t input_handler) {
    return false;
}

bool g2l_gpio_configure_input(uint8_t number,
                              bool is_pull_up,
                              g2l_gpio_input_edge_detect_t edge_detect) {
    return false;
}

bool g2l_gpio_configure_output(uint8_t number,
                               bool init_state,
                               bool is_pull_up) {
    return g2l_gpio_update_output(number, init_state);
}

bool g2l_gpio_update_output(uint8_t number, bool new_state) {
    return false;
}
