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
#include "g2l-os-mutex.h"
#include <stdlib.h>
#include "g2l-os-queue.h"

typedef struct g2l_os_mutex {
    g2l_os_queue_t* queue;
} g2l_os_mutex_t;

g2l_os_mutex_t* g2l_os_mutex_create(void) {
    g2l_os_mutex_t* mtx = calloc(1, sizeof(g2l_os_mutex_t));
    if (mtx) {
        mtx->queue = g2l_os_queue_create(1, sizeof(int));
    }
    return mtx;
}

void g2l_os_mutex_lock(g2l_os_mutex_t* mutex) {
    if (!mutex) {
        return;
    }
    int data = 0;
    g2l_os_queue_send(mutex->queue, &data);
}

void g2l_os_mutex_unlock(g2l_os_mutex_t* mutex) {
    if (!mutex) {
        return;
    }
    int data = 0;
    g2l_os_queue_receive(mutex->queue, &data);
}
