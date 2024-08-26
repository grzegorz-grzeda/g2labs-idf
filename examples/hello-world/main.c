#include "g2l-log.h"
#include "g2l-uart.h"

int main() {
    g2l_uart_init(9600);
    g2l_uart_register_stdout();

    char message[] = "Hello, World!";
    I("main", "Sending: %s", message);
    while (1)
        ;
    return 0;
}