#ifndef NVIC_H
#define NVIC_H

#include <stdint.h>
#include "cortexm4_map.h"
#include "mmio.h"

#define NVIC_REG_IDX(_irqn) ((_irqn) >> 5)
#define NVIC_REG_BIT(_irqn) (1u << ((uint32_t)(_irqn) & 31u))

static inline void nvic_enable_irq(uint32_t irqn) {
    mmio_write32(NVIC_ISER(NVIC_REG_IDX(irqn)), NVIC_REG_BIT(irqn));   
}

static inline void nvic_disable_irq(uint32_t irqn) {
    mmio_write32(NVIC_ICER(NVIC_REG_IDX(irqn)), NVIC_REG_BIT(irqn));   
}

static inline uint32_t nvic_get_pending_irq(uint32_t irqn) {
    return mmio_read32(NVIC_ISPR(NVIC_REG_IDX(irqn))) & NVIC_REG_BIT(irqn);
}

static inline void nvic_set_pending_irq(uint32_t irqn) {
    mmio_write32(NVIC_ISPR(NVIC_REG_IDX(irqn)), NVIC_REG_BIT(irqn));   
}

static inline void nvic_clear_pending_irq(uint32_t irqn) {
    mmio_write32(NVIC_ICPR(NVIC_REG_IDX(irqn)), NVIC_REG_BIT(irqn));   
}

static inline uint8_t nvic_get_priority_raw(uint32_t irqn) {
    return mmio_read8(NVIC_IPR(irqn));
}

static inline void nvic_set_priority_raw(uint32_t irqn, uint8_t prio) {
    mmio_write8(NVIC_IPR(irqn), prio);
}

#endif
