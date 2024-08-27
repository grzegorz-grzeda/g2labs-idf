#include "g2l-delay.h"
#include "g2l-log.h"
#include "g2l-uart.h"

int main() {
    g2l_uart_init(9600);
    g2l_uart_register_stdout();

    char message[] = "Hello, World!";
    I("main", "Sending: %s", message);
    while (1) {
        I("main", "Tick");
        g2l_delay_ms(1000);
        I("main", "Tock");
        g2l_delay_ms(1000);
    }
    return 0;
}