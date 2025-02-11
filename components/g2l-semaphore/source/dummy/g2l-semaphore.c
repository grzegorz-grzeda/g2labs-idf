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
#include <stddef.h>
#include "g2l-log.h"

#define TAG "g2l-semaphore"

typedef struct g2l_semaphore g2l_semaphore_t;

g2l_semaphore_t* g2l_semaphore_create(void) {
    E(TAG, "g2l_semaphore_create - Not implemented");
    return NULL;
}

void g2l_semaphore_destroy(g2l_semaphore_t* semaphore) {
    E(TAG, "g2l_semaphore_destroy - Not implemented");
}

void g2l_semaphore_wait(g2l_semaphore_t* semaphore) {
    E(TAG, "g2l_semaphore_wait - Not implemented");
}

void g2l_semaphore_post(g2l_semaphore_t* semaphore) {
    E(TAG, "g2l_semaphore_post - Not implemented");
}
