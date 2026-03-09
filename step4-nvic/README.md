# NVIC (External Interrupt Handling)

## Goal
This step introduces the Nested Vectored Interrupt Controller (NVIC) and general interrupt handling beyond the SysTick exception implemented in the previous step.

The goal is to understand how external peripheral interrupts are routed through the NVIC and delivered to the CPU. A minimal GPIOTE-based interrupt source is used to validate the interrupt pipeline on real hardware.

The GPIOTE implementation in this step is intentionally minimal and serves only as an interrupt source. A full GPIOTE driver will be implemented in a later step.

## Architectural Overview
The initial interrupt setup uses following layering:
```
Application
  |
gpiote.h
  |
nrf52832_gpiote_map.h
  |
nvic.h
  |
cortexm4_map.h
  |
mmio.h
  |
Hardware (NVIC + GPIOTE)
```
Unlike SysTick from the previous step, which is a core exception, peripheral interrupts are routed through the NVIC before reaching the CPU.

The NVIC starts at `0xE000E100` in the System Control Space (SCS) alongside other core components.

## Implementation design

### NVIC Register Map
The NVIC register interface was implemented in `nvic.h` using memory-mapped register access layer implemented in previous steps.

Key register groups:
- ISER – Interrupt Set Enable Register
- ICER – Interrupt Clear Enable Register
- ISPR – Interrupt Set Pending Register
- ICPR – Interrupt Clear Pending Register
- IPR – Interrupt Priority Register

These registers allow software to fully control interrupt states and priorities.

### Interrupt Control

The NVIC driver exposes essential primitives:
- enable/disable interrupt
- get/set/clear pending state
- get/set interrupt priority

All listed operations are atomic, which is crutial in the interrupt processing logic.

Interrupt priorities follow the Cortex-M priority encoding scheme where lower numerical values correspond to higher priority.


### Interrupt Source
To validate NVIC behavior on hardware, the GPIOTE peripheral is used as a simple interrupt source.

A GPIO input edge generates a GPIOTE event which raises `GPIOTE_IRQn`.

The interrupt handler scans each GPIOTE channel, clears the event flag, and invokes the registered callback.

Example flow:
```
Button press
  |
GPIO edge detected
  |
GPIOTE sets EVENTS_IN[n]
  |
GPIOTE raises interrupt request
  |
NVIC dispatches GPIOTE_IRQHandler
  |
Driver clears event and invokes callback
```
To minimize ISR latency, the interrupt handler performs only minimal work and signals the main loop via an event counter.

## Testing
To validate the interrupt path, the following test setup was used:

- button input configured with pull-up
- LED output controlled via the GPIO driver from `step2-gpio`
- GPIOTE channel configured for Hi-to-Lo edge detection
- callback function which increments a global call counter
- `while()` loop for blinking the LED based on the call counter value

Each button press generated a GPIOTE interrupt that incremented the counter inside the ISR. The main loop then consumed the accumulated counter value to toggle the LED, producing a cumulative blinking effect. This confirmed correct interrupt delivery, handling and event clearing.

## Conclusion
Step 4 extends the system from core exceptions to external interrupt handling through the NVIC.

With NVIC register-level control implemented and validated using a peripheral interrupt source, the system now supports interrupt-driven peripherals.

The minimal GPIOTE implementation used for testing will be expanded into a complete peripheral driver in the next step.

