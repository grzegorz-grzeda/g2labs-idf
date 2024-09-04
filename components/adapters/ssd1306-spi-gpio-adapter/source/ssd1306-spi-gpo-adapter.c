/**
 * Copyright (C) G2Labs Grzegorz Grzęda - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Grzegorz Grzęda <grzegorz.grzeda@g2labs.pl>, 03.2023
 */
#include "ssd1306-spi-gpo-adapter.h"
#include <stdlib.h>
#include <string.h>
#include "g2l-hal-gpio.h"
#include "g2l-log.h"
#define TAG "ssd1306-spi-gpo-adapter"

#define SSD1306_SPI_CLOCK_FREQUENCY (8000000)
#define SSD1306_SPI_MODE (PLATFORM_SPI_MODE_0)
#define SSD1306_SELECT_COMMAND (false)
#define SSD1306_SELECT_DATA (true)
#define SSD1306_DC_PIN_INIT_STATE (SSD1306_SELECT_COMMAND)
#define SSD1306_DC_PIN_PULL_UP (false)

typedef struct ssd1306_spi_gpo_adapter {
    g2l_hal_spi_device_t* spi_device;
    uint8_t dc_pin;
} ssd1306_spi_gpo_adapter_t;

static void reset(const generic_controller_t* controler) {}

static void send(const generic_controller_t* controler,
                 const uint8_t* buffer,
                 size_t size,
                 bool dc_state) {
    if (!controler || !controler->context) {
        return;
    }
    ssd1306_spi_gpo_adapter_t* ssd1306_spi_gpo_controler =
        (ssd1306_spi_gpo_adapter_t*)controler->context;
    g2l_hal_gpio_update_output(ssd1306_spi_gpo_controler->dc_pin, dc_state);
    g2l_hal_spi_transmit(ssd1306_spi_gpo_controler->spi_device, buffer, NULL,
                         size);
}

static void send_command(const generic_controller_t* controler,
                         const uint8_t* buffer,
                         size_t size) {
    send(controler, buffer, size, SSD1306_SELECT_COMMAND);
}

static void send_data(const generic_controller_t* controler,
                      const uint8_t* buffer,
                      size_t size) {
    send(controler, buffer, size, SSD1306_SELECT_DATA);
}

generic_controller_t* ssd1306_spi_gpo_controler_create(
    ssd1306_spi_gpo_adapter_configuration_t configuration) {
    ssd1306_spi_gpo_adapter_t* controler = calloc(1, sizeof(*controler));
    if (!controler || !configuration.spi) {
        return NULL;
    }
    generic_controller_t* gc = calloc(1, sizeof(*gc));
    if (!gc) {
        free(controler);
        return NULL;
    }
    g2l_hal_spi_device_configuration_t spi_device_configuration = {
        .clock_frequency_hz = SSD1306_SPI_CLOCK_FREQUENCY,
        .mode = SSD1306_SPI_MODE,
        .ncss_pin = configuration.ncss_pin,
    };
    controler->spi_device =
        g2l_hal_spi_add_device(configuration.spi, spi_device_configuration);
    controler->dc_pin = configuration.dc_pin;
    g2l_hal_gpio_configure_output(configuration.dc_pin,
                                  SSD1306_DC_PIN_INIT_STATE,
                                  SSD1306_DC_PIN_PULL_UP);

    gc->context = controler;
    gc->reset = reset;
    gc->send_command = send_command;
    gc->send_data = send_data;

    return gc;
}