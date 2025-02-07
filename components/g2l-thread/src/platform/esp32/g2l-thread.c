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
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "g2l-thread.h"

typedef struct g2l_thread {
    TaskHandle_t handle;
    g2l_thread_handler_t func;
    void* arg;
} g2l_thread_t;

void g2l_os_thread_wrapper(void* arg) {
    g2l_thread_t* data = (g2l_thread_t*)arg;
    if (data) {
        data->func(data->arg);
        free(data);
    }
    vTaskDelete(NULL);
}

g2l_thread_t* g2l_thread_create(const char* name,
                                uint32_t stack_size,
                                g2l_thread_handler_t func,
                                void* arg) {
    if (!name || !func) {
        return NULL;
    }
    if (stack_size == G2L_THREAD_STACK_SIZE_DEFAULT) {
        stack_size = 4096;
    } else if (stack_size < 128) {
        stack_size = 128;
    } else {
        return NULL;
    }
    g2l_thread_t* data = (g2l_thread_t*)calloc(1, sizeof(g2l_thread_t));
    if (!data) {
        return NULL;
    }
    data->func = func;
    data->arg = arg;
    xTaskCreate(g2l_os_thread_wrapper, name, stack_size, data, 1,
                &data->handle);
    return data;
}