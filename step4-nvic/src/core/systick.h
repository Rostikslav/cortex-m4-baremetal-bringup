#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>
#include <stdbool.h>

void systick_init_1ms(void);
void systick_start(bool enable_irq);
void systick_stop(bool reset_ticks);
uint32_t systick_ticks_get(void);
void delay_ms(uint32_t ms);

#endif
