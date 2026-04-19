#ifndef CORTEXM4_MAP_H
#define CORTEXM4_MAP_H

// system control space
#define SCS_BASE                0xE000E000L

// SysTick

// Control and Status Register
#define SYST_CSR_OFFS           0x0010
#define SYST_CSR_ENABLE_POS     0
#define SYST_CSR_ENABLE_MSK     (1u << SYST_CSR_ENABLE_POS)
#define SYST_CSR_TICKINT_POS    1
#define SYST_CSR_TICKINT_MSK    (1u << SYST_CSR_TICKINT_POS)
#define SYST_CSR_CLKSOURCE_POS  2
#define SYST_CSR_CLKSOURCE_MSK  (1u << SYST_CSR_CLKSOURCE_POS)
#define SYST_CSR_COUNTFLAG_POS  16
#define SYST_CSR_COUNTFLAG_MSK  (1u << SYST_CSR_COUNTFLAG_POS)

// Reload Value Register
#define SYST_RVR_OFFS           0x0014
#define SYST_RVR_RELOAD_POS     0
#define SYST_RVR_RELOAD_MSK     (0xFFFFFFu << SYST_RVR_RELOAD_POS)

// Current Value Register
#define SYST_CVR_OFFS           0x0018
#define SYST_CVR_CURRENT_POS    0
#define SYST_CVR_CURRENT_MSK    (0xFFFFFFu << SYST_CVR_CURRENT_POS) 

// Calibration Value Register
#define SYST_CALIB_OFFS         0x001C
#define SYST_CALIB_TENMS_POS    0
#define SYST_CALIB_TENMS_MSK    (0xFFFFFFu << SYST_CALIB_TENMS_POS)
#define SYST_CALIB_SKEW_POS     30
#define SYST_CALIB_SKEW_MSK     (1u << SYST_CALIB_SKEW_POS)
#define SYST_CALIB_NOREF_POS    31
#define SYST_CALIB_NOREF_MSK    (1u << SYST_CALIB_NOREF_POS)

// NVIC
#define NVIC_BASE               (SCS_BASE + 0x0100)
#define NVIC_ISER_BASE          (NVIC_BASE + 0x000)
#define NVIC_ICER_BASE          (NVIC_BASE + 0x080)
#define NVIC_ISPR_BASE          (NVIC_BASE + 0x100)
#define NVIC_ICPR_BASE          (NVIC_BASE + 0x180)
#define NVIC_IABR_BASE          (NVIC_BASE + 0x200)
#define NVIC_IPR_BASE           (NVIC_BASE + 0x300)
#define NVIC_STIR               (NVIC_BASE + 0xE00)

#define NVIC_ISER(_reg_i)       (NVIC_ISER_BASE + ((_reg_i) << 2))
#define NVIC_ICER(_reg_i)       (NVIC_ICER_BASE + ((_reg_i) << 2))
#define NVIC_ISPR(_reg_i)       (NVIC_ISPR_BASE + ((_reg_i) << 2))
#define NVIC_ICPR(_reg_i)       (NVIC_ICPR_BASE + ((_reg_i) << 2))
#define NVIC_IPR(_irqn)         (NVIC_IPR_BASE + ((uint32_t) _irqn))

#endif
