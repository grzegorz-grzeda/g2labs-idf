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
#ifndef ENCODINGS_BASE64_H
#define ENCODINGS_BASE64_H

#include <stddef.h>

/**
 * @defgroup encodings-base64 Encodings Base64
 * @brief Base64 encoding procedures
 * @{
 */

/**
 * @brief Get the required size of destination buffer for Base64 decoded content
 * @param[in] base64 pointer to Base64 C-string
 * @return size of the buffer needed to hold the decoded value
 */
size_t encodings_base64_get_decode_buffer_size(const char* base64);

/**
 * @brief Decode Base64 input stream
 * @param[in] base64 pointer to Base64 input C-string
 * @param[out] decoded pointer to destination buffer for decoded data
 * @note The decoded destination buffer needs to be able to hold all decoded
 * data!
 */
void encodings_base64_decode(const char* base64, char* decoded);

/**
 * @}
 */

#endif  // ENCODINGS_BASE64_H