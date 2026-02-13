# cortex-m4-baremetal-bringup
Cortex-M4 bare-metal bring-up progression from memory layout to peripheral drivers.

Validated on the Nordic nRF52832 (Cortex-M4F).

# Motivation

Embedded software is often developed on top of vendor frameworks that abstract away the processor startup sequence, memory model, interrupt configuration, and raw peripheral access.

While these abstractions are practical, they can obscure how the system actually operates at the architectural level.

The objective is to make the underlying mechanisms visible and explicit, rather than relying on prebuilt infrastructure.

# Overview
This repository documents the incremental bring-up of a Cortex-M4 system without relying on CMSIS, vendor startup files, or SDK frameworks.

It begins at the architectural entry point - the reset vector - and implements a runtime environment incrementally, including:

- Linker script and memory layout
- Vector table definition
- Custom Reset_Handler including section initialization for `.bss` and `.data`
- Exception decoding and fault handling
- Direct memory-mapped peripheral access

and more.

The project is organized as a sequence of incremental steps. Each step extends the previous one by introducing a new architectural concept or subsystem. While the steps are conceptually cumulative, each remains self-contained and can be built and inspected independently.

The target platform used for validation is the nRF52832 DK, chosen as a widely available and practical Cortex-M4F board with reliable debugging support.

# Requirements

This project was developed and tested under Arch Linux.

Package names may differ on other distributions.

## Toolchain
- arm-none-eabi-gcc
- arm-none-eabi-binutils
- arm-none-eabi-gdb
- make

```bash
sudo pacman -S arm-none-eabi-gcc arm-none-eabi-binutils arm-none-eabi-gdb make
```

## Debugging
SEGGER J-Link software (includes JLinkExe and JLinkGDBServer)

On Arch, typically installed via:
```bash
yay -S jlink-software-and-documentation
```

## Hardware
- Cortex-M4 target (validated on nRF52832 DK)
- J-Link compatible debug probe
- SWD connection

## Usage
Each sub-project (step) includes its own pre-configured `Makefile` along with a `gdbserver.sh` script to simplify build and debug process.

Enter a step directory:
```bash
cd stepX-<name>
```

### Build
```bash
make
```
This generates:

 - `build/app.elf` — linked ELF image (includes symbols and section layout)
 - `build/app.map` — linker map file
 - `build/src/*.o` — object files

 The ELF file is used for both flashing and debugging.

 ### Flash
 ```bash
 make flash
 ```
 Creates a JLink script and flashes the board.

 ### Debug
 Start GDBServer:
 ```bash
 ./scripts/gdbserver.sh
 ```
Start GDB:
 ```bash
 make debug
 ```
 or directly:
```bash
arm-none-eabi-gdb build/app.elf
```
Inside GDB:
```bash
target remote :2331
monitor reset
```
`load` command followed by `reset` can be used inside GDB to program and reset the MCU accordingly without terminating the session.
