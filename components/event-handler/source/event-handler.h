/*
 * MIT License
 * 
 * Copyright (c) 2023 Grzegorz Grzęda
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @defgroup event_handler Event Handler
 * @brief Simple and efficient C library for handling events
 * @{
*/

/**
 * @brief Event handler type
 * 
 * This is a callback pointer the the actual event handling code. 
 * @param[in] id event ID
 * @param[in] context pointer to some context passed during registration
 * @param[in] payload pointer to some payload associated with the event ID
*/
typedef void (*event_handler_t)(uint16_t id, void *context, void *payload);

/**
 * @brief Initialize the event handler
*/
void initialize_event_handler(void);

/**
 * @brief Register an new event handler
 *
 * @note This function is not thread safe! Consider registering all handlers in
 * the initialization part of your project.
 * 
 * @param[in] id event ID that we are registering for
 * @param[in] context pointer to some context for the event @ref handler
 * @param[in] handler pointer to the actual event handling callback
 * @return true if registration was successfull
 * @return false if handler was invalid or no more memory to store it
*/
bool register_event_handler(uint16_t id, void *context, event_handler_t handler);

/**
 * @brief Send event to handlers
 * 
 * It calls each handler registered for given event ID.
 * @note This code is platform-agnostic. If this function was called from e.g. 
 * - ISR
 * - another thread
 * It is the handlers job to cross thread/interrupt barriers correctly (e.g. through queues, semaphores etc.). 
 * The handlers are being called in the `send_event_to_handlers` caller context.
 * 
 * @param[in] id event ID to be sent
 * @param[in] payload pointer to possible payload associated with the event ID
 * @return true if at least one handler was executed for give event
 * @return false otherwise
*/
bool send_event_to_handlers(uint16_t id, void *payload);

/**
 * @}
*/

#endif // EVENT_HANDLER_H