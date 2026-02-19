TARGET = app
BUILD  = build

JLINK = JLinkExe
JLINK_SCRIPT := $(BUILD)/flash.jlink

CC      = arm-none-eabi-gcc
OBJDUMP = arm-none-eabi-objdump
GDB		= arm-none-eabi-gdb
SIZE    = arm-none-eabi-size

# device
DEFS = -DNRF52832_XXAA

CFLAGS  =	-mcpu=cortex-m4 -mthumb -mfloat-abi=soft \
			-ffunction-sections -fdata-sections \
			-ffreestanding -fno-builtin -Wall -Wextra -O2 -g3 \
			-MMD -MP \
			$(DEFS)

INCLUDES = -Isrc -Isrc/lib -Isrc/drivers -Isrc/soc
CFLAGS  += $(INCLUDES)


LDSCRIPT = src/linker.ld
LDFLAGS  = -T $(LDSCRIPT) -nostartfiles \
          -Wl,--gc-sections -Wl,-Map=$(BUILD)/$(TARGET).map

SRCS =	src/startup.S \
		src/main.c

OBJS = $(addprefix $(BUILD)/,$(SRCS:.c=.o))
OBJS := $(OBJS:.S=.o)

all: $(BUILD)/$(TARGET).elf

$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: %.S
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/$(TARGET).elf: $(OBJS) $(LDSCRIPT)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@
	$(SIZE) $@

$(JLINK_SCRIPT): $(BUILD)/$(TARGET).elf
	@mkdir -p $(BUILD)
	@printf '%s\n' \
	  'device NRF52832_XXAA' \
	  'if SWD' \
	  'speed 4000' \
	  'r' \
	  'h' \
	  'loadfile $(BUILD)/$(TARGET).elf' \
	  'r' \
	  'h' \
	  'g' \
	  'exit' > $@

disasm: $(BUILD)/$(TARGET).elf
	$(OBJDUMP) -d $< | less

clean:
	rm -rf $(BUILD)

flash: $(JLINK_SCRIPT)
	$(JLINK) -autoconnect 1 -commanderscript $(JLINK_SCRIPT)

debug: $(BUILD)/$(TARGET).elf
	$(GDB) $<

.PHONY: all clean disasm flash debug

DEPS := $(OBJS:.o=.d)
-include $(DEPS)
