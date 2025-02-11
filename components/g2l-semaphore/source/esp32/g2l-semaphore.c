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
#include "g2l-semaphore.h"
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

typedef struct g2l_semaphore {
    SemaphoreHandle_t handle;
} g2l_semaphore_t;

g2l_semaphore_t* g2l_semaphore_create(void) {
    g2l_semaphore_t* mtx = calloc(1, sizeof(g2l_semaphore_t));
    if (!mtx) {
        return NULL;
    }
    mtx->handle = xSemaphoreCreateBinary();
    if (mtx->handle == NULL) {
        free(mtx);
        return NULL;
    }
    return mtx;
}

void g2l_mutex_destroy(g2l_semaphore_t* semaphore) {
    if (!semaphore) {
        return;
    }
    if (semaphore->handle) {
        vSemaphoreDelete(semaphore->handle);
    }
    free(semaphore);
}

void g2l_semaphore_wait(g2l_semaphore_t* semaphore) {
    if (!semaphore || !semaphore->handle) {
        return;
    }
    if (xPortInIsrContext()) {
        BaseType_t _dummy = pdFALSE;
        xSemaphoreTakeFromISR(semaphore->handle, &_dummy);
    } else {
        xSemaphoreTake(semaphore->handle, portMAX_DELAY);
    }
}

void g2l_semaphore_post(g2l_semaphore_t* semaphore) {
    if (!semaphore || !semaphore->handle) {
        return;
    }
    if (xPortInIsrContext()) {
        BaseType_t _dummy = pdFALSE;
        xSemaphoreGiveFromISR(semaphore->handle, &_dummy);
    } else {
        xSemaphoreGive(semaphore->handle);
    }
}
