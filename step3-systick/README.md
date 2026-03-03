# SysTick (Core Timebase & Interrupt Introduction)

## Goal
This step introduces a system timebase using the Cortex-M4 SysTick exception.
It establishes interrupt-driven timing as a foundation for delays, timeouts, and future interrupt-based peripherals.

Unlike GPIO (SoC peripheral), SysTick is part of the ARM core and lives in the System Control Space (SCS). Implementing it extends the project from memory-mapped peripheral access to core exception handling and interrupt management.

## Architectural Overview
SysTick belongs to the Cortex-M4 core and is located in `0xE000E000` – System Control Space (SCS):

This differentiates it from Nordic-specific peripherals located in the `0x40000000` region.

The layering remains consistent:

```
Application
  |
systick.h
  |
cortexm4_map.h
  |
mmio.h
  |
Hardware (ARM core)
```

Additionally, a minimal intertupt abstraction was introduced (`irq.h`), providing controlled access to PRIMASK manipulation.

## Implementation design

### Tick Frequency
- Tick rate: 1kHz (1ms resolution)
- Derived from `CPU_HZ`
- Compile-time validated

```c
#define SYSTICK_HZ 1000u
#define SYSTICK_RELOAD (CPU_HZ / SYSTICK_HZ - 1u)
```
Validation:
- `CPU_HZ` divisible by `SYSTICK_HZ`
- Reload fits in 24-bit SysTick counter

This guarantees deterministic millisecond timing under the current clock assumption.

### SysTick configuration
Initialization performs:
- Set RELOAD register
- Clear CURRENT register
- Select processor clock (CLKSOURCE)
- Enable counter
- Optionally enable interrupt

The interrupt handler `void SysTick_Handler(void)` increments a private tick counter.

### Delay Implementation
`delay_ms()` uses unsigned wrap-safe subtraction:
```c
while ((uint32_t)(now - start) < ms);
```

This ensures correctness even with the 32-bit tick counter wraps.

The implementation assumes:
- SysTick interrupt enabled
- Global interrupts enabled

### Interrupt Control Layer
Since the interrupts must be enabled prior SysTick usage, a minimal interrupt abstraction was introduced:
- `irq_enable()` – enables interrupts and configurable fault handlers
- `irq_disable()` – disables interrupts and configurable fault handlers
- `irq_save()` – captures the current interrupt-enable state and disables IRQs
- `irq_restore()` – restores the interrupt-enable state

These operate on PRIMASK register using ARM instructions (`cpsid`, `cpsie`, `mrs`, `msr`).

## Conclusion
Step 3 extends the project from peripheral register manipulation to core exception handling and interrupt-driven execution.

With a 1 ms timebase and minimal interrupt control primitives in place, the system now supports timing-dependent logic and is structurally ready for NVIC configuration and peripheral interrupt handling in the next step.

