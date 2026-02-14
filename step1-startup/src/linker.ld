ENTRY(Reset_Handler)

MEMORY
{
	FLASH (rx)	: ORIGIN = 0x00000000, LENGTH = 512K
	RAM   (rwx)	: ORIGIN = 0x20000000, LENGTH = 64K
}

_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
	.isr_vector :
	{
		. = ALIGN(4);
		KEEP(*(.isr_vector))
		. = ALIGN(4);
	} > FLASH

	.text : /* combines .text and .rodata */
	{
		. = ALIGN(4);
		*(.text .text.*)
		*(.rodata .rodata.*)
		. = ALIGN(4);
		_etext = .;
	} > FLASH

	.data : AT( _etext )
	{
		. = ALIGN(4);
		_sdata = .;
		*(.data .data.*)
		. = ALIGN(4);
		_edata = .;
	} > RAM

	_sidata = LOADADDR(.data);

	.bss (NOLOAD) :
	{
		. = ALIGN(4);
		_sbss = .;
		*(.bss .bss.*)
		*(COMMON)
		. = ALIGN(4);
		_ebss = .;
	} > RAM

	_end = .;

	/DISCARD/ : { *(.eh_frame*) *(.comment) *(.note*) }
}
