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
#ifndef G2L_IDF_GPIO_H
#define G2L_IDF_GPIO_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @defgroup g2l-gpio Platform GPIO
 * @brief Platform specific code for handling general purpose input and output
 * pins.
 * @{
 */

/**
 * @brief Input edge detection configuration
 */
typedef enum g2l_gpio_edge_detect {
    PLATFORM_GPIO_INPUT_EDGE_DETECT_RISING,  /**< Detect input rising state. */
    PLATFORM_GPIO_INPUT_EDGE_DETECT_FALLING, /**< Detect input falling state. */
    PLATFORM_GPIO_INPUT_EDGE_DETECT_ANY,    /**< Detect either rising or falling
                                               input state. */
    _PLATFORM_GPIO_INPUT_EDGE_DETECT_COUNT, /**< Edge detection options count.
                                             */
} g2l_gpio_input_edge_detect_t;

/**
 * @brief GPIO input handler type
 *
 * This is a callback pointer.
 * @param[in] number Number of input pin, on which the registered event occoured
 */
typedef void (*g2l_gpio_input_handler_t)(uint8_t number);

/**
 * @brief Initialize g2l general purpose I/O
 *
 * Initializes the internals of the g2l GPIO code.
 * @param[in] input_handler pointer to the GPIO input handler.
 */
bool g2l_gpio_initialize(g2l_gpio_input_handler_t input_handler);

/**
 * @brief Configure g2l pin as general purpose input
 *
 * Configures the specified pin number as an input pin.
 * @param[in] number pin number to configure
 * @param[in] is_pull_up configure [or not] any pull up resistor to the pin
 * @param[in] edge_detect specify the edge detection configuration to respond to
 */
bool g2l_gpio_configure_input(uint8_t number,
                              bool is_pull_up,
                              g2l_gpio_input_edge_detect_t edge_detect);

/**
 * @brief Configure g2l pin as general purpose output
 *
 * Configures the specified pin number as an output pin.
 * @param[in] number pin number to configure
 * @param[in] init_state initial state to apply to output pin
 * @param[in] is_pull_up configure [or not] any pull up resistor to the pin
 */
bool g2l_gpio_configure_output(uint8_t number,
                               bool init_state,
                               bool is_pull_up);

/**
 * @brief Update g2l general purpose output
 *
 * Sets the specified output pin to the desired state.
 * @note The pin needs to be first initialized as output.
 * @param[in] number output pin number to update
 * @param[in] new_state new state to apply to output pin
 * @return @ref true pin state update was successfull
 */
bool g2l_gpio_update_output(uint8_t number, bool new_state);

/**
 * @}
 */

#endif  // G2L_IDF_GPIO_H