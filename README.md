# RV32G Processor model

This is the functional model of RISC-V instruction set
architecture. It contains all the instructions necessary for running
Mibench automotive programs using the base integer model. This model
implements the following extensions:

  - Base integer instructions
  - Single-precision floating point instructions
  - Double-precision floating point instructions

## Installation

`````````
acsim riscv.ac -abi     (Create the simulator)
make                    (Compile it)
`````````

Use the official RISC-V toolchain for creating the executables. Follow
the steps on the RISC-V page to download the toolchain.

Configure GCC for RISC-V with the following options to enable 32-bit
code generation:
`````````
./configure --prefix=/opt/riscv --enable-multilib
`````````

## How to create executables

Follow the steps below:

  - Step 1:. Go to the tests->libac_sysc folder and run make command
via terminal.

  - Step 2:. Copy the folder of the program you wish to test from
test/acstone-programs, test/automotive-IMA or test/acstone-FP to
the test folder. Enter that folder and run make.

## Running the executable

To run a test program on the simulator, use the following command
line:
`````````
./riscv.x -- <path/to/the/executable>.run
`````````

## Future Work

The following topics need further improvement:

   - Double-precision floating point instructions need testing.

   - GDB: The RISC-V toolchain creates executables using DWARF version 4.
     Currently, ArchC is capable of retargeting GDB 6.4, which is
     quite outdated and can only read DWARF version 2. Therefore,
     acbingen at the official ArchC repo needs to be udpated in order
     to allow GDB debugging in this model.

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