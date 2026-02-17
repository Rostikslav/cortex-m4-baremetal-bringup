#ifndef NRF52832_MAP_H
#define NRF52832_MAP_H

#define GPIO0_BASE	0x50000000UL

// offsets within GPIO port according to nRF52832 product specs
#define GPIO_OUT_OFFS			0x504
#define GPIO_OUTSET_OFFS		0x508
#define GPIO_OUTCLR_OFFS		0x50C
#define GPIO_IN_OFFS			0x510
#define GPIO_DIR_OFFS			0x514
#define GPIO_DIRSET_OFFS		0x518
#define GPIO_DIRCLR_OFFS		0x51C
#define GPIO_LATCH_OFFS			0x520

#define GPIO_DETECTMODE_OFFS	0x524
#define GPIO_DETECTMODE_DEFAULT	0
#define GPIO_DETECTMODE_LDETECT	1

// each pin has its own 32bit CNF register
#define GPIO_PIN_CNF_OFFS(_pin)	(0x700 + 4*(_pin))

// bit positions, masks and values of CNF register (per pin)
#define GPIO_CNF_DIR_POS		0
#define GPIO_CNF_DIR_MSK		(1u << GPIO_CNF_DIR_POS)
#define GPIO_CNF_DIR_INPUT		0
#define GPIO_CNF_DIR_OUTPUT		1

#define GPIO_CNF_IBUFF_POS		1
#define GPIO_CNF_IBUFF_MSK		(1u << GPIO_CNF_IBUFF_POS)
#define GPIO_CNF_IBUFF_CONN		0
#define GPIO_CNF_IBUFF_DISCONN	1

#define GPIO_CNF_PULL_POS		2
#define GPIO_CNF_PULL_MSK		(3u << GPIO_CNF_PULL_POS)
#define GPIO_CNF_PULL_NOPULL	0
#define GPIO_CNF_PULL_PULLDOWN	1
#define GPIO_CNF_PULL_PULLUP	3

#define GPIO_CNF_DRIVE_POS		8
#define GPIO_CNF_DRIVE_MSK		(7u << GPIO_CNF_DRIVE_POS)
#define GPIO_CNF_DRIVE_S0S1		0
#define GPIO_CNF_DRIVE_H0S1		1
#define GPIO_CNF_DRIVE_S0H1		2
#define GPIO_CNF_DRIVE_H0H1		3
#define GPIO_CNF_DRIVE_D0S1		4
#define GPIO_CNF_DRIVE_D0H1		5
#define GPIO_CNF_DRIVE_S0D1		6
#define GPIO_CNF_DRIVE_H0D1		7

#define GPIO_CNF_SENSE_POS		16
#define GPIO_CNF_SENSE_MSK		(3u << GPIO_CNF_SENSE_POS)
#define GPIO_CNF_SENSE_OFF		0
#define GPIO_CNF_SENSE_HIGH		2
#define GPIO_CNF_SENSE_LOW		3

#endif
