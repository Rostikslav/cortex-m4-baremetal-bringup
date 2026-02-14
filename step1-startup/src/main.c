
#include <stdint.h>

volatile uint32_t g_data = 0x12345678; // .data
volatile uint32_t g_bss;               // .bss

int main(void) {
	(void)g_bss;
	(void)g_data;
	
	while (1) {}
}
