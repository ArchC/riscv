# RV32G Processor model

This is the functional model of RISC-V instruction set architecture. It
contains all the instructions necessary for running mibench automotive programs using 
the base interger model. The model has following types of instruction encoding:

  - Integer Base instructions
  - Single-Precision floating point instructions 
  - Double-Precision floating point instructions

## Installation

For installing the processor model follow the below steps:

  - Step 1:. Download the RISC-V processor model in the processors folder(made 
             during the installation of ArchC) 
  - Step 2:. Perform the following commands on terminal: 
`````````
acsim riscv.ac -abi     (Create the simulator)     
make                    (Compile it) 
`````````

We require RISC-V toolchain for creating the executables. Follow the steps on 
the official RISC-V page to download the toolchain. 
Before building GCC you will need to run the following command:
`````````
./configure --prefix=/opt/riscv --enable-multilib
`````````
This will enable RV32G instruction encoding.

## How to create executables

Follow the below steps:

  - Step 1:. Go to the tests->libac_sysc folder and run make command
via terminal.
  - Step 2:. Copy the folder of the program you wish to test from 
test->acstone-programs or test->automotive-IMA or test->acstone-FP to the test 
folder. Enter that folder and run make via terminal.

## Running the executable

For running the test program on the model perform the following on the terminal:
````````` 
./riscv.x –load=<path/to/the/executable>.run  
`````````

## Future Work

Following things are left and need to be developed:

   - Double precision floating point instructions need testing.

## References

For developing my model I used the following as a reference:
   
   - Mips processor model
   [https://github.com/ArchC/mips/tree/mips32r2]

## Code Merged

The following codes were merged:
	

   - riscv_syscall.cpp and riscv_syscall.H from Dario Soares' github repo 
	[https://github.com/DarioSoares/riscv-archc]
   - Framework for creating the elfs has been developed by Dario Soares