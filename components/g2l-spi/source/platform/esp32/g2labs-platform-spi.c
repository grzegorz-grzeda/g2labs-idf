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
#include <stdlib.h>
#include "driver/spi_master.h"
#include "g2l-log.h"
#include "g2l-spi.h"

#define TAG "G2L-SPI"

typedef struct g2l_spi {
    uint8_t host_id;
    spi_bus_config_t buscfg;
} g2l_spi_t;

typedef struct g2l_spi_device {
    g2l_spi_t* spi;
    spi_device_handle_t device;
    spi_device_interface_config_t devcfg;
} g2l_spi_device_t;

uint8_t g2l_spi_get_host_count(void) {
    return 2;
}

g2l_spi_t* g2l_spi_initialize(g2l_spi_configuration_t configuration) {
    g2l_spi_t* spi = calloc(1, sizeof(*spi));
    if (!spi) {
        return NULL;
    }
    spi_bus_config_t buscfg = {
        .miso_io_num = configuration.miso_pin,
        .mosi_io_num = configuration.mosi_pin,
        .sclk_io_num = configuration.sclk_pin,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 0,
    };
    spi->host_id = configuration.host_id;
    spi->buscfg = buscfg;
    esp_err_t ret = spi_bus_initialize(configuration.host_id, &spi->buscfg,
                                       SPI_DMA_CH_AUTO);
    ESP_ERROR_CHECK(ret);
    D(TAG, "Initialized SPI for host %u", configuration.host_id);
    return spi;
}

g2l_spi_device_t* g2l_spi_add_device(
    g2l_spi_t* spi,
    g2l_spi_device_configuration_t configuration) {
    if (!spi) {
        return NULL;
    }
    g2l_spi_device_t* device = calloc(1, sizeof(*device));
    if (!device) {
        return NULL;
    }
    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = configuration.clock_frequency_hz,
        .mode = configuration.mode,
        .spics_io_num = configuration.ncss_pin,
        .queue_size = 1,
    };
    device->spi = spi;
    device->devcfg = devcfg;
    esp_err_t ret = spi_bus_add_device(device->spi->host_id, &device->devcfg,
                                       &device->device);
    D(TAG, "[NCSS:%u]Added device mode %u for host %u", configuration.ncss_pin,
      configuration.mode, spi->host_id);
    ESP_ERROR_CHECK(ret);
    return device;
}

size_t g2l_spi_transmit(g2l_spi_device_t* device,
                        const char* tx_buffer,
                        char* rx_buffer,
                        size_t size) {
    if (!device || !tx_buffer || !size) {
        return 0;
    }
    size_t rxsize = rx_buffer ? size : 0;
    spi_transaction_t transaction = {
        .tx_buffer = tx_buffer,
        .rx_buffer = rx_buffer,
        .length = size * 8,
        .rxlength = rxsize * 8,
    };
    esp_err_t ret = spi_device_polling_transmit(device->device, &transaction);
    D(TAG, "[NCSS:%u] Sent %lu, received %lu bytes",
      device->devcfg.spics_io_num, size, rxsize);
    ESP_ERROR_CHECK(ret);
    return size;
}
