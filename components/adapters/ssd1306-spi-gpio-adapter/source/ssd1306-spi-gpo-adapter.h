/**
 * Copyright (C) G2Labs Grzegorz Grzęda - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Grzegorz Grzęda <grzegorz.grzeda@g2labs.pl>, 03.2023
 */
#ifndef SSD1306_SPI_GPO_ADAPTER_H
#define SSD1306_SPI_GPO_ADAPTER_H

#include <stddef.h>
#include <stdint.h>
#include "g2l-hal-spi.h"
#include "generic-controller.h"

typedef struct ssd1306_spi_gpo_adapter_configuration {
    g2l_hal_spi_t* spi;
    uint8_t ncss_pin;
    uint8_t dc_pin;
} ssd1306_spi_gpo_adapter_configuration_t;

generic_controller_t* ssd1306_spi_gpo_adapter_create(
    ssd1306_spi_gpo_adapter_configuration_t configuration);

#endif  // SSD1306_SPI_GPO_ADAPTER_H