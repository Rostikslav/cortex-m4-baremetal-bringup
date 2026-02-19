#ifndef MMIO_H
#define MMIO_H

#include <stdint.h>

#define MMIO32(_addr) (*(volatile uint32_t *)(_addr))

#define BIT(_n) (1u << (_n))
#define GENMASK(_hi, _lo) ((~0u << (_lo)) & (~0u >> (31 - (_hi))))

// Volatile memory-mapped I/O access

// atomic 32-bit read
static inline uint32_t mmio_read32(uintptr_t addr) {
	return MMIO32(addr);
}

// atomic 32-bit write
static inline void mmio_write32(uintptr_t addr, uint32_t val) {
	MMIO32(addr) = val;
}

// NON-atomic 32-bit read-modify-write
static inline void mmio_rmw32(uintptr_t addr, uint32_t clear_mask, uint32_t set_mask) {
	uint32_t val = mmio_read32(addr);
	val &= ~clear_mask;
	val |= set_mask;
	mmio_write32(addr, val);
}

#endif
