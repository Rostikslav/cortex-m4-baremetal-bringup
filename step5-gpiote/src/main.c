#include "gpiote.h"
#include "nrf52832_irq.h"
#include "gpio.h"

#define LED1 GPIO0_PIN(17)
#define BTN1 GPIO0_PIN(13)
#define BTN2 GPIO0_PIN(14)

void cb(void) {
    gpio_write(LED1, !gpio_read_output(LED1));
}

int main(void) {
    
    // LED setup
    gpio_set(LED1);
    gpio_dir_set(LED1, GPIO_OUTPUT);

    // GPIOTE setup
    gpio_config_set(BTN1, (gpio_cnf_t) {
            .dir = GPIO_INPUT,
            .ibuff = GPIO_IBUFF_CONNECTED,
            .pull = GPIO_PULL_UP,
            .sense = GPIO_SENSE_LOW,
    });

    gpio_config_set(BTN2, (gpio_cnf_t) {
            .dir = GPIO_INPUT,
            .ibuff = GPIO_IBUFF_CONNECTED,
            .pull = GPIO_PULL_UP,
            .sense = GPIO_SENSE_LOW
    });

    gpiote_init(NRF52832_IRQ_PRIO_1);
    gpiote_set_port_callback(&cb);
    gpiote_enable_port_interrupt();

    while(1);
}
