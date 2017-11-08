# Use: make TARGET=<target architecture>
# It looks for the following commands for this target:
CC       = riscv32-unknown-elf-gcc
AR       = riscv32-unknown-elf-ar
RANLIB   = riscv32-unknown-elf-ranlib

# Flags
CFLAGS   = -O2 -g -I. -I.. -W -march=rv32imafd -std=gnu99 -mabi=ilp32d
ARFLAGS  = cr
RM       = rm
RMFLAGS  = -rf
TARGET   = riscv32-unknown-elf


ifneq (,$(findstring arm,$(TARGET)))
CFLAGS+=-DARM
endif

# Targets
LIBSYSC = libac_sysc.a

# Sources
LIBSYSC_SOURCES = $(wildcard *.c)

# Object files
LIBSYSC_OBJECTS = $(patsubst %.c,%.o,$(notdir $(LIBSYSC_SOURCES)))

# Target rules

all: $(LIBSYSC)

%.o: $(LIBSYSC_DIR)/%.c

$(LIBSYSC): $(LIBSYSC_OBJECTS)
	$(AR) $(ARFLAGS) $(LIBSYSC) $(LIBSYSC_OBJECTS)
	$(RANLIB) $(LIBSYSC)
	-$(RM) $(RMFLAGS) $(LIBSYSC_OBJECTS)

# Clean
clean:
	-$(RM) $(RMFLAGS) $(LIBSYSC) $(LIBSYSC_OBJECTS)
