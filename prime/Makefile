NAME=prime

TINYQV_SDK=../../tinyQV-sdk

all: $(NAME).bin $(NAME).hex

clean:
	rm *.o *.elf tmp.rbin *.bin

%.o: %.c
	riscv32-unknown-elf-gcc -O2 -I$(TINYQV_SDK) -march=rv32ec -mabi=ilp32e -mno-strict-align -nostdlib -nostartfiles -ffreestanding -ffunction-sections -fdata-sections -lc -c $< -o $@

%.o: %.s
	riscv32-unknown-elf-as -march=rv32ec -mabi=ilp32e $< -o $@

$(NAME).elf: main.o
	riscv32-unknown-elf-ld $^ $(TINYQV_SDK)/start.o /opt/riscv/riscv32-unknown-elf/lib/libc.a $(TINYQV_SDK)/tinyQV.a /opt/riscv/lib/gcc/riscv32-unknown-elf/12.2.0/libgcc.a  -T $(TINYQV_SDK)/memmap --gc-sections -o $@

$(NAME).bin: $(NAME).elf
	riscv32-unknown-elf-objcopy $< -O binary $@

$(NAME).hex: $(NAME).bin
	od -An -t x1 -w4 -v $< > $@
