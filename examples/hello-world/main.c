#include "color-manipulation.h"
#include "g2l-delay.h"
#include "g2l-hal-gpio.h"
#include "g2l-hal-ws28xx.h"
#include "g2l-log.h"
#include "g2l-uart.h"

#define TAG "hello-world"

#define STATUS_LED (2)
#define WS28XX_LED_PIN (16)
#define WS28XX_LED_COUNT (16)
#define WS28XX_LED_SATURATION (100)
#define WS28XX_LED_VALUE (20)

static void led_animate(void) {
    static uint16_t hue = 0;
    for (size_t i = 0; i < WS28XX_LED_COUNT; i++) {
        uint16_t local_hue = hue + ((360 * i) / WS28XX_LED_COUNT);
        color_hsv_t hsv = {
            .hue = local_hue,
            .saturation = WS28XX_LED_SATURATION,
            .value = WS28XX_LED_VALUE,
        };
        color_rgb_t rgb = convert_color_hsv_to_rgb(hsv);
        g2l_hal_ws28xx_set_led_color(i, rgb.red, rgb.green, rgb.blue);
    }
    hue += 1;
    hue %= 360;
}

int main(int argc, char** argv) {
    g2l_uart_init(9600);
    g2l_uart_register_stdout();

    g2l_hal_ws28xx_initialize(WS28XX_LED_PIN, WS28XX_LED_COUNT);

    bool state = false;
    g2l_hal_gpio_configure_output(STATUS_LED, state, false);

    char message[] = "Hello, World!";
    I(TAG, "Sending: %s", message);
    for (int i = 0; i < argc; i++) {
        I(TAG, "Arg %d: %s", i, argv[i]);
    }
    while (1) {
        state = !state;
        g2l_hal_gpio_update_output(STATUS_LED, state);
        led_animate();
        g2l_delay_ms(20);
    }
    return 0;
}