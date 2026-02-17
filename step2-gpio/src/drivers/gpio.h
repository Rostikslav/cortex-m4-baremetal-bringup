#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include <stdbool.h>
#include "nrf52832_map.h"
#include "mmio.h"

#define GPIO0_PIN(_pin_n) \
	((gpio_t){.base = GPIO0_BASE, .pin = (_pin_n)})
	
#define GPIO_PIN_CNF_ADDR(_g) \
	((_g).base + GPIO_PIN_CNF_OFFS((_g).pin))
	
#define GPIO_PIN_CNF_SET(_g, _pos, _msk, _raw) \
    mmio_rmw32(GPIO_PIN_CNF_ADDR(_g), (_msk), (((uint32_t)(_raw) << (_pos)) & (_msk)))

#define GPIO_PIN_CNF_GET(_g, _pos, _msk) \
    ((mmio_read32(GPIO_PIN_CNF_ADDR(_g)) & (_msk)) >> (_pos))

typedef struct {
	uintptr_t	base;
	uint8_t		pin;
} gpio_t;

typedef enum {
	GPIO_DETECT_MODE_DEFAULT = GPIO_DETECTMODE_DEFAULT,
	GPIO_DETECT_MODE_LATCH	 = GPIO_DETECTMODE_LDETECT,
} gpio_detectmode_t;

typedef enum {
	GPIO_INPUT	= GPIO_CNF_DIR_INPUT,
	GPIO_OUTPUT	= GPIO_CNF_DIR_OUTPUT, 
} gpio_cnf_dir_t;

typedef enum {
	GPIO_IBUFF_CONNECTED	= GPIO_CNF_IBUFF_CONN,
	GPIO_IBUFF_DISCONNECTED	= GPIO_CNF_IBUFF_DISCONN,
} gpio_cnf_ibuff_t;

typedef enum {
	GPIO_PULL_NONE	= GPIO_CNF_PULL_NOPULL,
	GPIO_PULL_DOWN	= GPIO_CNF_PULL_PULLDOWN,
	GPIO_PULL_UP	= GPIO_CNF_PULL_PULLUP,
} gpio_cnf_pull_t;

typedef enum {
	GPIO_DRIVE_S0S1 = GPIO_CNF_DRIVE_S0S1,
	GPIO_DRIVE_H0S1 = GPIO_CNF_DRIVE_H0S1,
	GPIO_DRIVE_S0H1 = GPIO_CNF_DRIVE_S0H1,
	GPIO_DRIVE_H0H1 = GPIO_CNF_DRIVE_H0H1,
	GPIO_DRIVE_D0S1 = GPIO_CNF_DRIVE_D0S1,
	GPIO_DRIVE_D0H1 = GPIO_CNF_DRIVE_D0H1,
	GPIO_DRIVE_S0D1 = GPIO_CNF_DRIVE_S0D1,
	GPIO_DRIVE_H0D1 = GPIO_CNF_DRIVE_H0D1,
} gpio_cnf_drive_t;

typedef enum {
	GPIO_SENSE_OFF	= GPIO_CNF_SENSE_OFF,
	GPIO_SENSE_HIGH	= GPIO_CNF_SENSE_HIGH,
	GPIO_SENSE_LOW	= GPIO_CNF_SENSE_LOW,
} gpio_cnf_sense_t;

typedef struct {
	gpio_cnf_dir_t dir;
	gpio_cnf_ibuff_t ibuff;
	gpio_cnf_pull_t pull;
	gpio_cnf_drive_t drive;
	gpio_cnf_sense_t sense;
} gpio_cnf_t;

// ----- IN/OUT -----

// atomic pin level read via IN register (input buffer must be connected)
static inline bool gpio_read_level(gpio_t g) {
	return (mmio_read32(g.base + GPIO_IN_OFFS) & BIT(g.pin)) != 0;
}

// atomic output state read via OUT register
static inline bool gpio_read_output(gpio_t g) {
	return (mmio_read32(g.base + GPIO_OUT_OFFS) & BIT(g.pin)) != 0;
}

// ----- OUTSET/OUTCLR -----

// atomic set via OUTSET register
static inline void gpio_set(gpio_t g) {
	mmio_write32(g.base + GPIO_OUTSET_OFFS, BIT(g.pin));
}

// atomic clear via OUTCLR register
static inline void gpio_clr(gpio_t g) {
	mmio_write32(g.base + GPIO_OUTCLR_OFFS, BIT(g.pin));
}

// atomic write via OUTSET/OUTCLR based on state
static inline void gpio_write(gpio_t g, bool state) {
	if(state)
		gpio_set(g);
	else
		gpio_clr(g);
}

// ----- DIR/DIRSET/DIRCLR -----

// atomic direction check via DIR register
static inline bool gpio_is_input(gpio_t g) {
	return (mmio_read32(g.base + GPIO_DIR_OFFS) & BIT(g.pin)) == 0;
}

// atomic direction check via DIR register
static inline bool gpio_is_output(gpio_t g) {
	return (mmio_read32(g.base + GPIO_DIR_OFFS) & BIT(g.pin)) != 0;
}

// atomic direction setter via DIRSET/DIRCLR register
static inline void gpio_dir_set(gpio_t g, gpio_cnf_dir_t state) {
	if(state == GPIO_OUTPUT)
		mmio_write32(g.base + GPIO_DIRSET_OFFS, BIT(g.pin));
	else
		mmio_write32(g.base + GPIO_DIRCLR_OFFS, BIT(g.pin));
}

// ----- LATCH -----

// atomic raw latch state getter
static inline uint32_t gpio_latch_get_raw(uintptr_t base_addr) {
	return mmio_read32(base_addr + GPIO_LATCH_OFFS);
}

// atomic per-pin latch state check
static inline bool gpio_latch_is_set(gpio_t g) {
	return (mmio_read32(g.base + GPIO_LATCH_OFFS) & BIT(g.pin)) != 0;
}

// atomic latch state clear 
static inline void gpio_latch_clr(gpio_t g) {
	mmio_write32(g.base + GPIO_LATCH_OFFS, BIT(g.pin));
}

// ----- DETECTMODE -----

// atomic port detect mode getter
static inline gpio_detectmode_t gpio_detectmode_get(uintptr_t base_addr) {
	return mmio_read32(base_addr + GPIO_DETECTMODE_OFFS);
}

// atomic port detect mode setter
static inline void gpio_detectmode_set(uintptr_t base_addr, gpio_detectmode_t mode) {
	mmio_write32(base_addr + GPIO_DETECTMODE_OFFS, mode);
}

// ------ CNF ------

// atomic per-pin input buffer state getter
static inline gpio_cnf_ibuff_t gpio_ibuff_get(gpio_t g) {
	return GPIO_PIN_CNF_GET(g, GPIO_CNF_IBUFF_POS, GPIO_CNF_IBUFF_MSK);
}

// NON-atomic per-pin input buffer state setter
static inline void gpio_ibuff_set(gpio_t g, gpio_cnf_ibuff_t state) {
	GPIO_PIN_CNF_SET(g, GPIO_CNF_IBUFF_POS, GPIO_CNF_IBUFF_MSK, state);
}

// atomic per-pin pull mode getter
static inline gpio_cnf_pull_t gpio_pull_get(gpio_t g) {
	return GPIO_PIN_CNF_GET(g, GPIO_CNF_PULL_POS, GPIO_CNF_PULL_MSK);
}

// NON-atomic per-pin pull mode setter
static inline void gpio_pull_set(gpio_t g, gpio_cnf_pull_t state) {
	GPIO_PIN_CNF_SET(g, GPIO_CNF_PULL_POS, GPIO_CNF_PULL_MSK, state);
}

// atomic per-pin drive mode getter
static inline gpio_cnf_drive_t gpio_drive_get(gpio_t g) {
	return GPIO_PIN_CNF_GET(g, GPIO_CNF_DRIVE_POS, GPIO_CNF_DRIVE_MSK);
}

// NON-atomic per-pin drive mode setter
static inline void gpio_drive_set(gpio_t g, gpio_cnf_drive_t state) {
	GPIO_PIN_CNF_SET(g, GPIO_CNF_DRIVE_POS, GPIO_CNF_DRIVE_MSK, state);
}

// atomic per-pin sense mode getter
static inline gpio_cnf_sense_t gpio_sense_get(gpio_t g) {
	return GPIO_PIN_CNF_GET(g, GPIO_CNF_SENSE_POS, GPIO_CNF_SENSE_MSK);
}

// NON-atomic per-pin sense mode setter
static inline void gpio_sense_set(gpio_t g, gpio_cnf_sense_t state) {
	GPIO_PIN_CNF_SET(g, GPIO_CNF_SENSE_POS, GPIO_CNF_SENSE_MSK, state);
}

// atomic per-pin config getter
// returns the whole register value
static inline uint32_t gpio_config_get_raw(gpio_t g) {
	return mmio_read32(g.base + GPIO_PIN_CNF_OFFS(g.pin));
}

// atomic per-pin config setter
// overrides the whole register
static inline void gpio_config_set(gpio_t g, gpio_cnf_t cnf) {
	uint32_t cnf_val = 
		(cnf.dir << GPIO_CNF_DIR_POS) |
		(cnf.ibuff << GPIO_CNF_IBUFF_POS) |
		(cnf.pull << GPIO_CNF_PULL_POS) |
		(cnf.drive << GPIO_CNF_DRIVE_POS) |
		(cnf.sense << GPIO_CNF_SENSE_POS);
		
	mmio_write32(g.base + GPIO_PIN_CNF_OFFS(g.pin), cnf_val);
}

// NON-atomic per-pin config update
// overrides only masked values
static inline void gpio_config_update(gpio_t g, gpio_cnf_t cnf, uint32_t cnf_mask) {
	uint32_t cnf_val = 
			(cnf.dir << GPIO_CNF_DIR_POS) |
			(cnf.ibuff << GPIO_CNF_IBUFF_POS) |
			(cnf.pull << GPIO_CNF_PULL_POS) |
			(cnf.drive << GPIO_CNF_DRIVE_POS) |
			(cnf.sense << GPIO_CNF_SENSE_POS);
			
	cnf_val &= cnf_mask;
	mmio_rmw32(g.base + GPIO_PIN_CNF_OFFS(g.pin), cnf_mask, cnf_val);
}

#endif
