/*************************************************
**************************************************
*	@file		test.ld
*	@author		Dário Dias
*	@email		dsd@cin.ufpe.br		
*	@version	0.20
*	@date		9 July 2016
*	@brief		The ArchC Risc-V functional model.
**************************************************
*************************************************/

/*======================================================================*/
/* Proxy kernel linker script                                           */
/*======================================================================*/
/* This is the linker script used when building the proxy kernel. */

/*----------------------------------------------------------------------*/
/* Setup                                                                */
/*----------------------------------------------------------------------*/

/* The OUTPUT_ARCH command specifies the machine architecture where the
   argument is one of the names used in the BFD library. More
   specifically one of the entires in bfd/cpu-mips.c */

OUTPUT_ARCH( "riscv" )

/* The ENTRY command specifies the entry point (ie. first instruction
   to execute). The symbol _start should be defined in each test. */

ENTRY( _start )

/*----------------------------------------------------------------------*/
/* Sections                                                             */
/*----------------------------------------------------------------------*/
_RAM_SIZE = 2M;
_RAM_START = 0x0000;
_RAM_END = _RAM_START + _RAM_SIZE;
SECTIONS
{


. = 0x100;
  .text : 
  {
    crt.o(.text)
    *(.text)
  }
  
  . = 0x20100;
  /* data segmemt */
  .data : { *(.data) }
  .sdata : { *(.sdata) }
  .rodata : { *(.rodata) }
  .sbss : { *(.sbss) }	
  .bss : { *(.bss) }
 
/* text: test code section */

  /* thread-local data segment */
  .tbss : { 
    crt.o(.tbss) /* Make sure tls_start is the first TLS symbol */
    *(.tbss)
  }
  .tdata : { *(.tdata) }
  /* End of uninitalized data segement */
 


 _end = .;

}

