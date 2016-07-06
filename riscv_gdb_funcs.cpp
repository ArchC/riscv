/*
* @file		riscv_gdb_funcs.cpp
* @version	1.0
*
*
* @date
* @brief 	The ArchC RISC-V functional model
*
*/

#include "riscv.H"

using namespace riscv_parms;

int riscv::nRegs(void){
	return 33;
}

ac_word riscv::reg_read(){
	if((reg>=0) && (reg<32))
		return RB.read(reg);
	else if(reg == 32)
		return ac_pc;
}

ac_word riscv::reg_write(){
	if((reg>=0) && (reg<32))
		RB.write(reg, value);
	else if(reg == 32)
		ac_pc = value;
}


unsigned char riscv::mem_read(){
	return IM->read_byte(address);
}

void riscv::mem_write(){
	IM->write_byte(adress, byte);
}
