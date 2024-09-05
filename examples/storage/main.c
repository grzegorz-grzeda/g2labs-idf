#include <stdio.h>
#include <string.h>
#include "g2l-os-delay.h"
#include "g2l-os-fs.h"

int main(int argc, char** argv) {
    printf("Hello, world from storage example!\n");
    g2l_os_fs_initialize();

    size_t file_size = g2l_os_fs_get_size_of_file("test.txt");
    printf("File size: %d\n", file_size);

    if (file_size == 0) {
        printf("Storing file...\n");
        const char* data = "Hello, world!";
        g2l_os_fs_store_file("test.txt", data, strlen(data));
    }

    char buffer[256];
    size_t read_size = g2l_os_fs_load_file("test.txt", buffer, sizeof(buffer));
    printf("Read size: %d\n", read_size);
    printf("Read data: %.*s\n", read_size, buffer);

    while (1) {
        g2l_os_delay_ms(100);
    }
    return 0;
}