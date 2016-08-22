/*************************************************
**************************************************
*	@file		riscv_syscall.cpp
*	@author		Dário Dias
*	@email		dsd@cin.ufpe.br		
*	@version	0.20
*	@date		8 July 2016
*	@brief		The ArchC Risc-V functional model.
**************************************************
*************************************************/

#include "riscv_syscall.H"

// 'using namespace' statement to allow access to all
// riscv-specific datatypes
using namespace riscv_parms;
unsigned procNumber = 0;

void riscv_syscall::get_buffer(int argn, unsigned char* buf, unsigned int size)
{
  unsigned int addr = RB[10+argn];

  for (unsigned int i = 0; i<size; i++, addr++) {
    buf[i] = DM.read_byte(addr);
  }
}

void riscv_syscall::set_buffer(int argn, unsigned char* buf, unsigned int size)
{
  unsigned int addr = RB[10+argn];

  for (unsigned int i = 0; i<size; i++, addr++) {
    DM.write_byte(addr, buf[i]);
  }
}

void riscv_syscall::set_buffer_noinvert(int argn, unsigned char* buf, unsigned int size)
{
  unsigned int addr = RB[10+argn];

  for (unsigned int i = 0; i<size; i+=4, addr+=4) {
    DM.write(addr, *(unsigned int *) &buf[i]);
  }
}

int riscv_syscall::get_int(int argn)
{
  return RB[10+argn];
}

void riscv_syscall::set_int(int argn, int val)
{
  RB[10+argn] = val;
}

void riscv_syscall::return_from_syscall()
{
  ac_pc = RB[1];
}

void riscv_syscall::set_prog_args(int argc, char **argv)
{
  int i, j, base;

  unsigned int ac_argv[30];
  char ac_argstr[512];

  base = AC_RAM_END - 512 - procNumber * 64 * 1024;
  for (i=0, j=0; i<argc; i++) {
    int len = strlen(argv[i]) + 1;
    ac_argv[i] = base + j;
    memcpy(&ac_argstr[j], argv[i], len);
    j += len;
  }

  RB[10] = base;
  set_buffer(0, (unsigned char*) ac_argstr, 512);   //$25 = $29(sp) - 4 (set_buffer adds 4)


  RB[10] = base - 120;
  set_buffer_noinvert(0, (unsigned char*) ac_argv, 120);

  //RB[4] = AC_RAM_END-512-128;

  //Set %o0 to the argument count
  RB[10] = argc;

  //Set %o1 to the string pointers
  RB[11] = base - 120;

  procNumber ++;
}
