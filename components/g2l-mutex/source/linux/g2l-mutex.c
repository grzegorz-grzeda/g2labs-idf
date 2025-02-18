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
#include <pthread.h>
#include <stdlib.h>

typedef struct g2l_mutex {
    pthread_mutex_t mutex;
} g2l_mutex_t;

g2l_mutex_t* g2l_mutex_create(void) {
    g2l_mutex_t* mtx = calloc(1, sizeof(g2l_mutex_t));
    if (mtx) {
        if (pthread_mutex_init(&mtx->mutex, NULL) != 0) {
            free(mtx);
            mtx = NULL;
        }
    }
    return mtx;
}

void g2l_mutex_destroy(g2l_mutex_t* mutex) {
    if (!mutex) {
        return;
    }
    pthread_mutex_destroy(&mutex->mutex);
    free(mutex);
}

void g2l_mutex_lock(g2l_mutex_t* mutex) {
    if (!mutex) {
        return;
    }
    pthread_mutex_lock(&mutex->mutex);
}

void g2l_mutex_unlock(g2l_mutex_t* mutex) {
    if (!mutex) {
        return;
    }
    pthread_mutex_unlock(&mutex->mutex);
}
