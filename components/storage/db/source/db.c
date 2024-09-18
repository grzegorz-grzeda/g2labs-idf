/*
 * MIT License
 *
 * Copyright (c) 2023 G2Labs Grzegorz Grzęda
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
#include "db.h"
#include "g2l-os-fs.h"

#include <string.h>

#define MAX_NAMESPACE_KEY_LENGTH (60)

typedef char namespace_key[MAX_NAMESPACE_KEY_LENGTH];

static void create_namespace_key(namespace_key nk,
                                 const char* namespace,
                                 const char* key) {
    char* result = (char*)nk;
    strcpy(result, "db-");
    strcat(result, namespace);
    strcat(result, "-");
    strcat(result, key);
    strcat(result, ".txt");
}

void db_init(void) {
    g2l_os_fs_initialize();
}

size_t db_set(const char* namespace,
              const char* key,
              const void* value,
              size_t value_size) {
    namespace_key nk;
    create_namespace_key(nk, namespace, key);
    return g2l_os_fs_store_file(nk, value, value_size);
}

size_t db_does_exist(const char* namespace, const char* key) {
    namespace_key nk;
    create_namespace_key(nk, namespace, key);
    return g2l_os_fs_get_size_of_file(nk);
}

size_t db_get(const char* namespace,
              const char* key,
              void* value,
              size_t max_value_size) {
    namespace_key nk;
    create_namespace_key(nk, namespace, key);
    return g2l_os_fs_load_file(nk, value, max_value_size);
}