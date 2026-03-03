#include "gpio.h"
#include "systick.h"
#include "irq.h"

#define LED1 GPIO0_PIN(17)

int main(void) {

    gpio_dir_set(LED1, GPIO_OUTPUT);

    systick_init_1ms();
    irq_enable();
    systick_start(true);

    while(1) {
        gpio_write(LED1, !gpio_read_output(LED1));
        delay_ms(1000);
    }
}
