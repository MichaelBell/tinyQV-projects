NAME = hello

PROJECT_SOURCES ?= main.c

RISCV_TOOLCHAIN ?= /opt/tinyQV

CC = $(RISCV_TOOLCHAIN)/bin/riscv32-unknown-elf-gcc
AS = $(RISCV_TOOLCHAIN)/bin/riscv32-unknown-elf-as
AR = $(RISCV_TOOLCHAIN)/bin/riscv32-unknown-elf-ar
LD = $(RISCV_TOOLCHAIN)/bin/riscv32-unknown-elf-ld
OBJCOPY = $(RISCV_TOOLCHAIN)/bin/riscv32-unknown-elf-objcopy

TINYQV_SDK ?= ../../tinyQV-sdk

all: $(NAME).bin $(NAME).hex

clean:
	cd $(dir $(NAME)) && rm *.o *.elf *.bin *.hex

%.o: %.c
	$(CC) -O2 -I$(TINYQV_SDK) -march=rv32ec_zcb -mabi=ilp32e -mno-strict-align -nostdlib -nostartfiles -ffreestanding -ffunction-sections -fdata-sections -Wall -Werror -lc -c $< -o $@

%.o: %.s
	$(AS) -march=rv32ec_zcb -mabi=ilp32e $< -o $@

$(NAME).elf: $(PROJECT_SOURCES:.c=.o)
	$(LD) $^ $(TINYQV_SDK)/start.o $(TINYQV_SDK)/tinyQV.a $(RISCV_TOOLCHAIN)/riscv32-unknown-elf/lib/libc.a $(RISCV_TOOLCHAIN)/lib/gcc/riscv32-unknown-elf/*/libgcc.a  -T $(TINYQV_SDK)/memmap --gc-sections -o $@

$(NAME).bin: $(NAME).elf
	$(OBJCOPY) $< -O binary $@

$(NAME).hex: $(NAME).bin
	od -An -t x1 -w4 -v $< > $@
