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
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "g2l-os-queue.h"

typedef struct g2l_os_queue {
    QueueHandle_t queue;
} g2l_os_queue_t;

g2l_os_queue_t* g2l_os_queue_create(size_t size, size_t item_size) {
    g2l_os_queue_t* queue = calloc(1, sizeof(g2l_os_queue_t));
    if (queue) {
        queue->queue = xQueueCreate(size, item_size);
    }
    return queue;
}

void g2l_os_queue_destroy(g2l_os_queue_t* queue) {
    if (queue) {
        vQueueDelete(queue->queue);
        free(queue);
    }
}

void g2l_os_queue_send(g2l_os_queue_t* queue, const void* item) {
    if (!queue) {
        return;
    }
    if (xPortInIsrContext()) {
        BaseType_t _dummy = pdFALSE;
        xQueueSendFromISR(queue->queue, item, &_dummy);
    } else {
        xQueueSend(queue->queue, item, portMAX_DELAY);
    }
}

void g2l_os_queue_receive(g2l_os_queue_t* queue, void* item) {
    if (!queue) {
        return;
    }
    if (xPortInIsrContext()) {
        BaseType_t _dummy = pdFALSE;
        xQueueReceiveFromISR(queue->queue, item, &_dummy);
    } else {
        xQueueReceive(queue->queue, item, portMAX_DELAY);
    }
}
