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
#include "event-handler.h"
#include "simple-list.h"
#include <stddef.h>
#include <stdlib.h>

typedef struct event_handler_entry {
    void *context;
    event_handler_t handler;
    uint16_t id;
} event_handler_entry_t;

static simple_list_t *event_handlers;

void initialize_event_handler(void)
{
    event_handlers = create_simple_list();
}

bool register_event_handler(uint16_t id, void *context, event_handler_t handler)
{
    if (!handler) {
        return false;
    }
    event_handler_entry_t *entry = calloc(1, sizeof(event_handler_entry_t));
    if (!entry) {
        return false;
    }
    entry->context = context;
    entry->handler = handler;
    entry->id = id;
    append_to_simple_list(event_handlers, entry);
    return true;
}

static int event_handler_entry_filter(const void *current_entry, const void *event_id)
{
    uint16_t id = *(uint16_t *) event_id;
    event_handler_entry_t *entry = (event_handler_entry_t *) current_entry;
    return !(id == entry->id);
}

bool send_event_to_handlers(uint16_t id, void *payload)
{
    bool was_any_event_handler_executed = false;
    for (simple_list_iterator_t *it = simple_list_begin_filtered(event_handlers, &id, event_handler_entry_filter); it;
         it = simple_list_next_filtered(it, &id, event_handler_entry_filter)) {
        event_handler_entry_t *entry = get_from_simple_list_iterator(it);
        entry->handler(id, entry->context, payload);
        was_any_event_handler_executed = true;
    }
    return was_any_event_handler_executed;
}