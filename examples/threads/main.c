#include <stddef.h>
#include "g2l-log.h"
#include "g2l-os-delay.h"
#include "g2l-os-thread.h"

#define TAG "threads"

void thread_1(void* arg) {
    while (1) {
        I(TAG, "Thread 1");
        g2l_os_delay_ms(1000);
    }
}

void thread_2(void* arg) {
    I(TAG, "Thread 2");
    g2l_os_delay_ms(500);
    I(TAG, "Thread 2 - exiting");
}

void thread_3(void* arg) {
    I(TAG, "Thread 3");
    g2l_os_delay_ms(250);
    I(TAG, "Thread 3 - exiting");
}

int main(int argc, char** argv) {
    g2l_os_thread_create("Thread 1", G2L_OS_THREAD_STACK_SIZE_DEFAULT, thread_1,
                         NULL);
    g2l_os_thread_create("Thread 2", G2L_OS_THREAD_STACK_SIZE_DEFAULT, thread_2,
                         NULL);
    g2l_os_thread_create("Thread 3", G2L_OS_THREAD_STACK_SIZE_DEFAULT, thread_3,
                         NULL);

    while (1) {
        I(TAG, "main");
        g2l_os_delay_ms(1000);
    }
    return 0;
}