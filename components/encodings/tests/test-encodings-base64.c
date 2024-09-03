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
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "cmocka.h"

#include "encodings-base64.h"

static void test_calculate_decode_buffer_length(void** state) {
    size_t size = encodings_base64_get_decode_buffer_size("AA==");  // "\0"
    assert_int_equal(size, 1);
    size = encodings_base64_get_decode_buffer_size("QQ==");  // "A"
    assert_int_equal(size, 1);

    size = encodings_base64_get_decode_buffer_size("aGVsbG8=");  // "hello"
    assert_int_equal(size, 5);

    size = encodings_base64_get_decode_buffer_size(
        "TG9yZW0gaXBzdW0gZG9sb3Igc2l0IGFtZXQ=");  // "Lorem ipsum dolor sit
                                                  // amet"
    assert_int_equal(size, 26);
}

static void test_decode_text(void** state) {
    const char* encoded = "QQ==";  // "A"
    char* decoded = calloc(encodings_base64_get_decode_buffer_size(encoded) + 1,
                           sizeof(char));
    encodings_base64_decode(encoded, decoded);
    assert_string_equal(decoded, "A");
    free(decoded);

    encoded = "aGVsbG8=";  // "hello"
    decoded = calloc(encodings_base64_get_decode_buffer_size(encoded) + 1,
                     sizeof(char));
    encodings_base64_decode(encoded, decoded);
    assert_string_equal(decoded, "hello");
    free(decoded);

    encoded = "TG9yZW0gaXBzdW0gZG9sb3Igc2l0IGFtZXQ=";  // "Lorem ipsum dolor sit
                                                       // amet"
    decoded = calloc(encodings_base64_get_decode_buffer_size(encoded) + 1,
                     sizeof(char));
    encodings_base64_decode(encoded, decoded);
    assert_string_equal(decoded, "Lorem ipsum dolor sit amet");
    free(decoded);
}

int main(int argc, char** argv) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_calculate_decode_buffer_length),
        cmocka_unit_test(test_decode_text),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}