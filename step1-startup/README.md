# Step 1 – Startup Sequence and Minimal Runtime

## Goal
Implement a minimal Cortex-M4 runtime environment starting from the memory layout and reaching `main()`.

This forms the architectural foundation for all subsequent steps.

## Memory Layout
Before any code executes, the memory layout must be defined.
According to the nRF52832 product specs, there are two primary memory regions:

- **FLASH:** `0x00000000 – 0x00080000` (512 KiB)
- **RAM:** `0x20000000 – 0x20010000` (64 KiB)

These regions are fixed by hardware. Section placement inside them is controlled via the linker script.

### Linker Memory Definition
```ld
MEMORY
{
    FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 512K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 64K
}
```

Within these regions, the following sections are defined:
- `.isr_vector` – Located at the beginning of FLASH. Contains the vector table.
- `.text` – Executable code. Stored in FLASH.
- `.rodata` – Read-only constants. Merged into `.text` for simplicity   .
- `.data` – Initialized variables.
  - Stored in FLASH (Load Memory Address, LMA)
  - Copied into RAM (Virtual Memory Address, VMA) before `main()`
- `.bss` – Uninitialized variables.
  - Allocated in RAM
  - Zeroed before `main()`

### Runtime Symbols

The linker provides symbols required for initialization:
- `_estack` — Initial stack pointer. `ORIGIN(RAM) + LENGTH(RAM)`
- `_sbss`, `_ebss` — Bounds of `.bss`
- `_sidata` — Flash start of `.data` (LMA)
- `_sdata`, `_edata` — RAM bounds of `.data` (VMA)

These symbols are accessed in the startup file and allow the startup code to initialize memory correctly.

## Vector Table
The vector table lives in `.isr_vector` at address `0x00000000`.

Structure:
- Initial Stack Pointer (SP)
- Reset_Handler address
- Core exception handlers (ARM-defined)
- Device-specific IRQ handlers (Nordic-defined)

Only `Reset_Handler` has a strong definition at this stage.

All other handlers are weak aliases to `Default_Handler`.

### Verification
From the linker map:
```bash
cat build/app.map | grep isr_vector
```
Output:
```
.isr_vector     0x00000000       0xdc
 *(.isr_vector)
 .isr_vector    0x00000000       0xdc build/src/startup.o
                0x00000000                _isr_vector
```
The vector table is correctly placed at address `0x00000000`.

### Inspecting Vector Contents
```bash
arm-none-eabi-objdump -s -j .isr_vector build/app.elf
```
Output:
```
build/app.elf:     file format elf32-littlearm

Contents of section .isr_vector:
 0000 00000120 dd000000 29010000 29010000  ... ....)...)...
 0010 29010000 29010000 29010000 00000000  )...)...).......
 0020 00000000 00000000 00000000 29010000  ............)...
 0030 29010000 00000000 29010000 29010000  ).......)...)...
 0040 29010000 29010000 29010000 29010000  )...)...)...)...
 0050 29010000 29010000 29010000 29010000  )...)...)...)...
 0060 29010000 29010000 29010000 29010000  )...)...)...)...
 0070 29010000 29010000 29010000 29010000  )...)...)...)...
 0080 29010000 29010000 29010000 29010000  )...)...)...)...
 0090 29010000 29010000 29010000 29010000  )...)...)...)...
 00a0 29010000 29010000 29010000 29010000  )...)...)...)...
 00b0 29010000 29010000 00000000 00000000  )...)...........
 00c0 29010000 29010000 29010000 29010000  )...)...)...)...
 00d0 29010000 29010000 29010000           )...)...)...    

```

First two entries (little-endian):
- `00000120` → `0x20010000` → Initial SP (`0x20000000 + 0x00010000`)
- `dd000000` → `0x000000DD` → Reset handler address

The repeated values (`0x00000129`) in the dump correspond to the address of `Default_Handler` with the Thumb bit set.

Since all interrupt handlers except `Reset_Handler` are currently defined as weak aliases to `Default_Handler`, their vector table entries resolve to the same address. This is expected behavior at this stage of the bring-up.

### Symbol table inspection
```bash
arm-none-eabi-nm -n build/app.elf | grep -E 'Reset_Handler'
```
Output:
```
000000dc T Reset_Handler
```

The vector table stores `0x000000DD`, while the symbol shows `0x000000DC`, which is correct.

Cortex-M executes only in Thumb mode.
`Bit[0]` of an exception return address must be set to 1 to indicate Thumb state.

Therefore:
```
stored_address = physical_address | 1
```

## Reset_Handler
`Reset_Handler` performs mandatory runtime initialization:
1. Zero `.bss`
2. Copy `.data` from FLASH to RAM
3. Call `main()`
4. Trap if `main()` returns

Simplified flow:
```asm
Reset_Handler:
    bl clear_bss
    bl copy_data
    bl main
1:  b 1b
```

The trap ensures execution does not continue into undefined memory if `main()` returns.

## Literal Pools
The startup code loads linker symbols using:
```asm
ldr r1, =_sbss
```

This is a pseudo-instruction. Since the processor cannot encode full 32-bit addresses directly into a single instruction, the assembler:

- Places the constant into a literal pool inside `.text`
- Generates a PC-relative load instruction to fetch it

Literal pools appear in the disassembly as `.word` entries and are expected behavior.

Literal pool emission can be controlled explicitly using the `.ltorg` directive. However, since PC-relative loads have a limited reach, forcing pool placement must ensure that the generated constants remain within range of the referencing instructions.

## Debug Verification

After flashing and setting a breakpoint at main:
```
Breakpoint 1, main () at src/main.c:3
```
### Register state:
```
sp   0x20010000
pc   0x12c <main>
xpsr 0x61000000
```
Observations:
- `SP = 0x20010000` → matches `_estack`
- `PC = 0x12 <main>` → control flow reached C code
- `xPSR = 0x61000000`
  - `Bit[24] (T) = 1` → valid Thumb execution
  - `Bits[8:0] (IPSR) = 0` → not inside an interrupt


### Section initialization sanity check (`.data` / `.bss`)
To validate that `.data` is copied from FLASH to RAM and `.bss` is zeroed before `main()`, two globals are used:

```c
volatile uint32_t g_data = 0x12345678; // .data (initialized)
volatile uint32_t g_bss;               // .bss (uninitialized, should be 0)
```
They are referenced in `main()` to prevent dead-stripping:
```c
(void)g_data;
(void)g_bss;
```
#### ELF section placement
```bash
arm-none-eabi-objdump -h build/app.elf | grep -E 'Name|\.data|\.bss'
```
Output:
```
Idx Name     Size      VMA       LMA       File off  Algn
  2 .data    00000004  20000000  00000140  00002000  2**2
  3 .bss     00000004  20000004  00000144  00002004  2**2
```
`.data` has a RAM VMA (runtime location), a FLASH LMA (load location).

`.bss` lives in RAM and is expected to be zero-initialized by the reset sequence.

#### Runtime verification (GDB)
```gdb
x/wx &g_bss
x/wx &g_data
```
Output:
```
0x20000004 <g_bss>:     0x00000000
0x20000000 <g_data>:    0x12345678
```
This confirms:

- `.bss` was cleared to zero before entering `main()`
- `.data` initializers were copied from FLASH into RAM correctly

## Outcome
At this stage, the system:
- Boots from reset
- Uses a custom vector table
- Correctly initializes `.bss` and `.data`
- Enters `main()` under debugger control

This establishes full control over the startup process without external runtime support.
