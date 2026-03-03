#ifndef CPU_HZ
#error "CPU_HZ not defined"
#endif

#include <stdint.h>
#include <stdbool.h>
#include "systick.h"
#include "cortexm4_map.h"
#include "mmio.h"

#define SYSTICK_HZ          1000u
#define SYSTICK_RELOAD      (CPU_HZ / SYSTICK_HZ - 1u)
#define SYST_CSR_CLKSOURCE  1    

_Static_assert(CPU_HZ % SYSTICK_HZ == 0, "CPU_HZ must be divisible by SYSTICK_HZ");
_Static_assert(SYSTICK_RELOAD <= 0xFFFFFFu, "SysTick reload out of range");

static volatile uint32_t g_systick_ticks;

void SysTick_Handler(void) {
    g_systick_ticks++;
}

void systick_init_1ms(void) {
    
    mmio_write32(SCS_BASE + SYST_RVR_OFFS, SYSTICK_RELOAD & SYST_RVR_RELOAD_MSK);
    mmio_write32(SCS_BASE + SYST_CVR_OFFS, 0);

    uint32_t csr_val =  (0 & SYST_CSR_ENABLE_MSK) |
                        (0 & SYST_CSR_TICKINT_MSK) |
                        ((SYST_CSR_CLKSOURCE << SYST_CSR_CLKSOURCE_POS) & SYST_CSR_CLKSOURCE_MSK);
                        
    mmio_write32(SCS_BASE + SYST_CSR_OFFS, csr_val);
}

void systick_start(bool enable_irq) {
    uint32_t csr_val =  (SYST_CSR_ENABLE_MSK) |
                        (enable_irq ? SYST_CSR_TICKINT_MSK : 0 ) |
                        ((SYST_CSR_CLKSOURCE << SYST_CSR_CLKSOURCE_POS) & SYST_CSR_CLKSOURCE_MSK);
                            
    mmio_write32(SCS_BASE + SYST_CSR_OFFS, csr_val);
}

void systick_stop(bool reset_ticks) {
    uint32_t csr_val =  (0 & SYST_CSR_ENABLE_MSK) |
                        (0 & SYST_CSR_TICKINT_MSK) |
                        ((SYST_CSR_CLKSOURCE << SYST_CSR_CLKSOURCE_POS) & SYST_CSR_CLKSOURCE_MSK);
                                
    mmio_write32(SCS_BASE + SYST_CSR_OFFS, csr_val);
    
    if(reset_ticks) g_systick_ticks = 0;
}

uint32_t systick_ticks_get(void) {
    return g_systick_ticks;
}

void delay_ms(uint32_t ms){
    uint32_t ticks_start = g_systick_ticks;
    while((uint32_t)(g_systick_ticks - ticks_start) < ms);
}
