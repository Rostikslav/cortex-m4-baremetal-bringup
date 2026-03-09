#include "gpiote.h"
#include "mmio.h"
#include "nrf52832_gpiote_map.h"
#include "nrf52832_irq.h"
#include "nvic.h"
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

static gpiote_cb_t cb_map[8] = {0};

static inline void gpiote_event_clear(uint8_t channel) {
    mmio_write32(GPIOTE_EVENTS_IN_ADDR(channel), 0);
}

static inline bool gpiote_event_is_set(uint8_t channel) {
    return mmio_read32(GPIOTE_EVENTS_IN_ADDR(channel)) != 0;
}

void gpiote_init(nrf52832_irq_prio_t priority) {
    nvic_disable_irq(GPIOTE_IRQn);

    mmio_write32(GPIOTE_INTENCLR_ADDR, -1u);
    mmio_write32(GPIOTE_EVENTS_PORT_ADDR, 0);
    for(uint8_t ch = 0; ch < 8; ch++)
        gpiote_event_clear(ch);

    nvic_clear_pending_irq(GPIOTE_IRQn);
    nvic_set_priority_raw(GPIOTE_IRQn, (uint8_t)priority);
    nvic_enable_irq(GPIOTE_IRQn);
}

void gpiote_configure_channel(gpiote_config_t cfg) {
    uint32_t cfg_word = cfg.mode
                      | GPIOTE_CONFIG_PSEL(cfg.pin)
                      | cfg.polarity
                      | cfg.outinit;
    gpiote_disable_interrupt(cfg.channel);
    gpiote_event_clear(cfg.channel);
    cb_map[cfg.channel] = NULL;
    mmio_write32(GPIOTE_CONFIG_ADDR(cfg.channel), cfg_word);
}

void gpiote_enable_interrupt(gpiote_channel_t channel) {
    if(mmio_read32(GPIOTE_INTENSET_ADDR) & GPIOTE_INTENSET_IN_MSK(channel))
        return;

    gpiote_event_clear(channel);
    mmio_write32(GPIOTE_INTENSET_ADDR, GPIOTE_INTENSET_IN_MSK(channel));
}

void gpiote_disable_interrupt(gpiote_channel_t channel) {
    mmio_write32(GPIOTE_INTENCLR_ADDR, GPIOTE_INTENCLR_IN_MSK(channel));
    gpiote_event_clear(channel);

}

void gpiote_set_callback(gpiote_channel_t channel, gpiote_cb_t cb) {
    cb_map[channel] = cb;
}

void GPIOTE_IRQHandler(void) {
    for(uint8_t ch = 0; ch < 8; ch++) {
        if(gpiote_event_is_set(ch)) {
            gpiote_event_clear(ch);
            if(cb_map[ch]) cb_map[ch]();
        }
    }
}

