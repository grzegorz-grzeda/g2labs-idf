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
#ifndef G2L_TCP_H
#define G2L_TCP_H

#include <stddef.h>

typedef struct g2l_tcp g2l_tcp_t;

typedef struct g2l_tcp_connection g2l_tcp_connection_t;

g2l_tcp_t* g2l_tcp_create_server(int port, int max_connections_count);

void g2l_tcp_destroy(g2l_tcp_t* tcp);

g2l_tcp_connection_t* g2l_tcp_accept(g2l_tcp_t* tcp);

size_t g2l_tcp_read(g2l_tcp_connection_t* connection,
                    char* data,
                    size_t max_data_size);

size_t g2l_tcp_write(g2l_tcp_connection_t* connection,
                     const char* data,
                     size_t data_size);

void g2l_tcp_close(g2l_tcp_connection_t* connection);

#endif  // G2L_TCP_H