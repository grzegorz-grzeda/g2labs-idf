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
#include "g2l-mutex.h"
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

typedef struct g2l_mutex {
    SemaphoreHandle_t handle;
} g2l_mutex_t;

g2l_mutex_t* g2l_mutex_create(void) {
    g2l_mutex_t* mtx = calloc(1, sizeof(g2l_mutex_t));
    if (mtx) {
        mtx->handle = xSemaphoreCreateMutex();
        if (mtx->handle == NULL) {
            free(mtx);
            mtx = NULL;
        }
    }
    return mtx;
}

void g2l_mutex_lock(g2l_mutex_t* mutex) {
    if (mutex && mutex->handle) {
        if (xPortInIsrContext()) {
            BaseType_t _dummy = pdFALSE;
            xSemaphoreTakeFromISR(mutex->handle, &_dummy);
        } else {
            xSemaphoreTake(mutex->handle, portMAX_DELAY);
        }
    }
}

void g2l_mutex_unlock(g2l_mutex_t* mutex) {
    if (mutex && mutex->handle) {
        if (xPortInIsrContext()) {
            BaseType_t _dummy = pdFALSE;
            xSemaphoreGiveFromISR(mutex->handle, &_dummy);
        } else {
            xSemaphoreGive(mutex->handle);
        }
    }
}

void g2l_mutex_destroy(g2l_mutex_t* mutex) {
    if (mutex) {
        if (mutex->handle) {
            vSemaphoreDelete(mutex->handle);
        }
        free(mutex);
    }
}
