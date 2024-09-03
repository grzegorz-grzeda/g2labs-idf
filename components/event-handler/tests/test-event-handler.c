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
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>

#include "event-handler.h"

static void test_event_handler(uint16_t id, void *context, void *payload)
{
    function_called();
    check_expected(id);
    check_expected_ptr(context);
    check_expected(payload);
}

static void test_another_event_handler(uint16_t id, void *context, void *payload)
{
    function_called();
    check_expected(id);
    check_expected_ptr(context);
    check_expected(payload);
}

static void register_invalid_handler(void **state)
{
    assert_false(register_event_handler(0, NULL, NULL));
}

static void send_event_no_handlers_registered(void **state)
{
    const uint16_t event_id = 22;
    assert_false(send_event_to_handlers(event_id, NULL));
}

static void send_event_other_handler_registered_test(void **state)
{
    const uint16_t registered_event_id = 22;
    const uint16_t event_id = 10;

    assert_false(send_event_to_handlers(event_id, NULL));
    assert_true(register_event_handler(registered_event_id, NULL, test_event_handler));
    assert_false(send_event_to_handlers(event_id, NULL));
}

static void send_event_handler_registered_test(void **state)
{
    const uint16_t event_id = 10;

    assert_false(send_event_to_handlers(event_id, NULL));
    assert_true(register_event_handler(event_id, NULL, test_event_handler));
    expect_function_call(test_event_handler);
    expect_value(test_event_handler, id, event_id);
    expect_value(test_event_handler, context, NULL);
    expect_value(test_event_handler, payload, NULL);
    assert_true(send_event_to_handlers(event_id, NULL));
}

static void send_event_handler_with_context_test(void **state)
{
    const uint16_t event_id = 200;
    uint8_t context[100];

    assert_true(register_event_handler(event_id, context, test_event_handler));
    expect_function_call(test_event_handler);
    expect_value(test_event_handler, id, event_id);
    expect_value(test_event_handler, context, context);
    expect_value(test_event_handler, payload, NULL);
    assert_true(send_event_to_handlers(event_id, NULL));
}

static void send_event_handler_with_payload_test(void **state)
{
    const uint16_t event_id = 200;
    uint8_t payload[100];

    assert_true(register_event_handler(event_id, NULL, test_event_handler));
    expect_function_call(test_event_handler);
    expect_value(test_event_handler, id, event_id);
    expect_value(test_event_handler, context, NULL);
    expect_value(test_event_handler, payload, payload);
    assert_true(send_event_to_handlers(event_id, payload));
}

static void send_event_handler_with_context_and_payload_test(void **state)
{
    const uint16_t event_id = 200;
    uint8_t context[50];
    uint8_t payload[100];

    assert_true(register_event_handler(event_id, context, test_event_handler));
    expect_function_call(test_event_handler);
    expect_value(test_event_handler, id, event_id);
    expect_value(test_event_handler, context, context);
    expect_value(test_event_handler, payload, payload);
    assert_true(send_event_to_handlers(event_id, payload));
}

static void send_event_multiple_event_handlers_test(void **state)
{
    const uint16_t event_id_0 = 200;
    const uint16_t event_id_1 = 22;
    uint8_t context[50];
    uint8_t payload_0[100];

    assert_true(register_event_handler(event_id_0, context, test_event_handler));
    assert_true(register_event_handler(event_id_1, NULL, test_another_event_handler));

    expect_function_call(test_event_handler);
    expect_value(test_event_handler, id, event_id_0);
    expect_value(test_event_handler, context, context);
    expect_value(test_event_handler, payload, payload_0);
    assert_true(send_event_to_handlers(event_id_0, payload_0));

    expect_function_call(test_another_event_handler);
    expect_value(test_another_event_handler, id, event_id_1);
    expect_value(test_another_event_handler, context, NULL);
    expect_value(test_another_event_handler, payload, NULL);
    assert_true(send_event_to_handlers(event_id_1, NULL));
}

static void register_one_handler_to_many_event_ids_test(void **state)
{
    const uint16_t max_count = 20;

    for (uint16_t i = 0; i < max_count; i++) {
        assert_true(register_event_handler(i, NULL, test_event_handler));
    }

    for (uint16_t i = 0; i < max_count; i++) {
        uint8_t payload[2];
        expect_function_call(test_event_handler);
        expect_value(test_event_handler, id, i);
        expect_value(test_event_handler, context, NULL);
        expect_value(test_event_handler, payload, payload);
        assert_true(send_event_to_handlers(i, payload));
    }
}

static void register_many_handlers_to_one_event_id_test(void **state)
{
    const uint16_t event_id = 1;
    uint8_t context_0[2];
    uint8_t payload[3];

    assert_true(register_event_handler(event_id, context_0, test_event_handler));
    assert_true(register_event_handler(event_id, NULL, test_another_event_handler));

    expect_function_call(test_event_handler);
    expect_value(test_event_handler, id, event_id);
    expect_value(test_event_handler, context, context_0);
    expect_value(test_event_handler, payload, payload);
    expect_function_call(test_another_event_handler);
    expect_value(test_another_event_handler, id, event_id);
    expect_value(test_another_event_handler, context, NULL);
    expect_value(test_another_event_handler, payload, payload);
    assert_true(send_event_to_handlers(event_id, payload));
}

static int test_set_up(void **state)
{
    initialize_event_handler();
    return 0;
}

int main(int argc, char **argv)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup(register_invalid_handler, test_set_up),
        cmocka_unit_test_setup(send_event_no_handlers_registered, test_set_up),
        cmocka_unit_test_setup(send_event_other_handler_registered_test, test_set_up),
        cmocka_unit_test_setup(send_event_handler_registered_test, test_set_up),
        cmocka_unit_test_setup(send_event_handler_with_context_test, test_set_up),
        cmocka_unit_test_setup(send_event_handler_with_payload_test, test_set_up),
        cmocka_unit_test_setup(send_event_handler_with_context_and_payload_test, test_set_up),
        cmocka_unit_test_setup(send_event_multiple_event_handlers_test, test_set_up),
        cmocka_unit_test_setup(register_one_handler_to_many_event_ids_test, test_set_up),
        cmocka_unit_test_setup(register_many_handlers_to_one_event_id_test, test_set_up),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}