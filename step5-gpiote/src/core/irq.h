#ifndef IRQ_H
#define IRQ_H

#include <stdint.h>

static inline void irq_enable(void) {
    __asm volatile("cpsie i" ::: "memory");
}

static inline void irq_disable(void) {
    __asm volatile("cpsid i" ::: "memory");
}

static inline uint32_t irq_save(void) {
    uint32_t primask;
    __asm volatile("\
        mrs %0, PRIMASK\n\
        cpsid i\n\
        "
        : "=r"(primask)
        :
        : "memory");
    return primask;
    
}

static inline void irq_restore(uint32_t primask) {
    __asm volatile("\
        msr PRIMASK, %0\n\
    "
    :
    :"r"(primask)
    :"memory");
}

#endif
