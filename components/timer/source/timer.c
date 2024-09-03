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
#include "timer.h"
#include <stddef.h>
#include <string.h>

#ifndef TIMER_ENTRIES_MAX_COUNT
#error "TIMER_ENTRIES_MAX_COUNT not defined!"
#define TIMER_ENTRIES_MAX_COUNT (0)
#elif ((TIMER_ENTRIES_MAX_COUNT) < 1)
#error "TIMER_ENTRIES_MAX_COUNT is an array size and must be a positive number!"
#endif

typedef struct timer_instance {
    timer_instance_handler_t handler;
    void* handler_context;
    bool is_active;
    uint32_t current_value;
    uint32_t period_ms;
    uint32_t delay_ms;
} timer_instance_t;

static timer_instance_t timers[TIMER_ENTRIES_MAX_COUNT];
static size_t timers_count = 0;

static void process_timer(timer_instance_t* timer) {
    if (!timer->is_active) {
        return;
    }
    if (timer->current_value > 0) {
        timer->current_value--;
        return;
    }
    if (timer->handler) {
        timer->handler(timer, timer->handler_context);
    }
    if (!timer->period_ms) {
        timer->is_active = false;
    } else {
        timer->current_value = (timer->period_ms - 1);
    }
}

static void process_timers_upon_ms_tick(void) {
    for (size_t i = 0; i < timers_count; i++) {
        process_timer(timers + i);
    }
}

timer_tick_ms_handler_t timer_initialize(void) {
    timers_count = 0;
    memset(timers, 0, sizeof(timers));
    return process_timers_upon_ms_tick;
}

timer_instance_t* timer_register(uint32_t period_ms,
                                 uint32_t delay_ms,
                                 timer_instance_handler_t handler,
                                 void* context) {
    if (timers_count == (TIMER_ENTRIES_MAX_COUNT - 1) || !handler || ((period_ms == 0) && (delay_ms == 0))) {
        return NULL;
    }
    timer_instance_t* timer = timers + timers_count;
    timers_count++;
    timer->is_active = false;
    timer->period_ms = period_ms;
    timer->delay_ms = delay_ms;
    timer->handler = handler;
    timer->handler_context = context;
    return timer;
}

bool timer_start(timer_instance_t* timer) {
    if (!timer || timer->is_active) {
        return false;
    }
    timer->current_value = timer->delay_ms;
    timer->is_active = true;
    return true;
}

bool timer_stop(timer_instance_t* timer) {
    if (!timer) {
        return false;
    }
    timer->is_active = false;
    return true;
}

bool timer_restart(timer_instance_t* timer) {
    timer_stop(timer);
    return timer_start(timer);
}
