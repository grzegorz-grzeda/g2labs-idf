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
#include "g2l-tcp.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include "g2l-log.h"

#define TAG "g2l-tcp"

typedef struct g2l_tcp {
    int socket_fd;
} g2l_tcp_t;

typedef struct g2l_tcp_connection {
    int id;
} g2l_tcp_connection_t;

g2l_tcp_t* g2l_tcp_create_server(int port, int max_connections_count) {
    if (port < 0 || max_connections_count <= 0) {
        E(TAG, "Invalid port or max_connections_count");
        return NULL;
    }
    g2l_tcp_t* tcp = calloc(1, sizeof(g2l_tcp_t));
    if (!tcp) {
        E(TAG, "Failed to allocate memory for g2l_tcp_t");
        return NULL;
    }
    tcp->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp->socket_fd < 0) {
        E(TAG, "Failed to create socket");
        free(tcp);
        return NULL;
    }
    int true_value = 1;
    if (setsockopt(tcp->socket_fd, SOL_SOCKET, SO_REUSEADDR, &true_value,
                   sizeof(int)) < 0) {
        E(TAG, "Failed to set socket options");
        close(tcp->socket_fd);
        free(tcp);
        return NULL;
    }

    struct sockaddr_in tcp_addr;
    memset(&tcp_addr, '0', sizeof(tcp_addr));
    tcp_addr.sin_family = AF_INET;
    tcp_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    tcp_addr.sin_port = htons(port);

    if (bind(tcp->socket_fd, (struct sockaddr*)&tcp_addr, sizeof(tcp_addr)) <
        0) {
        E(TAG, "Failed to bind socket");
        close(tcp->socket_fd);
        free(tcp);
        return NULL;
    }

    if (listen(tcp->socket_fd, max_connections_count) < 0) {
        E(TAG, "Failed to listen on socket");
        close(tcp->socket_fd);
        free(tcp);
        return NULL;
    }
    return tcp;
}

void g2l_tcp_destroy(g2l_tcp_t* tcp) {
    if (!tcp) {
        return;
    }
    close(tcp->socket_fd);
    free(tcp);
}

g2l_tcp_connection_t* g2l_tcp_accept(g2l_tcp_t* tcp) {
    if (!tcp) {
        E(TAG, "Invalid tcp");
        return NULL;
    }
    g2l_tcp_connection_t* connection = calloc(1, sizeof(g2l_tcp_connection_t));
    if (!connection) {
        E(TAG, "Failed to allocate memory for g2l_tcp_connection_t");
        return NULL;
    }
    connection->id = accept(tcp->socket_fd, (struct sockaddr*)NULL, NULL);
    if (connection->id < 0) {
        E(TAG, "Failed to accept connection");
        free(connection);
        return NULL;
    }
    return connection;
}

size_t g2l_tcp_read(g2l_tcp_connection_t* connection,
                    char* data,
                    size_t max_data_size) {
    if (!connection || (connection->id < 0) || !data || (max_data_size < 1)) {
        E(TAG, "Invalid connection or data");
        return 0;
    }
    ssize_t read_bytes = read(connection->id, data, max_data_size);
    if (read_bytes < 0) {
        E(TAG, "Failed to read data from connection");
        return 0;
    }
    return (size_t)read_bytes;
}

size_t g2l_tcp_write(g2l_tcp_connection_t* connection,
                     const char* data,
                     size_t data_size) {
    if (!connection || (connection->id < 0) || !data || (data_size < 1)) {
        E(TAG, "Invalid connection or data");
        return 0;
    }
    ssize_t write_size = write(connection->id, data, data_size);
    if (write_size < 0) {
        E(TAG, "Failed to write data to connection");
        return 0;
    }
    return (size_t)write_size;
}

void g2l_tcp_close(g2l_tcp_connection_t* connection) {
    if (!connection || (connection->id < 0)) {
        E(TAG, "Invalid connection");
        return;
    }
    close(connection->id);
    free(connection);
}