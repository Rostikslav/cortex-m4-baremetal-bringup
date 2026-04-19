#ifndef GPIOTE_H
#define GPIOTE_H

#include "nrf52832_gpiote_map.h"
#include "nrf52832_irq.h"
#include "mmio.h"
#include <stdint.h>

typedef void(*gpiote_channel_cb_t)(void);
typedef void(*gpiote_port_cb_t)(void);

typedef enum {
    GPIOTE_MODE_DISABLED = GPIOTE_CONFIG_MODE_DISABLED,
    GPIOTE_MODE_EVENT    = GPIOTE_CONFIG_MODE_EVENT,
    GPIOTE_MODE_TASK     = GPIOTE_CONFIG_MODE_TASK,
} gpiote_mode_t;

typedef enum {
    GPIOTE_POLARITY_NONE   = GPIOTE_CONFIG_POLARITY_NONE,
    GPIOTE_POLARITY_LOTOHI = GPIOTE_CONFIG_POLARITY_LOTOHI,
    GPIOTE_POLARITY_HITOLO = GPIOTE_CONFIG_POLARITY_HITOLO,
    GPIOTE_POLARITY_TOGGLE = GPIOTE_CONFIG_POLARITY_TOGGLE,
} gpiote_polarity_t;

typedef enum {
    GPIOTE_OUTINIT_LOW  = GPIOTE_CONFIG_OUTINIT_LOW,
    GPIOTE_OUTINIT_HIGH = GPIOTE_CONFIG_OUTINIT_HIGH,
} gpiote_outinit_t;

typedef enum {
    GPIOTE_CHANNEL_0 = 0,
    GPIOTE_CHANNEL_1,
    GPIOTE_CHANNEL_2,
    GPIOTE_CHANNEL_3,
    GPIOTE_CHANNEL_4,
    GPIOTE_CHANNEL_5,
    GPIOTE_CHANNEL_6,
    GPIOTE_CHANNEL_7,
} gpiote_channel_t;

typedef struct {
    uint8_t pin;
    gpiote_channel_t channel;
    gpiote_mode_t mode;
    gpiote_polarity_t polarity;
    gpiote_outinit_t outinit;
} gpiote_config_t;

void gpiote_init(nrf52832_irq_prio_t priority);
void gpiote_configure_channel(gpiote_config_t cfg);
void gpiote_enable_interrupt(gpiote_channel_t channel);
void gpiote_disable_interrupt(gpiote_channel_t channel);
void gpiote_enable_port_interrupt(void);
void gpiote_disable_port_interrupt(void);
void gpiote_set_channel_callback(gpiote_channel_t channel, gpiote_channel_cb_t cb);
void gpiote_set_port_callback(gpiote_port_cb_t cb);

static inline void gpiote_task_out(gpiote_channel_t channel) {
    mmio_write32(GPIOTE_TASKS_OUT_ADDR(channel), 1u);
}

static inline void gpiote_task_set(gpiote_channel_t channel) {
    mmio_write32(GPIOTE_TASKS_SET_ADDR(channel), 1u);
}

static inline void gpiote_task_clr(gpiote_channel_t channel) {
    mmio_write32(GPIOTE_TASKS_CLR_ADDR(channel), 1u);
}

#endif
