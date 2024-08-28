#include "g2l-delay.h"
#include "g2l-gpio.h"
#include "g2l-log.h"
#include "g2l-uart.h"

#define STATUS_LED (2)

int main() {
    g2l_uart_init(9600);
    g2l_uart_register_stdout();

    g2l_gpio_configure_output(STATUS_LED, false, false);

    char message[] = "Hello, World!";
    I("main", "Sending: %s", message);
    while (1) {
        I("main", "Tick");
        g2l_delay_ms(1000);
        g2l_gpio_update_output(STATUS_LED, true);
        I("main", "Tock");
        g2l_delay_ms(1000);
        g2l_gpio_update_output(STATUS_LED, false);
    }
    return 0;
}