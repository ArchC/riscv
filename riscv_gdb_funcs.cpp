/*
*
* @file        riscv_isa.cpp
* @version     1.0
*
*
* @date        May 2016
* @brief       The ArchC RISC-V functional model
*
*
*/

#include "riscv.H"

using namespace riscv_parms;

int riscv::nRegs(void) { return 33; }

ac_word riscv::reg_read(int reg) {
  if ((reg >= 0) && (reg < 32))
    return RB.read(reg);
  else if (reg == 32)
    return ac_pc;
}

void riscv::reg_write(int reg, ac_word value) {
  if ((reg >= 0) && (reg < 32))
    RB.write(reg, value);
  else if (reg == 32)
    ac_pc = value;
}

unsigned char riscv::mem_read(unsigned int address) {
  return DATA_PORT->read_byte(address);
}

void riscv::mem_write(unsigned int address, unsigned char byte) {
  DATA_PORT->write_byte(address, byte);
}
