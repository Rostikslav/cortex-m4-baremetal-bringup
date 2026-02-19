.syntax unified
.cpu cortex-m4
.thumb


.extern _estack
.extern _sdata
.extern _edata
.extern _sidata
.extern _sbss
.extern _ebss

.extern main

.section .isr_vector, "a", %progbits
.global _isr_vector
.type _isr_vector, %object
.align 2
_isr_vector:
	/* cortex-4m interrupts */
	.word _estack
	.word Reset_Handler
	.word NMI_IRQHandler
	.word HardFault_IRQHandler
	.word MemManage_IRQHandler
	.word BusFault_IRQHandler
	.word UsageFault_IRQHandler
	.word 0
	.word 0
	.word 0
	.word 0
	.word SVCall_IRQHandler
	.word DebugMon_IRQHandler
	.word 0
	.word PendSV_IRQHandler
	.word SysTick_IRQHandler
	
	/* nrf52 specific interrupts */
	.word POWER_CLOCK_IRQHandler
	.word RADIO_IRQHandler
	.word UARTE0_UART0_IRQHandler
	.word SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQHandler
	.word SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQHandler
	.word NFCT_IRQHandler
	.word GPIOTE_IRQHandler
	.word SAADC_IRQHandler
	.word TIMER0_IRQHandler
	.word TIMER1_IRQHandler
	.word TIMER2_IRQHandler
	.word RTC0_IRQHandler
	.word TEMP_IRQHandler
	.word RNG_IRQHandler
	.word ECB_IRQHandler
	.word CCM_AAR_IRQHandler
	.word WDT_IRQHandler
	.word RTC1_IRQHandler
	.word QDEC_IRQHandler
	.word COMP_LPCOMP_IRQHandler
	.word SWI0_EGU0_IRQHandler
	.word SWI1_EGU1_IRQHandler
	.word SWI2_EGU2_IRQHandler
	.word SWI3_EGU3_IRQHandler
	.word SWI4_EGU4_IRQHandler
	.word SWI5_EGU5_IRQHandler
	.word TIMER3_IRQHandler
	.word TIMER4_IRQHandler
	.word PWM0_IRQHandler
	.word PDM_IRQHandler
	.word 0
	.word 0
	.word MWU_IRQHandler
	.word PWM1_IRQHandler
	.word PWM2_IRQHandler
	.word SPIM2_SPIS2_SPI2_IRQHandler
	.word RTC2_IRQHandler
	.word I2S_IRQHandler
	.word FPU_IRQHandler
	
	.size _isr_vector, . - _isr_vector


.section .text, "ax", %progbits
.global Reset_Handler
.type Reset_Handler, %function
Reset_Handler:
	ldr		r1, =_sbss
	ldr		r2, =_ebss
	mov		r3, #0
	bl		clear_bss
	
	ldr		r1, =_sidata
	ldr		r2, =_sdata
	ldr		r3, =_edata
	bl		copy_data

	bl		main
	b		. /* trap if main returns */
	
	.size	Reset_Handler, . - Reset_Handler
	

clear_bss:
	cmp		r1, r2
	bhs		clear_bss_done
	str		r3, [r1], #4
	b		clear_bss
clear_bss_done:
	bx 		lr


copy_data:
	cmp		r2, r3
	bhs		copy_data_done
	ldr		r0, [r1], #4
	str		r0, [r2], #4
	b		copy_data
copy_data_done:
	bx 		lr


.section .text.handlers, "ax", %progbits
.weak Default_Handler
.type Default_Handler, %function
Default_Handler:
	b		.
	.size	Default_Handler, . - Default_Handler


.macro	DEFAULT_BRANCH handler
.weak	\handler
.set	\handler, Default_Handler
.endm


DEFAULT_BRANCH NMI_IRQHandler
DEFAULT_BRANCH HardFault_IRQHandler
DEFAULT_BRANCH MemManage_IRQHandler
DEFAULT_BRANCH BusFault_IRQHandler
DEFAULT_BRANCH UsageFault_IRQHandler
DEFAULT_BRANCH SVCall_IRQHandler
DEFAULT_BRANCH DebugMon_IRQHandler
DEFAULT_BRANCH PendSV_IRQHandler
DEFAULT_BRANCH SysTick_IRQHandler
DEFAULT_BRANCH POWER_CLOCK_IRQHandler
DEFAULT_BRANCH RADIO_IRQHandler
DEFAULT_BRANCH UARTE0_UART0_IRQHandler
DEFAULT_BRANCH SPIM0_SPIS0_TWIM0_TWIS0_SPI0_TWI0_IRQHandler
DEFAULT_BRANCH SPIM1_SPIS1_TWIM1_TWIS1_SPI1_TWI1_IRQHandler
DEFAULT_BRANCH NFCT_IRQHandler
DEFAULT_BRANCH GPIOTE_IRQHandler
DEFAULT_BRANCH SAADC_IRQHandler
DEFAULT_BRANCH TIMER0_IRQHandler
DEFAULT_BRANCH TIMER1_IRQHandler
DEFAULT_BRANCH TIMER2_IRQHandler
DEFAULT_BRANCH RTC0_IRQHandler
DEFAULT_BRANCH TEMP_IRQHandler
DEFAULT_BRANCH RNG_IRQHandler
DEFAULT_BRANCH ECB_IRQHandler
DEFAULT_BRANCH CCM_AAR_IRQHandler
DEFAULT_BRANCH WDT_IRQHandler
DEFAULT_BRANCH RTC1_IRQHandler
DEFAULT_BRANCH QDEC_IRQHandler
DEFAULT_BRANCH COMP_LPCOMP_IRQHandler
DEFAULT_BRANCH SWI0_EGU0_IRQHandler
DEFAULT_BRANCH SWI1_EGU1_IRQHandler
DEFAULT_BRANCH SWI2_EGU2_IRQHandler
DEFAULT_BRANCH SWI3_EGU3_IRQHandler
DEFAULT_BRANCH SWI4_EGU4_IRQHandler
DEFAULT_BRANCH SWI5_EGU5_IRQHandler
DEFAULT_BRANCH TIMER3_IRQHandler
DEFAULT_BRANCH TIMER4_IRQHandler
DEFAULT_BRANCH PWM0_IRQHandler
DEFAULT_BRANCH PDM_IRQHandler
DEFAULT_BRANCH MWU_IRQHandler
DEFAULT_BRANCH PWM1_IRQHandler
DEFAULT_BRANCH PWM2_IRQHandler
DEFAULT_BRANCH SPIM2_SPIS2_SPI2_IRQHandler
DEFAULT_BRANCH RTC2_IRQHandler
DEFAULT_BRANCH I2S_IRQHandler
DEFAULT_BRANCH FPU_IRQHandler

.end
