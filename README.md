# RV32G Processor model

This is the functional model of RISC-V instruction set
architecture. It contains all the instructions necessary for running
Mibench automotive programs using the base integer model. This model
implements the following extensions:

  - Base integer instructions
  - Single-precision floating point instructions
  - Double-precision floating point instructions 
  - Atomic instructions

## Installation

`````````
acsim riscv.ac -abi     (Create the simulator)
make                    (Compile it)
`````````

Use the official [RISC-V toolchain](https://github.com/riscv/riscv-gnu-toolchain/) for creating the executables. Follow
the steps on the RISC-V page to download the toolchain. Use the master branch when generating the toolchain binaries.

Configure GCC for RISC-V with the following options to enable 32-bit
code generation for general purpose riscv(use newlib support):
`````````
./configure --prefix=/opt/riscv --with-arch=rv32imafd --with-abi=ilp32d
make
`````````

## How to create executables

Follow the steps below:

  - Step 1:. Go to the tests->libac_sysc folder and run make command
via terminal.

  - Step 2:. Copy the folder of the program you wish to test from
test/acstone-programs, test/automotive-IMA or test/acstone-FP to
the test folder. Enter that folder and run make.

  - Use test/paranoia/Makefile as a starting point for compiling other programs.

## Running the executable

To run a test program on the simulator, use the following command
line:
`````````
./riscv.x -- <path/to/the/executable>.run
`````````

## Debugging

Generate the simulator with -gdb flag(i.e, acsim -abi -gdb) and use, for example:
`````````
./riscv.x -- 201.atomic.riscv
riscv32-unknown-elf-gdb 201.atomic.riscv
`````````

Inside gdb use:
`````````
target remote :5000
`````````

to connect to the riscv simulator.



## Future Work

The following topics need further improvement:

   - Atomic instructions needs multicore testing.
   - System instructions: CSRR* needs more testing.

Float and double instructions proved to be stable as confirmed by paranoia.

## References

I used the following as a reference for developing this model:

   - Mips processor model
   [https://github.com/ArchC/mips/tree/mips32r2]

## Code imported

This model uses the following third-party code:

   - riscv_syscall.cpp and riscv_syscall.H from Dario Soares' github repo
	[https://github.com/DarioSoares/riscv-archc]
   - Framework for creating ArchC-compatible ELF binaries developed by
     Dario Soares