/**
 * MIT License
 * Copyright (c) 2023 Grzegorz Grzęda
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
#ifndef G2L_HAL_SPI_H
#define G2L_HAL_SPI_H

#include <stddef.h>
#include <stdint.h>

typedef struct g2l_hal_spi g2l_hal_spi_t;

typedef struct g2l_hal_spi_device g2l_hal_spi_device_t;

typedef enum g2l_hal_spi_mode {
    PLATFORM_SPI_MODE_0 = 0,
    PLATFORM_SPI_MODE_1,
    PLATFORM_SPI_MODE_2,
    PLATFORM_SPI_MODE_3,
} g2l_hal_spi_mode_t;

typedef struct g2l_hal_spi_configuration {
    uint8_t host_id;
    uint8_t mosi_pin;
    uint8_t miso_pin;
    uint8_t sclk_pin;
} g2l_hal_spi_configuration_t;

typedef struct g2l_hal_spi_device_configuration {
    uint32_t clock_frequency_hz;
    g2l_hal_spi_mode_t mode;
    uint8_t ncss_pin;
} g2l_hal_spi_device_configuration_t;

uint8_t g2l_hal_spi_get_host_count(void);

g2l_hal_spi_t* g2l_hal_spi_initialize(
    g2l_hal_spi_configuration_t configuration);

g2l_hal_spi_device_t* g2l_hal_spi_add_device(
    g2l_hal_spi_t* spi,
    g2l_hal_spi_device_configuration_t configuration);

size_t g2l_hal_spi_transmit(g2l_hal_spi_device_t* device,
                            const char* tx_buffer,
                            char* rx_buffer,
                            size_t size);

#endif  // G2L_HAL_SPI_H