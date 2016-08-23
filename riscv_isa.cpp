/*
*
* @file        riscv_isa.cpp
* @version     1.0
* @author      Pavani Tripathi
*
* Thanks to Dario Soares for contributing his model at
* https://github.com/DarioSoares/riscv-archc
*
* @date        Aug 2016
* @brief       The ArchC RISC-V functional model
*
* @Source      http://people.eecs.berkeley.edu/~krste/papers/riscv-spec-2.0.pdf
*
*/

#include "riscv_isa.H"
#include "riscv_isa_init.cpp"
#include "riscv_bhv_macros.H"

// Uncomment for debug Information
//#define DEBUG_MODEL
#include "ac_debug_model.H"

#define Ra 1
#define Sp 14

// For using all the RISC-V parameters
using namespace riscv_parms;

static int processors_started = 0;
#define DEFAULT_STACK_SIZE (512 * 1024);


// Generic instruction behavior method
void ac_behavior(instruction) {
  dbg_printf("---PC=%#x---%lld\n", (int)ac_pc, ac_instr_counter);
  ac_pc = ac_pc + 4;
}

// Instruction Format behavior methods
void ac_behavior(Type_R) {}
void ac_behavior(Type_R4) {}
void ac_behavior(Type_I) {}
void ac_behavior(Type_S) {}
void ac_behavior(Type_SB) {}
void ac_behavior(Type_U) {}
void ac_behavior(Type_UJ) {}


// Behavior called before starting simulation
void ac_behavior(begin) {
  dbg_printf("@@@ begin behavior @@@\n");

  for (int regNum = 0; regNum < 32; regNum++)
    {
      RB[regNum] = 0;
      RBF[regNum] = 0;
      RBF[regNum+32] = 0;
    }
  fcsr = 0;
}


// Behavior called after finishing simulation
void ac_behavior(end) {
  dbg_printf("@@@ end behavior @@@\n");
}

// Instruction ADD behavior method. (no check for overflow)
void ac_behavior(ADD) {
  dbg_printf("ADD r%d, r%d, r%d\n", rd, rs1, rs2);
  dbg_printf("RB[rs1] = %d\n", RB[rs1]);
  dbg_printf("RB[rs2] = %d\n", RB[rs2]);
  RB[rd] = RB[rs1] + RB[rs2];
  dbg_printf("Result = %d\n\n", RB[rd]);
}

// Instruction SUB behavior method. (no check for overflow)
void ac_behavior(SUB) {
  dbg_printf("SUB r%d, r%d, r%d\n", rd, rs1, rs2);
  dbg_printf("RB[rs1] = %d\n", RB[rs1]);
  dbg_printf("RB[rs2] = %d\n", RB[rs2]);
  RB[rd] = RB[rs1] - RB[rs2];
  dbg_printf("Result = %d\n\n", RB[rd]);
}

// Instruction SLL behavior method.
void ac_behavior(SLL) {
  dbg_printf("SLL r%d, r%d, r%d\n", rd, rs1, rs2);
  RB[rd] = RB[rs1] << RB[rs2];
  dbg_printf("RB[rs1] = %d\n", RB[rs1]);
  dbg_printf("RB[rs2] = %d\n", RB[rs2]);
  dbg_printf("Result = %d\n\n", RB[rd]);
}

// Instruction SLT behavior method.
void ac_behavior(SLT) {
  dbg_printf("SLT r%d, r%d, r%d\n", rd, rs1, rs2);
  if ((ac_Sword)RB[rs1] < (ac_Sword)RB[rs2])
    RB[rd] = 1;
  else
    RB[rd] = 0;
  dbg_printf("RB[rs1] = %d\n", RB[rs1]);
  dbg_printf("RB[rs2] = %d\n", RB[rs2]);
  dbg_printf("Result = %d\n\n", RB[rd]);
}

// Instruction SLTU behavior method.
void ac_behavior(SLTU) {
  dbg_printf("SLTU r%d, r%d, r%d\n", rd, rs1, rs2);
  if ((ac_Uword)RB[rs1] < (ac_Uword)RB[rs2])
    RB[rd] = 1;
  else
    RB[rd] = 0;
  dbg_printf("RB[rs1] = %#x\n", RB[rs1]);
  dbg_printf("RB[rs2] = %#x\n", RB[rs2]);
  dbg_printf("Result = %#x\n\n", RB[rd]);
}

// Instruction XOR behavior method.
void ac_behavior(XOR) {
  dbg_printf("XOR r%d, r%d, r%d\n", rd, rs1, rs2);
  RB[rd] = RB[rs1] ^ RB[rs2];
  dbg_printf("RB[rs1] = %#x\n", RB[rs1]);
  dbg_printf("RB[rs2] = %#x\n", RB[rs2]);
  dbg_printf("Result = %#x\n\n", RB[rd]);
}

// Instruction SRL behavior method.
void ac_behavior(SRL) {
  dbg_printf("SRL r%d, r%d, r%d\n", rd, rs1, rs2);
  RB[rd] = RB[rs1] >> RB[rs2];
  dbg_printf("RB[rs1] = %d\n", RB[rs1]);
  dbg_printf("RB[rs2] = %d\n", RB[rs2]);
  dbg_printf("Result = %d\n\n", RB[rd]);
}

// Instruction SRA behavior method.
void ac_behavior(SRA) {
  dbg_printf("SRA r%d, r%d, r%d\n", rd, rs1, rs2);
  RB[rd] = ((ac_Sword)RB[rs1]) >> RB[rs2];
  dbg_printf("RB[rs1] = %d\n", RB[rs1]);
  dbg_printf("RB[rs2] = %d\n", RB[rs2]);
  dbg_printf("Result = %d\n\n", RB[rd]);
}

// Instruction OR behavior method.
void ac_behavior(OR) {
  dbg_printf("OR r%d, r%d, r%d\n", rd, rs1, rs2);
  RB[rd] = RB[rs1] | RB[rs2];
  dbg_printf("RB[rs1] = %#x\n", RB[rs1]);
  dbg_printf("RB[rs2] = %#x\n", RB[rs2]);
  dbg_printf("Result = %#x\n\n", RB[rd]);
}

// Instruction AND behavior method.
void ac_behavior(AND) {
  dbg_printf("AND r%d, r%d, r%d\n", rd, rs1, rs2);
  RB[rd] = RB[rs1] & RB[rs2];
  dbg_printf("RB[rs1] = %#x\n", RB[rs1]);
  dbg_printf("RB[rs2] = %#x\n", RB[rs2]);
  dbg_printf("Result = %#x\n\n", RB[rd]);
}

// Instruction LB behavior method.
void ac_behavior(LB) {
  char byte;
  int offset;
  offset = (imm4 << 11) | (imm3 << 5) | (imm2 << 1) | imm1;
  dbg_printf("LB r%d, r%d, %d\n", rd, rs1, offset);
  int sign_ext;
  sign_ext = sign_extend(offset, 12);
  byte = DM.read_byte(RB[rs1] + sign_ext);
  RB[rd] = sign_extend(byte, 7);
  dbg_printf("RB[rs1] = %#x, byte = %#x\n", RB[rs1], byte);
  dbg_printf("addr = %#x\n", RB[rs1] + sign_ext);
  dbg_printf("Result = %#x\n\n", RB[rd]);
}

// Instruction LH behavior method.
void ac_behavior(LH) {
  short int half;
  int offset;
  offset = (imm4 << 11) | (imm3 << 5) | (imm2 << 1) | imm1;
  dbg_printf("LH r%d, r%d, %d\n", rd, rs1, offset);
  int sign_ext;
  sign_ext = sign_extend(offset, 12);
  half = DM.read_half(RB[rs1] + sign_ext);
  RB[rd] = sign_extend(half, 16);
  dbg_printf("RB[rs1] = %#x, half = %#x\n", RB[rs1], half);
  dbg_printf("addr = %#x\n", RB[rs1] + sign_ext);
  dbg_printf("Result = %#x\n\n", RB[rd]);
}

// Instruction LW behavior method.
void ac_behavior(LW) {
  int offset;
  offset = (imm4 << 11) | (imm3 << 5) | (imm2 << 1) | imm1;
  dbg_printf("LW r%d, r%d, %d\n", rd, rs1, offset);
  int sign_ext;
  sign_ext = sign_extend(offset, 12);
  RB[rd] = DM.read(RB[rs1] + sign_ext);
  dbg_printf("RB[rs1] = %#x\n", RB[rs1]);
  dbg_printf("addr = %#x\n", RB[rs1] + sign_ext);
  dbg_printf("Result = %#x\n\n", RB[rd]);
}

// Instruction LBU behavior method.
void ac_behavior(LBU) {
  int offset;
  offset = (imm4 << 11) | (imm3 << 5) | (imm2 << 1) | imm1;
  dbg_printf("LBU r%d, r%d, %d\n", rd, rs1, offset);
  int sign_ext;
  sign_ext = sign_extend(offset, 12);
  RB[rd] = DM.read_byte(RB[rs1] + sign_ext);
  dbg_printf("RB[rs1] = %#x\n", RB[rs1]);
  dbg_printf("addr = %#x\n", RB[rs1] + sign_ext);
  dbg_printf("Result = %#x\n\n", RB[rd]);
}

// Instruction LHU behavior method.
void ac_behavior(LHU) {
  int offset;
  offset = (imm4 << 11) | (imm3 << 5) | (imm2 << 1) | imm1;
  dbg_printf("LHU r%d, r%d, %d\n", rd, rs1, offset);
  int sign_ext;
  sign_ext = sign_extend(offset, 12);
  RB[rd] = DM.read_half(RB[rs1] + sign_ext);
  dbg_printf("RB[rs1] = %#x\n", RB[rs1]);
  dbg_printf("addr = %#x\n", RB[rs1] + sign_ext);
  dbg_printf("Result = %#x\n\n", RB[rd]);
}

// Instruction ADDI behavior method.
void ac_behavior(ADDI) {
  int imm;
  imm = (imm4 << 11) | (imm3 << 5) | (imm2 << 1) | imm1;
  dbg_printf("ADDI r%d, r%d, %d\n", rd, rs1, imm);
  if ((rd == 0) && (rs1 == 0) && (imm == 0)) {
    dbg_printf("NOP executed!");
  } else {
    int sign_ext;
    sign_ext = sign_extend(imm, 12);
    RB[rd] = RB[rs1] + sign_ext;
    dbg_printf("RB[rs1] = %d\n", RB[rs1]);
    dbg_printf("imm = %d\n", sign_ext);
    dbg_printf("Result = %d\n\n", RB[rd]);
  }
}

// Instruction SLTI behavior method.
void ac_behavior(SLTI) {
  int imm;
  imm = (imm4 << 11) | (imm3 << 5) | (imm2 << 1) | imm1;
  dbg_printf("SLTI r%d, r%d, %d\n", rd, rs1, imm);
  int sign_ext;
  sign_ext = sign_extend(imm, 12);
  if (RB[rs1] < sign_ext)
    RB[rd] = 1;
  else
    RB[rd] = 0;
  dbg_printf("RB[rs1] = %d\n", RB[rs1]);
  dbg_printf("imm = %d\n", sign_ext);
  dbg_printf("Result = %d\n\n", RB[rd]);
}

// Instruction SLTIU behavior method.
void ac_behavior(SLTIU) {
  int imm;
  imm = (imm4 << 11) | (imm3 << 5) | (imm2 << 1) | imm1;
  dbg_printf("SLTIU r%d, r%d, %d\n", rd, rs1, imm);
  int sign_ext;
  sign_ext = sign_extend(imm, 12);
  if ((ac_Uword)RB[rs1] < (ac_Uword)sign_ext)
    RB[rd] = 1;
  else
    RB[rd] = 0;
  dbg_printf("Result = %#x\n\n", RB[rd]);
}

// Instruction XORI behavior method.
void ac_behavior(XORI) {
  int imm;
  imm = (imm4 << 11) | (imm3 << 5) | (imm2 << 1) | imm1;
  dbg_printf("XORI r%d, r%d, %d\n", rd, rs1, imm);
  int sign_ext;
  sign_ext = sign_extend(imm, 12);
  RB[rd] = RB[rs1] ^ sign_ext;
  dbg_printf("Result = %#x\n\n", RB[rd]);
}

// Instruction ORI behavior method.
void ac_behavior(ORI) {
  int imm;
  imm = (imm4 << 11) | (imm3 << 5) | (imm2 << 1) | imm1;
  dbg_printf("ORI r%d, r%d, %d\n", rd, rs1, imm);
  int sign_ext;
  sign_ext = sign_extend(imm, 12);
  RB[rd] = RB[rs1] | sign_ext;
  dbg_printf("Result = %#x\n\n", RB[rd]);
}

// Instruction ANDI behavior method.
void ac_behavior(ANDI) {
  int imm;
  imm = (imm4 << 11) | (imm3 << 5) | (imm2 << 1) | imm1;
  dbg_printf("ANDI r%d, r%d, %d\n", rd, rs1, imm);
  int sign_ext;
  sign_ext = sign_extend(imm, 12);
  RB[rd] = RB[rs1] & sign_ext;
  dbg_printf("Result = %#x\n\n", RB[rd]);
}

// Instruction JALR behavior method.
void ac_behavior(JALR) {
  int target_addr;
  int imm;
  imm = (imm4 << 11) | (imm3 << 5) | (imm2 << 1) | imm1;
  dbg_printf("JALR r%d, r%d, %d\n", rd, rs1, imm);
  int sign_ext;
  sign_ext = sign_extend(imm, 12);
  target_addr = RB[rs1] + sign_ext;
  target_addr = target_addr >> 1;
  target_addr = target_addr << 1;
  if (rd != 0)
    RB[rd] = ac_pc;
  ac_pc = target_addr;
  dbg_printf("Target = %#x\n", (ac_pc & 0xF0000000) | target_addr);
  dbg_printf("Target = %#x\n", target_addr);
  dbg_printf("Return = %#x\n\n", RB[rd]);
}

// Instruction SLLI behavior method.
void ac_behavior(SLLI) {
  short int shamt;
  shamt = (imm2 << 1) | imm1;
  dbg_printf("SLLI r%d, r%d, %d\n", rd, rs1, shamt);
  RB[rd] = RB[rs1] << shamt;
  dbg_printf("shamt = %d\n", shamt);
  dbg_printf("Result = %#x\n\n", RB[rd]);
}

// Instruction SRLI behavior method.
void ac_behavior(SRLI) {
  short int shamt;
  shamt = (imm2 << 1) | imm1;
  dbg_printf("SRLI r%d, r%d, %d\n", rd, rs1, shamt);
  RB[rd] = RB[rs1] >> shamt;
  dbg_printf("shamt = %d\n", shamt);
  dbg_printf("Result = %#x\n\n", RB[rd]);
}

// Instruction SRAI behavior method.
void ac_behavior(SRAI) {
  short int shamt;
  shamt = (imm2 << 1) | imm1;
  dbg_printf("SRAI r%d, r%d, %d\n", rd, rs1, shamt);
  if ((RB[rs1] >> 31) == 1)
    RB[rd] = (RB[rs1] >> shamt) | (0xFFFFFFFF << (32 - shamt));
  else
    RB[rd] = RB[rs1] >> shamt;
  dbg_printf("shamt = %d\n", shamt);
  dbg_printf("Result = %#x\n\n", RB[rd]);
}

// Instruction SCALL behavior method.
void ac_behavior(SCALL) {
  dbg_printf("SCALL\n");
  printf("System Call\n\n");
  stop();
}

// Instruction SBREAK behavior method.
void ac_behavior(SBREAK) {
  dbg_printf("SBREAK\n");
  printf("Breakpoint\n\n");
  stop();
}

// Instruction RDCYCLE behavior method.
void ac_behavior(RDCYCLE) {
  dbg_printf("RDCYCLE r%d\n", rd);
  RB[rd] = ac_pc;
  dbg_printf("Result = %#x\n", RB[rd]);
}

// Instruction RDCYCLEH behavior method.
void ac_behavior(RDCYCLEH) { dbg_printf("RDCYCLEH r%d\n", rd); }

// Instruction RDTIME behavior method.
void ac_behavior(RDTIME) { dbg_printf("RDTIME r%d\n", rd); }

// Instruction RDTIMEH behavior method.
void ac_behavior(RDTIMEH) { dbg_printf("RDTIMEH r%d\n", rd); }

// Instruction RDINSTRET behavior method.
void ac_behavior(RDINSTRET) { dbg_printf("RDINSTRET r%d\n", rd); }

// Instruction RDINSTRETH behavior method.
void ac_behavior(RDINSTRETH) { dbg_printf("RDINSTRETH r%d\n", rd); }

// Instruction SB behavior method
void ac_behavior(SB) {
  int imm;
  imm = (imm4 << 11) | (imm3 << 5) | (imm2 << 1) | imm1;
  dbg_printf("SB r%d, r%d, %d\n", rs1, rs2, imm);
  unsigned char byte = RB[rs2] & 0xFF;
  int sign_ext;
  sign_ext = sign_extend(imm, 12);
  DM.write_byte(RB[rs1] + sign_ext, byte);
  dbg_printf("addr: %#x\n", RB[rs1] + sign_ext);
  dbg_printf("Result: %#x\n\n\n", byte);
}

// Instruction SH behavior method
void ac_behavior(SH) {
  int imm;
  imm = (imm4 << 11) | (imm3 << 5) | (imm2 << 1) | imm1;
  dbg_printf("SH r%d, r%d, %d\n", rs1, rs2, imm);
  int sign_ext;
  sign_ext = sign_extend(imm, 12);
  unsigned short int half = RB[rs2] & 0xFFFF;
  DM.write_half(RB[rs1] + sign_ext, half);
  dbg_printf("addr: %#x\n", RB[rs1] + sign_ext);
  dbg_printf("Result: %#x\n\n\n", half);
}

// Instruction SW behavior method
void ac_behavior(SW) {
  int imm;
  imm = (imm4 << 11) | (imm3 << 5) | (imm2 << 1) | imm1;
  dbg_printf("SW r%d, r%d, %d\n", rs1, rs2, imm);
  int sign_ext;
  sign_ext = sign_extend(imm, 12);
  DM.write(RB[rs1] + sign_ext, RB[rs2]);
  dbg_printf("addr: %d\n\n", RB[rs1] + sign_ext);
}

// Instruction BEQ behavior method
void ac_behavior(BEQ) {
  int imm;
  imm = (imm4 << 11) | (imm3 << 10) | (imm2 << 4) | imm1;
  dbg_printf("BEQ r%d, r%d, %d\n", rs1, rs2, imm);
  unsigned int addr;
  if (imm4 == 1)
    addr = ac_pc + ((imm << 1) | 0xFFFFF000) - 4;
  else
    addr = ac_pc + (imm << 1) - 4;
  if (RB[rs1] == RB[rs2]) {
    ac_pc = addr;
    dbg_printf("---Branch Taken--- to %#x\n\n", addr);
  } else
    dbg_printf("---Branch not Taken---\n\n");
}

// Instruction BNE behavior method
void ac_behavior(BNE) {
  int imm;
  imm = (imm4 << 11) | (imm3 << 10) | (imm2 << 4) | imm1;
  dbg_printf("BNE r%d, r%d, %d\n", rs1, rs2, imm);
  unsigned int addr;
  if (imm4 == 1)
    addr = ac_pc + ((imm << 1) | 0xFFFFF000) - 4;
  else
    addr = ac_pc + (imm << 1) - 4;
  if (RB[rs1] != RB[rs2]) {
    ac_pc = addr;
    dbg_printf("---Branch Taken---\n\n");
  } else
    dbg_printf("---Branch not Taken---\n\n");
}

// Instruction BLT behavior method
void ac_behavior(BLT) {
  int imm;
  imm = (imm4 << 11) | (imm3 << 10) | (imm2 << 4) | imm1;
  dbg_printf("BLT r%d, r%d, %d\n", rs1, rs2, imm);
  unsigned int addr;
  if (imm4 == 1)
    addr = ac_pc + ((imm << 1) | 0xFFFFF000) - 4;
  else
    addr = ac_pc + (imm << 1) - 4;
  dbg_printf("blt r%d, r%d, %#x\n", rs1, rs2, addr);
  dbg_printf("addr = %#x\n", addr);
  dbg_printf("rs1 = %#x\n", RB[rs1]);
  dbg_printf("rs2 = %#x\n", RB[rs2]);
  if ((ac_Sword)RB[rs1] < (ac_Sword)RB[rs2]) {
    ac_pc = addr;
    dbg_printf("---Branch Taken---\n\n");
  } else
    dbg_printf("---Branch not Taken---\n\n");
}

// Instruction BGE behavior method
void ac_behavior(BGE) {
  int imm;
  imm = (imm4 << 11) | (imm3 << 10) | (imm2 << 4) | imm1;
  dbg_printf("BGE r%d, r%d, %d\n", rs1, rs2, imm);
  unsigned int addr;
  if (imm4 == 1)
    addr = ac_pc + ((imm << 1) | 0xFFFFF000) - 4;
  else
    addr = ac_pc + (imm << 1) - 4;
  if ((ac_Sword)RB[rs1] >= (ac_Sword)RB[rs2]) {
    ac_pc = addr;
    dbg_printf("---Branch Taken---\n\n");
  } else
    dbg_printf("---Branch not Taken---\n\n");
}

// Instruction BLTU behavior method
void ac_behavior(BLTU) {
  int imm;
  imm = (imm4 << 11) | (imm3 << 10) | (imm2 << 4) | imm1;
  dbg_printf("BLTU r%d, r%d, %d\n", rs1, rs2, imm);
  unsigned int addr;
  if (imm4 == 1)
    addr = ac_pc + ((imm << 1) | 0xFFFFF000) - 4;
  else
    addr = ac_pc + (imm << 1) - 4;
  if ((ac_Uword)RB[rs1] < (ac_Uword)RB[rs2]) {
    ac_pc = addr;
    dbg_printf("---Branch Taken---\n\n");
  } else
    dbg_printf("---Branch not Taken---\n\n");
}

// Instruction BGEU behavior method
void ac_behavior(BGEU) {
  int imm;
  imm = (imm4 << 11) | (imm3 << 10) | (imm2 << 4) | imm1;
  dbg_printf("BGEU r%d, r%d, %d\n", rs1, rs2, imm);
  int addr;
  if (imm4 == 1)
    addr = ac_pc + ((imm << 1) | 0xFFFFF000) - 4;
  else
    addr = ac_pc + (imm << 1) - 4;
  if (((ac_Uword)RB[rs1] > (ac_Uword)RB[rs2]) ||
      ((ac_Uword)RB[rs1] == (ac_Uword)RB[rs2])) {
    ac_pc = addr;
    dbg_printf("---Branch Taken---\n\n");
  } else
    dbg_printf("---Branch not Taken---\n\n");
}

// Instruction LUI behavior method
void ac_behavior(LUI) {
  dbg_printf("LUI r%d, %d\n", rd, imm);
  RB[rd] = imm << 12;
  dbg_printf("Result = %#x\n\n", RB[rd]);
}

// Instruction AUIPC behavior method
void ac_behavior(AUIPC) {
  dbg_printf("AUIPC r%d, %d\n", rd, imm);
  unsigned int offset;
  offset = imm << 12;
  RB[rd] = ac_pc + offset - 4;
  dbg_printf("Result = %#x\n\n", RB[rd]);
}

// Instruction JAL behavior method
void ac_behavior(JAL) {
  int imm;
  imm = (imm4 << 19) | (imm3 << 11) | (imm2 << 10) | imm1;
  dbg_printf("JAL r%d, %d\n", rd, imm);
  int addr;
  if (imm4 == 1)
    addr = ac_pc + ((imm << 1) | 0xFFF00000) - 4;
  else
    addr = ac_pc + (imm << 1) - 4;
  if (rd != 0)
    RB[rd] = ac_pc;
  ac_pc = (ac_pc & 0xF0000000) | addr;
  dbg_printf("--- Jump taken ---\n\n");
}

// Instruction MUL behavior method
void ac_behavior(MUL) {
  dbg_printf("MUL r%d, r%d, r%d\n", rd, rs1, rs2);
  dbg_printf("RB[rs1] = %d\n", RB[rs1]);
  dbg_printf("RB[rs2] = %d\n", RB[rs2]);
  long long mult;
  mult = (ac_Sword)RB[rs1];
  mult *= (ac_Sword)RB[rs2];
  int half;
  half = mult;
  RB[rd] = half;
  dbg_printf("Result = %#x\n\n", RB[rd]);
}

// Instruction MULH behavior method
void ac_behavior(MULH) {
  dbg_printf("MULH r%d, r%d, r%d\n", rd, rs1, rs2);
  dbg_printf("RB[rs1] = %d\n", RB[rs1]);
  dbg_printf("RB[rs2] = %d\n", RB[rs2]);
  long long mult;
  int half;
  mult = (ac_Sword)RB[rs1];
  mult *= (ac_Sword)RB[rs2];
  half = mult >> 32;
  RB[rd] = half;
  dbg_printf("Result = %#x\n\n", RB[rd]);
}

// Instruction MULHSU behavior method
void ac_behavior(MULHSU) {
  dbg_printf("MULHSU r%d, r%d, r%d\n", rd, rs1, rs2);
  dbg_printf("RB[rs1] = %d\n", RB[rs1]);
  dbg_printf("RB[rs2] = %d\n", RB[rs2]);
  long long mult;
  int half;
  mult = RB[rs1];
  mult *= (ac_Uword)RB[rs2];
  half = mult >> 32;
  RB[rd] = half;
  dbg_printf("Result = %#x\n\n", RB[rd]);
}

// Instruction MULHU behavior method
void ac_behavior(MULHU) {
  dbg_printf("MULHU r%d, r%d, r%d\n", rd, rs1, rs2);
  dbg_printf("RB[rs1] = %d\n", RB[rs1]);
  dbg_printf("RB[rs2] = %d\n", RB[rs2]);
  unsigned long long mult;
  unsigned int half;
  mult = (ac_Uword)RB[rs1];
  mult *= (ac_Uword)RB[rs2];
  half = mult >> 32;
  RB[rd] = half;
  dbg_printf("Result = %d\n\n", RB[rd]);
}

// Instruction DIV behavior method
void ac_behavior(DIV) {
  dbg_printf("DIV r%d, r%d, r%d\n", rd, rs1, rs2);
  dbg_printf("rs1 = %d\n", RB[rs1]);
  dbg_printf("rs2 = %d\n", RB[rs2]);
  if (RB[rs2] == 0)
    RB[rd] = -1;
  else if ((RB[rs1] == ((-2) ^ 31)) && (RB[rs2] == -1))
    RB[rd] = ((-2) ^ 31);
  else
    RB[rd] = (ac_Sword)RB[rs1] / (ac_Sword)RB[rs2];
  dbg_printf("Result = %d\n\n", RB[rd]);
}

// Instruction DIVU behavior method
void ac_behavior(DIVU) {
  dbg_printf("DIVU r%d, r%d, r%d\n", rd, rs1, rs2);
  dbg_printf("RB[rs1] = %d\n", RB[rs1]);
  dbg_printf("RB[rs2] = %d\n", RB[rs2]);
  if (RB[rs2] == 0)
    RB[rd] = -1;
  else if ((RB[rs1] == ((-2) ^ 31)) && (RB[rs2] == -1))
    dbg_printf("Wrong division exception!");
  else
    RB[rd] = (ac_Uword)RB[rs1] / (ac_Uword)RB[rs2];
  dbg_printf("Result = %#x\n\n", (ac_Uword)RB[rd]);
}

// Instruction REM behavior method
void ac_behavior(REM) {
  dbg_printf("REM r%d, r%d, r%d\n", rd, rs1, rs2);
  dbg_printf("RB[rs1] = %d\n", RB[rs1]);
  dbg_printf("RB[rs2] = %d\n", RB[rs2]);
  if (RB[rs2] == 0)
    RB[rd] = RB[rs1];
  else if ((RB[rs1] == ((-2) ^ 31)) && (RB[rs2] == -1))
    RB[rd] = 0;
  else
    RB[rd] = RB[rs1] % RB[rs2];
  dbg_printf("Result = %#x\n\n", RB[rd]);
}

// Instruction REMU behavior method
void ac_behavior(REMU) {
  dbg_printf("REMU r%d, r%d, r%d\n", rd, rs1, rs2);
  dbg_printf("RB[rs1] = %d\n", RB[rs1]);
  dbg_printf("RB[rs2] = %d\n", RB[rs2]);
  if (RB[rs2] == 0)
    RB[rd] = RB[rs1];
  else if ((RB[rs1] == ((-2) ^ 31)) && (RB[rs2] == -1))
    dbg_printf("Wrong division exception!");
  else
    RB[rd] = (ac_Uword)RB[rs1] % (ac_Uword)RB[rs2];
  dbg_printf("Result = %#x\n\n", (ac_Uword)RB[rd]);
}

// Instruction LR.W behavior method
void ac_behavior(LR_W) { RB[rd] = DM.read(RB[rs1]); }

// Instruction SC.w behavior method
void ac_behavior(SC_W) {
  DM.write(RB[rs1], RB[rs2]);
  RB[rd] = 0; // indicating success
}

// Instruction AMOSWAP.W behavior method
void ac_behavior(AMOSWAP_W) {
  dbg_printf("AMOSWAP.W r%d, r%d, r%d\n", rd, rs1, rs2);
  RB[rd] = DM.read(RB[rs1]);
  dbg_printf("RB[rd] = %d\n", RB[rd]);
  dbg_printf("RB[rs2] = %d\n", RB[rs2]);
  int temp;
  temp = RB[rs2];
  RB[rs2] = RB[rd];
  RB[rd] = temp;
  dbg_printf("After swapping RB[rd] = %d\n\n", RB[rd]);
  DM.write(RB[rs1], RB[rd]);
}

// Instruction AMOADD.W behavior method
void ac_behavior(AMOADD_W) {
  dbg_printf("AMOADD.W r%d, r%d, r%d\n", rd, rs1, rs2);
  RB[rd] = DM.read(RB[rs1]);
  dbg_printf("RB[rd] = %d\n", RB[rd]);
  dbg_printf("RB[rs2] = %d\n", RB[rs2]);
  DM.write(RB[rs1], (RB[rd] + RB[rs2]));
  dbg_printf("Result = %d\n\n", RB[rd] + RB[rs2]);
}

// Instruction AMOXOR.W behavior method
void ac_behavior(AMOXOR_W) {
  dbg_printf("AMOXOR.W r%d, r%d, r%d\n", rd, rs1, rs2);
  RB[rd] = DM.read(RB[rs1]);
  dbg_printf("RB[rd] = %d\n", RB[rd]);
  dbg_printf("RB[rs2] = %d\n", RB[rs2]);
  DM.write(RB[rs1], (RB[rd] ^ RB[rs2]));
  dbg_printf("Result = %d\n\n", RB[rd] ^ RB[rs2]);
}

// Instruction AMOAND.W behavior method
void ac_behavior(AMOAND_W) {
  dbg_printf("AMOAND.W r%d, r%d, r%d\n", rd, rs1, rs2);
  RB[rd] = DM.read(RB[rs1]);
  dbg_printf("RB[rd] = %d\n", RB[rd]);
  dbg_printf("RB[rs2] = %d\n", RB[rs2]);
  DM.write(RB[rs1], (RB[rd] & RB[rs2]));
  dbg_printf("Result = %d\n\n", RB[rd] & RB[rs2]);
}

// Instruction AMOOR.W behavior method
void ac_behavior(AMOOR_W) {
  dbg_printf("AMOOR.W r%d, r%d, r%d\n", rd, rs1, rs2);
  RB[rd] = DM.read(RB[rs1]);
  dbg_printf("RB[rd] = %d\n", RB[rd]);
  dbg_printf("RB[rs2] = %d\n", RB[rs2]);
  DM.write(RB[rs1], (RB[rd] | RB[rs2]));
  dbg_printf("Result = %d\n\n", RB[rd] | RB[rs2]);
}

// Instruction AMOMIN.W behavior method
void ac_behavior(AMOMIN_W) {
  dbg_printf("AMOMIN.W r%d, r%d, r%d\n", rd, rs1, rs2);
  RB[rd] = DM.read(RB[rs1]);
  dbg_printf("RB[rd] = %d\n", RB[rd]);
  dbg_printf("RB[rs2] = %d\n", RB[rs2]);
  if (RB[rd] < RB[rs2])
    DM.write(RB[rs1], RB[rd]);
  else
    DM.write(RB[rs1], RB[rs2]);
}

// Instruction AMOMAX.W behavior method
void ac_behavior(AMOMAX_W) {
  dbg_printf("AMOMAX.W r%d, r%d, r%d\n", rd, rs1, rs2);
  RB[rd] = DM.read(RB[rs1]);
  dbg_printf("RB[rd] = %d\n", RB[rd]);
  dbg_printf("RB[rs2] = %d\n\n", RB[rs2]);
  if (RB[rd] > RB[rs2])
    DM.write(RB[rs1], RB[rd]);
  else
    DM.write(RB[rs1], RB[rs2]);
}

// Instruction AMOMINU.W behavior method
void ac_behavior(AMOMINU_W) {
  dbg_printf("AMOMINU.W r%d, r%d, r%d\n", rd, rs1, rs2);
  RB[rd] = DM.read(RB[rs1]);
  dbg_printf("RB[rd] = %d\n", RB[rd]);
  dbg_printf("RB[rs2] = %d\n\n", RB[rs2]);
  if ((ac_Uword)RB[rd] < (ac_Uword)RB[rs2])
    DM.write(RB[rs1], RB[rd]);
  else
    DM.write(RB[rs1], RB[rs2]);
}

// Instruction AMOMAXU.W behavior method
void ac_behavior(AMOMAXU_W) {
  dbg_printf("AMOMAXU.W r%d, r%d, r%d\n", rd, rs1, rs2);
  RB[rd] = DM.read(RB[rs1]);
  dbg_printf("RB[rd] = %d\n", RB[rd]);
  dbg_printf("RB[rs2] = %d\n", RB[rs2]);
  if ((ac_Uword)RB[rd] > (ac_Uword)RB[rs2])
    DM.write(RB[rs1], RB[rd]);
  else
    DM.write(RB[rs1], RB[rs2]);
}

// Instruction FLW behavior method
void ac_behavior(FLW) {
  int offset;
  offset = (imm4 << 11) | (imm3 << 5) | (imm2 << 1) | imm1;
  dbg_printf("FLW r%d, r%d, %d\n", rd, rs1, offset);
  int sign_ext;
  sign_ext = sign_extend(offset, 12);
  RBF[rd] = DM.read(RB[rs1] + sign_ext);
  dbg_printf("RB[rs1] = %#x\n", RB[rs1]);
  dbg_printf("addr = %#x\n", RB[rs1] + sign_ext);
  dbg_printf("Result = %.3f\n\n", (float)RBF[rd]);
}

// Instruction FSW behavior method
void ac_behavior(FSW) {
  int imm;
  imm = (imm4 << 11) | (imm3 << 5) | (imm2 << 1) | imm1;
  dbg_printf("FSW r%d, r%d, %d\n", rs1, rs2, imm);
  int sign_ext;
  sign_ext = sign_extend(imm, 12);
  DM.write(RB[rs1] + sign_ext, RBF[rs2]);
  dbg_printf("addr: %d\n\n", RB[rs1] + sign_ext);
}

// Instruction FADD.S behavior method
void ac_behavior(FADD_S) {
  dbg_printf("FADD.S r%d, r%d, r%d\n", rd, rs1, rs2);
  dbg_printf("RBF[rs1] = %.3f\n", load_float(rs1));
  dbg_printf("RBF[rs2] = %.3f\n", load_float(rs2));
  float sum;
  sum = load_float(rs1) + load_float(rs2);
  save_float(sum, rd);
  dbg_printf("Result = %.3f\n\n", sum);
}

// Instruction FSUB.S behavior method
void ac_behavior(FSUB_S) {
  dbg_printf("FSUB.S r%d, r%d, r%d\n", rd, rs1, rs2);
  dbg_printf("RBF[rs1] = %.3f\n", load_float(rs1));
  dbg_printf("RBF[rs2] = %.3f\n", load_float(rs2));
  float diff;
  diff = load_float(rs1) - load_float(rs2);
  save_float(diff, rd);
  dbg_printf("Result = %.3f\n\n", diff);
}

// Instruction FMUL.S behavior method
void ac_behavior(FMUL_S) {
  dbg_printf("FMUL.S r%d, r%d, r%d\n", rd, rs1, rs2);
  dbg_printf("RBF[rs1] = %.3f\n", load_float(rs1));
  dbg_printf("RBF[rs2] = %.3f\n", load_float(rs2));
  float product;
  product = load_float(rs1) * load_float(rs2);
  save_float(product, rd);
  dbg_printf("Result = %.3f\n\n", product);
}

// Instruction FDIV.S behavior method
void ac_behavior(FDIV_S) {
  dbg_printf("FDIV.S r%d, r%d, r%d\n", rd, rs1, rs2);
  dbg_printf("RBF[rs1] = %.3f\n", load_float(rs1));
  dbg_printf("RBF[rs2] = %.3f\n", load_float(rs2));
  float quot;
  quot = load_float(rs1) / load_float(rs2);
  save_float(quot, rd);
  dbg_printf("Result = %.3f\n\n", quot);
}

// Instruction FMIN.S behavior method
void ac_behavior(FMIN_S) {
  dbg_printf("FMIN.S r%d, r%d, r%d\n", rd, rs1, rs2);
  dbg_printf("RBF[rs1] = %.3f\n", load_float(rs1));
  dbg_printf("RBF[rs2] = %.3f\n", load_float(rs2));
  float temp;
  if (load_float(rs1) < load_float(rs2)) {
    temp = load_float(rs1);
    save_float(temp, rd);
  } else {
    temp = load_float(rs2);
    save_float(temp, rd);
  }
  dbg_printf("Result = %.3f\n\n", temp);
}

// Instruction FMAX.S behavior method
void ac_behavior(FMAX_S) {
  dbg_printf("FMAX.S r%d, r%d, r%d\n", rd, rs1, rs2);
  dbg_printf("RBF[rs1] = %.3f\n", load_float(rs1));
  dbg_printf("RBF[rs2] = %.3f\n", load_float(rs2));
  float temp;
  if (load_float(rs1) > load_float(rs2)) {
    temp = load_float(rs1);
    save_float(temp, rd);
  } else {
    temp = load_float(rs2);
    save_float(temp, rd);
  }
  dbg_printf("Result = %.3f\n\n", temp);
}

// Instruction FSQRT.S behavior method
void ac_behavior(FSQRT_S) {
  dbg_printf("FSQRT.S r%d, r%d\n", rd, rs1);
  dbg_printf("RBF[rs1] = %.3f\n", load_float(rs1));
  float temp;
  if (load_float(rs1) < 0) {
    dbg_printf("Invalid!");
    stop();
  } else
    RBF[rd] = sqrt(RBF[rs1]);
  dbg_printf("Result = %.3f\n\n", (float)RBF[rd]);
}

// Instruction FMADD.S behavior method
void ac_behavior(FMADD_S) {
  dbg_printf("FMADD.S r%d, r%d, r%d, r%d\n", rd, rs1, rs2, rs3);
  dbg_printf("RBF[rs1] = %.3f\n", load_float(rs1));
  dbg_printf("RBF[rs2] = %.3f\n", load_float(rs2));
  dbg_printf("RBF[rs2] = %.3f\n", load_float(rs3));
  float res;
  res = load_float(rs1) * load_float(rs2) + load_float(rs3);
  save_float(res, rd);
  dbg_printf("Result = %.3f\n\n", res);
}

// Instruction FMSUB.S behavior method
void ac_behavior(FMSUB_S) {
  dbg_printf("FMSUB.S r%d, r%d, r%d, r%d\n", rd, rs1, rs2, rs3);
  dbg_printf("RBF[rs1] = %.3f\n", load_float(rs1));
  dbg_printf("RBF[rs2] = %.3f\n", load_float(rs2));
  dbg_printf("RBF[rs2] = %.3f\n", load_float(rs3));
  float res;
  res = load_float(rs1) * load_float(rs2) - load_float(rs3);
  save_float(res, rd);
  dbg_printf("Result = %.3f\n\n", res);
}

// Instruction FNMSUB.S behavior method
void ac_behavior(FNMSUB_S) {
  dbg_printf("FNMSUB.S r%d, r%d, r%d, r%d\n", rd, rs1, rs2, rs3);
  dbg_printf("RBF[rs1] = %.3f\n", load_float(rs1));
  dbg_printf("RBF[rs2] = %.3f\n", load_float(rs2));
  dbg_printf("RBF[rs2] = %.3f\n", load_float(rs3));
  float res;
  res = (-(load_float(rs1) * load_float(rs2) - load_float(rs3)));
  save_float(res, rd);
  dbg_printf("Result = %.3f\n\n", res);
}

// Instruction FNMADD.S behavior method
void ac_behavior(FNMADD_S) {
  dbg_printf("FNMADD.S r%d, r%d, r%d, r%d\n", rd, rs1, rs2, rs3);
  dbg_printf("RBF[rs1] = %.3f\n", load_float(rs1));
  dbg_printf("RBF[rs2] = %.3f\n", load_float(rs2));
  dbg_printf("RBF[rs2] = %.3f\n", load_float(rs3));
  float res;
  res = (-(load_float(rs1) * load_float(rs2) + load_float(rs3)));
  save_float(res, rd);
  dbg_printf("Result = %.3f\n\n", res);
}

// Instruction FCVT.W.S behavior method
void ac_behavior(FCVT_W_S) {
  dbg_printf("FCVT.W.S r%d, r%d\n", rd, rs1);
  dbg_printf("RBF[rs1] = %f\n", load_float(rs1));
  RB[rd] = round(load_float(rs1));
  dbg_printf("RB[rd] = %d \n \n", RB[rd]);
}

// Instruction FCVT.WU.S behavior method
void ac_behavior(FCVT_WU_S) {
  dbg_printf("FCVT.WU.S r%d, r%d\n", rd, rs1);
  dbg_printf("RBF[rs1] = %f\n", load_float(rs1));
  RB[rd] = (unsigned int)(load_float(rs1));
  dbg_printf("RB[rd] = %d \n \n", RB[rd]);
}

// Instruction FCVT.S.W behaior method
void ac_behavior(FCVT_S_W) {
  dbg_printf("FCVT.S.W r%d, r%d \n", rd, rs1);
  dbg_printf("RB[rs1] = %d \n", RB[rs1]);
  float temp;
  temp = (float)RB[rs1];
  save_float(temp, rd);
}

// Instruction FCVT_S_WU behaior method
void ac_behavior(FCVT_S_WU) {
  dbg_printf("FCVT.S.W r%d, r%d \n", rd, rs1);
  dbg_printf("RB[rs1] = %d \n", RB[rs1]);
  float temp;
  temp = (float)RB[rs1];
  save_float(temp, rd);
}

// Instruction FMV_X_S behavior method
void ac_behavior(FMV_X_S) {
  dbg_printf("FMV.X.S r%d, r%d \n", rd, rs1);
  dbg_printf("RBF[rs1] = %f \n", load_float(rs1));
  RB[rd] = (int)load_float(rs1);
  dbg_printf("RB[rd] = %d \n \n", RB[rd]);
}

// Instruction FMV_S_X behavior method
void ac_behavior(FMV_S_X) {
  dbg_printf("FMV.S.X r%d, r%d \n", rd, rs1);
  dbg_printf("RB[rs1] = %d \n", RB[rs1]);
  save_float(RB[rs1], rd);
  dbg_printf("RBF[rd] = %f \n \n", load_float(rd));
}

// Instruction FEQ_S behavior method
void ac_behavior(FEQ_S) {
  dbg_printf("FEQ.S r%d, r%d, r%d \n", rd, rs1, rs2);
  dbg_printf("RBF[rs1] = %f \n", load_float(rs1));
  dbg_printf("RBF[rs2] = %f \n", load_float(rs2));
  if ((isnan(load_float(rs1)) == 1) || (isnan(load_float(rs2)) == 1)) {
    printf("Invalid Operation\n");
    RB[rd] = 0;
  }
  if (load_float(rs1) == load_float(rs2))
    RB[rd] = 1;
  else
    RB[rd] = 0;
  dbg_printf("Result = %d \n \n", RB[rd]);
}

// Instruction FLE_S behavior method
void ac_behavior(FLE_S) {
  dbg_printf("FLE.S r%d, r%d, r%d \n", rd, rs1, rs2);
  dbg_printf("RBF[rs1] = %f \n", load_float(rs1));
  dbg_printf("RBF[rs2] = %f \n", load_float(rs2));
  if ((isnan(load_float(rs1)) == 1) || (isnan(load_float(rs2)) == 1)) {
    printf("Invalid Operation\n");
    RB[rd] = 0;
  }
  if (load_float(rs1) <= load_float(rs2))
    RB[rd] = 1;
  else
    RB[rd] = 0;
  dbg_printf("Result = %d \n \n", RB[rd]);
}

// Instruction FLT_S behavior method
void ac_behavior(FLT_S) {
  dbg_printf("FLT.S r%d, r%d, r%d \n", rd, rs1, rs2);
  dbg_printf("RBF[rs1] = %f \n", load_float(rs1));
  dbg_printf("RBF[rs2] = %f \n", load_float(rs2));
  if ((isnan(load_float(rs1)) == 1) || (isnan(load_float(rs2)) == 1)) {
    printf("Invalid Operation\n");
    RB[rd] = 0;
  }
  if (load_float(rs1) < load_float(rs2))
    RB[rd] = 1;
  else
    RB[rd] = 0;
  dbg_printf("Result = %d \n \n", RB[rd]);
}

// Instruction FMV.S behavior method
void ac_behavior(FMV_S) {
  dbg_printf("FMV.S r%d, r%d", rd, rs1);
  float temp;
  temp = load_float(rs1);
  save_float(temp, rd);
}

// Instruction FLD behavior method
void ac_behavior(FLD) {
  int imm;
  imm = (imm4 << 11) | (imm3 << 5) | (imm2 << 1) | imm1;
  dbg_printf("FLD r%d, r%d, %d\n", rd, rs1, imm);
  int sign_ext;
  sign_ext = sign_extend(imm, 12);
  RBF[rd * 2] = DM.read(RB[rs1] + sign_ext);
  RBF[rd * 2 + 1] = DM.read(RB[rs1] + sign_ext + 4);
  dbg_printf("RB[rs1] = %#x\n", RB[rs1]);
  dbg_printf("addr = %#x\n", RB[rs1] + sign_ext);
  double temp = load_double(rd);
  dbg_printf("Double: %lf", temp);
}

// Instruction FSD behavior method
void ac_behavior(FSD) {
  int imm;
  imm = (imm4 << 11) | (imm3 << 5) | (imm2 << 1) | imm1;
  dbg_printf("FSD r%d, r%d, %d\n", rs1, rs2, imm);
  int sign_ext;
  sign_ext = sign_extend(imm, 12);
  DM.write(RB[rs1] + sign_ext, RBF[rs2 * 2]);
  DM.write(RB[rs1] + sign_ext + 4, RBF[rs2 * 2 + 1]);
  dbg_printf("addr: %d\n\n", RB[rs1] + sign_ext);
}

// Instruction FADD.D behavior method
void ac_behavior(FADD_D) {
  dbg_printf("FADD.D r%d, r%d, r%d\n", rd, rs1, rs2);
  dbg_printf("RBF[rs1] = %.3f\n", load_double(rs1));
  dbg_printf("RBF[rs2] = %.3f\n", load_double(rs2));
  double sum;
  sum = load_double(rs1) + load_double(rs2);
  save_double(sum, rd);
  dbg_printf("Result = %.3f\n\n", sum);
}

// Instruction FSUB.D behavior method
void ac_behavior(FSUB_D) {
  dbg_printf("FSUB.D r%d, r%d, r%d\n", rd, rs1, rs2);
  dbg_printf("RBF[rs1] = %.3f\n", load_double(rs1));
  dbg_printf("RBF[rs2] = %.3f\n", load_double(rs2));
  double diff;
  diff = load_double(rs1) - load_double(rs2);
  save_double(diff, rd);
  dbg_printf("Result = %.3f\n\n", diff);
}

// Instruction FMUL.D behavior method
void ac_behavior(FMUL_D) {
  dbg_printf("FMUL.D r%d, r%d, r%d\n", rd, rs1, rs2);
  dbg_printf("RBF[rs1] = %.3f\n", load_double(rs1));
  dbg_printf("RBF[rs2] = %.3f\n", load_double(rs2));
  double product;
  product = load_double(rs1) * load_double(rs2);
  save_double(product, rd);
  dbg_printf("Result = %.3f\n\n", product);
}

// Instruction FDIV.D behavior method
void ac_behavior(FDIV_D) {
  dbg_printf("FDIV.D r%d, r%d, r%d\n", rd, rs1, rs2);
  dbg_printf("RBF[rs1] = %.3f\n", load_double(rs1));
  dbg_printf("RBF[rs2] = %.3f\n", load_double(rs2));
  double quot;
  quot = load_double(rs1) / load_double(rs2);
  save_double(quot, rd);
  dbg_printf("Result = %.3f\n\n", quot);
}

// Instruction FMIN.D behavior method
void ac_behavior(FMIN_D) {
  dbg_printf("FMIN.S r%d, r%d, r%d\n", rd, rs1, rs2);
  dbg_printf("RBF[rs1] = %.3f\n", load_double(rs1));
  dbg_printf("RBF[rs2] = %.3f\n", load_double(rs2));
  double temp;
  if (load_double(rs1) < load_double(rs2)) {
    temp = load_double(rs1);
    save_double(temp, rd);
  } else {
    temp = load_double(rs2);
    save_double(temp, rd);
  }
  dbg_printf("Result = %.3f\n\n", temp);
}

// Instruction FMAX.D behavior method
void ac_behavior(FMAX_D) {
  dbg_printf("FMAX.D r%d, r%d, r%d\n", rd, rs1, rs2);
  dbg_printf("RBF[rs1] = %.3f\n", load_double(rs1));
  dbg_printf("RBF[rs2] = %.3f\n", load_double(rs2));
  double temp;
  if (load_double(rs1) > load_double(rs2)) {
    temp = load_double(rs1);
    save_double(temp, rd);
  } else {
    temp = load_double(rs2);
    save_double(temp, rd);
  }
  dbg_printf("Result = %.3f\n\n", temp);
}

// Instruction FSQRT.D behavior method
void ac_behavior(FSQRT_D) {
  dbg_printf("FSQRT.D r%d, r%d\n", rd, rs1);
  dbg_printf("RBF[rs1] = %.3f\n", load_double(rs1));
  double temp;
  if (load_double(rs1) < 0) {
    dbg_printf("Invalid!");
    stop();
  } else {
    temp = sqrt(load_double(rs1));
    save_double(temp, rd);
  }
  dbg_printf("Result = %.3f\n\n", temp);
}

// Instruction FMADD.D behavior method
void ac_behavior(FMADD_D) {
  dbg_printf("FMADD.D r%d, r%d, r%d, r%d\n", rd, rs1, rs2, rs3);
  dbg_printf("RBF[rs1] = %.3f\n", load_double(rs1));
  dbg_printf("RBF[rs2] = %.3f\n", load_double(rs2));
  dbg_printf("RBF[rs2] = %.3f\n", load_double(rs3));
  double res;
  res = load_double(rs1) * load_double(rs2) + load_double(rs3);
  save_double(res, rd);
  dbg_printf("Result = %.3f\n\n", res);
}

// Instruction FMSUB.D behavior method
void ac_behavior(FMSUB_D) {
  dbg_printf("FMSUB.D r%d, r%d, r%d, r%d\n", rd, rs1, rs2, rs3);
  dbg_printf("RBF[rs1] = %.3f\n", load_double(rs1));
  dbg_printf("RBF[rs2] = %.3f\n", load_double(rs2));
  dbg_printf("RBF[rs2] = %.3f\n", load_double(rs3));
  double res;
  res = load_double(rs1) * load_double(rs2) - load_double(rs3);
  save_double(res, rd);
  dbg_printf("Result = %.3f\n\n", res);
}

// Instruction FNMSUB.D behavior method
void ac_behavior(FNMSUB_D) {
  dbg_printf("FNMSUB.D r%d, r%d, r%d, r%d\n", rd, rs1, rs2, rs3);
  dbg_printf("RBF[rs1] = %.3f\n", load_double(rs1));
  dbg_printf("RBF[rs2] = %.3f\n", load_double(rs2));
  dbg_printf("RBF[rs2] = %.3f\n", load_double(rs3));
  double res;
  res = (-(load_double(rs1) * load_double(rs2) - load_double(rs3)));
  save_double(res, rd);
  dbg_printf("Result = %.3f\n\n", res);
}

// Instruction FNMADD.D behavior method
void ac_behavior(FNMADD_D) {
  dbg_printf("FNMADD.D r%d, r%d, r%d, r%d\n", rd, rs1, rs2, rs3);
  dbg_printf("RBF[rs1] = %.3f\n", load_double(rs1));
  dbg_printf("RBF[rs2] = %.3f\n", load_double(rs2));
  dbg_printf("RBF[rs2] = %.3f\n", load_double(rs3));
  double res;
  res = (-(load_double(rs1) * load_double(rs2) + load_double(rs3)));
  save_double(res, rd);
  dbg_printf("Result = %.3f\n\n", res);
}

// Instruction FCVT.W.D behavior method
void ac_behavior(FCVT_W_D) {
  dbg_printf("FCVT.W.D r%d, r%d\n", rd, rs1);
  dbg_printf("RBF[rs1] = %f\n", load_double(rs1));
  RB[rd] = lround(load_double(rs1));
  dbg_printf("RB[rd] = %d \n \n", RB[rd]);
}

// Instruction FCVT.WU.D behavior method
void ac_behavior(FCVT_WU_D) {
  dbg_printf("FCVT.WU.D r%d, r%d\n", rd, rs1);
  dbg_printf("RBF[rs1] = %f\n", load_double(rs1));
  RB[rd] = (unsigned int)(load_double(rs1));
  dbg_printf("RB[rd] = %d \n \n", RB[rd]);
}

// Instruction FCVT_D_W behaior method
void ac_behavior(FCVT_D_W) {
  dbg_printf("FCVT.D.W r%d, r%d \n", rd, rs1);
  dbg_printf("RB[rs1] = %d \n", RB[rs1]);
  double temp;
  temp = (double)RB[rs1];
  save_float(temp, rd);
}

// Instruction FCVT_D_WU behaior method
void ac_behavior(FCVT_D_WU) {
  dbg_printf("FCVT.D.W r%d, r%d \n", rd, rs1);
  dbg_printf("RB[rs1] = %d \n", RB[rs1]);
  double temp;
  temp = (double)RB[rs1];
  save_double(temp, rd);
}

// Instruction FCVT_S_D behavior method
void ac_behavior(FCVT_S_D) {
  dbg_printf("FCVT.S.D r%d, r%d", rd, rs1);
  dbg_printf("RBF[rs1] = %f \n", load_float(rs1));
  double temp;
  temp = (float)(load_double(rs1));
  save_float(temp, rd);
}

// Instruction FCVT_D_S behavior method
void ac_behavior(FCVT_D_S) {
  dbg_printf("FCVT.D.S r%d, r%d", rd, rs1);
  dbg_printf("RBF[rs1] = %f \n", load_double(rs1));
  float temp;
  temp = (double)(load_float(rs1));
  save_double(temp, rd);
}

// Instruction FMV.D behavior method
void ac_behavior(FMV_D) {
  dbg_printf("FMV.D r%d, r%d", rd, rs1);
  double temp;
  temp = load_double(rs1);
  save_double(temp, rd);
}

// Instruction FEQ_D behavior method
void ac_behavior(FEQ_D) {
  dbg_printf("FEQ.D r%d, r%d, r%d \n", rd, rs1, rs2);
  dbg_printf("RBF[rs1] = %f \n", load_double(rs1));
  dbg_printf("RBF[rs2] = %f \n", load_double(rs2));
  if ((custom_isnan(load_double(rs1)) == 1) ||
      (custom_isnan(load_double(rs2)) == 1)) {
    printf("Invalid Operation\n");
    RB[rd] = 0;
  }
  if (load_double(rs1) == load_double(rs2))
    RB[rd] = 1;
  else
    RB[rd] = 0;
  dbg_printf("Result = %d \n \n", RB[rd]);
}

// Instruction FLE_D behavior method
void ac_behavior(FLE_D) {
  dbg_printf("FLE.D r%d, r%d, r%d \n", rd, rs1, rs2);
  dbg_printf("RBF[rs1] = %f \n", load_double(rs1));
  dbg_printf("RBF[rs2] = %f \n", load_double(rs2));
  if ((custom_isnan(load_double(rs1)) == 1) ||
      (custom_isnan(load_double(rs2)) == 1)) {
    printf("Invalid Operation\n");
    RB[rd] = 0;
  }
  if (load_double(rs1) <= load_double(rs2))
    RB[rd] = 1;
  else
    RB[rd] = 0;
  dbg_printf("Result = %d \n \n", RB[rd]);
}

// Instruction FLT_D behavior method
void ac_behavior(FLT_D) {
  dbg_printf("FLT.D r%d, r%d, r%d \n", rd, rs1, rs2);
  dbg_printf("RBF[rs1] = %f \n", load_double(rs1));
  dbg_printf("RBF[rs2] = %f \n", load_double(rs2));
  if ((custom_isnan(load_double(rs1)) == 1) ||
      (custom_isnan(load_double(rs2)) == 1)) {
    printf("Invalid Operation\n");
    RB[rd] = 0;
  }
  if (load_double(rs1) < load_double(rs2))
    RB[rd] = 1;
  else
    RB[rd] = 0;
  dbg_printf("Result = %d \n \n", RB[rd]);
}
