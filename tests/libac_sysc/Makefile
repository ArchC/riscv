# Use: make TARGET=<target architecture>
# It looks for the following commands for this target:
CC       = riscv64-unknown-elf-gcc
AR       = riscv64-unknown-elf-ar
RANLIB   = riscv64-unknown-elf-ranlib

# Flags
CFLAGS   = -O2 -g -I. -I.. -m32 -Wa,-march=RV32IM -std=gnu99 -msoft-float 
ARFLAGS  = cr
RM       = rm
RMFLAGS  = -rf
TARGET   = riscv64-unknown-elf


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
