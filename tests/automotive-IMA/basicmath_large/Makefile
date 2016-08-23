CC		:=	riscv64-unknown-elf-gcc
OBJDUMP := riscv64-unknown-elf-objdump --disassemble-all --disassemble-zeroes --section=.text --section=.text.startup --section=.data

TARGET	:= basicmath_large.c rad2deg.c cubic.c isqrt.c
GCC_OPTS = -m32 -Wa,-march=RV32IMA -std=gnu99 -msoft-float
LINK_OPTS = -m32 -nostartfiles -lc -lm
LIB_DIR	:=	-L../libac_sysc
LIBS	:=	-lc -lac_sysc
HAL		:=	../rv_hal/get_id.S
SRCS	:=

all:	$(TARGET)
	$(CC) -c ../rv_hal/crt.S -m32 -Wa,-march=RV32IM -std=gnu99 -msoft-float
	$(CC) $(TARGET) -o basicmath_large.run $(SRCS) $(HAL) $(LIB_DIR) $(LIBS) -T ../rv_hal/test.ld $(GCC_OPTS) $(LINK_OPTS)
	$(OBJDUMP) basicmath_large.run > basicmath_large.out

clean:
	rm basicmath_large.run crt.o basicmath_large.out
