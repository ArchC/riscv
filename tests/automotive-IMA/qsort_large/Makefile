CC		:=	riscv64-unknown-elf-gcc
OBJDUMP := riscv64-unknown-elf-objdump --disassemble-all --disassemble-zeroes --section=.text --section=.text.startup --section=.data

TARGET	:= qsort_large.c
GCC_OPTS = -m32 -Wa,-march=RV32IMA -std=gnu99 -msoft-float 
LINK_OPTS = -m32 -nostartfiles -lc -lm
LIB_DIR	:=	-L../libac_sysc
LIBS	:=	-lc -lac_sysc
HAL		:=	../rv_hal/get_id.S
SRCS	:=

all:	$(TARGET)
	$(CC) -c ../rv_hal/crt.S -m32 -Wa,-march=RV32IM -std=gnu99 -msoft-float
	$(CC) $(TARGET) -o qsort_large.run $(SRCS) $(HAL) $(LIB_DIR) $(LIBS) -T ../rv_hal/test.ld $(GCC_OPTS) $(LINK_OPTS)
	$(OBJDUMP) qsort_large.run > qsort_large.out

clean:
	rm qsort_large.run crt.o qsort_large.out
