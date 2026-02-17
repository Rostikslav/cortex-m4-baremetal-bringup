# Step 2 – GPIO Driver (Foundational Peripheral Abstraction)

## Goal
Implement a minimal GPIO driver early in the project to establish a reliable hardware interaction layer before introducing timing, interrupts, and communication peripherals.

GPIO provides immediate physical observability (LEDs, buttons), which makes it an effective debugging and validation tool for subsequent steps.

Furthermore, nearly all microcontroller components (including core peripherals like SysTick) are memory-mapped. Implementing GPIO first ensures a solid understanding of register-level interaction, bitfield manipulation, and atomic register semantics before building more complex subsystems on top of the same principles.

## Architectural Overview
The implementation follows a strict layered structure:
```
Application
  |
gpio.h              (Driver API, handle abstraction)
  |
nrf52832_map.h      (Register map: base addresses, offsets, masks)
  |
mmio.h              (Volatile memory-mapped I/O primitives)
  |
Hardware
```

This separation ensures:
- Hardware facts remain isolated
- Register access is centralized
- Peripheral logic remains readable and deterministic

## Design Principles

### 1. Handle-Based Abstraction
Each pin is represented as:
   ```c
   typedef struct {
       uintptr_t base;
       uint8_t   pin;
   } gpio_t;
   ```
  which keeps the API scalable and explicit.

### 2. Atomic Register Usage
Runtime changes use hardware-provided atomic registers:
- `OUTSET` / `OUTCLR`
- `DIRSET` / `DIRCLR`

This prevents unintended modification of unrelated pins and avoids software read-modify-write hazards.

### 3. Explicit Bitfield Management
`PIN_CNF[n]` fields are handled using:
- Raw enum values
- Shifts (`*_POS`)
- Masks (`*_MSK`)
- `mmio_rmw32()` updates

### 4. Clear Read Semantics
To avoid ambiguity between electrical state and driven state, two read functions are provided:
- `gpio_read_level()` – reads physical pin level (`IN`)
- `gpio_read_output()` – reads output latch (`OUT`)

### 5. Handling of Special Registers
- `LATCH` is treated as write-1-to-clear (W1C) register
- `DETECTMODE` is handled at port level

## Summary

This step establishes the first reusable peripheral abstraction within the project.

With a structured register map, centralized MMIO access layer, and a handle-based driver interface, the project now has a consistent pattern for interacting with memory-mapped hardware. The architectural conventions introduced here form the template for all subsequent components, including SysTick configuration, NVIC interrupt handling, UART communication and more.
