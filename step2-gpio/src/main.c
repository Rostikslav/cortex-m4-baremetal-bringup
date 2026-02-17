#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"

#define BTN1 GPIO0_PIN(13)

#define LED1 GPIO0_PIN(17)
#define LED2 GPIO0_PIN(18)
#define LED3 GPIO0_PIN(19)
#define LED4 GPIO0_PIN(20)

int main(void) {

	gpio_dir_set(LED1, GPIO_OUTPUT);
	gpio_dir_set(LED2, GPIO_OUTPUT);
	gpio_dir_set(LED3, GPIO_OUTPUT);

	gpio_config_set(
		BTN1,
		(gpio_cnf_t){
			.dir=GPIO_INPUT,
			.pull=GPIO_PULL_UP,
			.ibuff=GPIO_IBUFF_CONNECTED,
		}
	);

	// readback check
	gpio_sense_set(BTN1, GPIO_SENSE_HIGH);
	gpio_write(LED1, !(gpio_sense_get(BTN1) == GPIO_SENSE_HIGH));
	gpio_sense_set(BTN1, GPIO_SENSE_OFF);
	gpio_write(LED2, !(gpio_sense_get(BTN1) == GPIO_SENSE_OFF));

	while(1) {
		gpio_write(LED3, gpio_read_level(BTN1));
	}
}
