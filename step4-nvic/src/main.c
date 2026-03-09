#include "gpio.h"
#include "gpiote.h"
#include "nrf52832_irq.h"
#include "systick.h"

#define LED1 GPIO0_PIN(17)
#define BTN1 GPIO0_PIN(13)

volatile uint32_t cnt = 0;

void btn_cb(void) { cnt++; }

int main(void) {

    // LED setup
    gpio_dir_set(LED1, GPIO_OUTPUT);
    gpio_write(LED1, 1);

    // button setup
    gpio_config_set(
        BTN1,
        (gpio_cnf_t){
            .dir = GPIO_INPUT,
            .pull = GPIO_PULL_UP,
            .ibuff = GPIO_IBUFF_CONNECTED,
        }
    );
    
    // SysTick setup
    systick_init_1ms();
    systick_start(1);

    // GPIOTE setup
    gpiote_init(NRF52832_IRQ_PRIO_1);
    gpiote_configure_channel(
        (gpiote_config_t){
            .channel = GPIOTE_CHANNEL_0,
            .pin = 13,
            .mode = GPIOTE_MODE_EVENT,
            .polarity = GPIOTE_POLARITY_HITOLO,
        }
    );
    gpiote_set_callback(GPIOTE_CHANNEL_0, &btn_cb);
    gpiote_enable_interrupt(GPIOTE_CHANNEL_0);

    while (1) {
        if(cnt) {
            gpio_clr(LED1);
            delay_ms(1000);
            gpio_set(LED1);
            delay_ms(1000);
            cnt--;
        }
    }
}
