#include "riscv_isa.H"
#include "riscv_isa_init.cpp"
#include "riscv_bhv_macros.H"

//For debug Information
#define DEBUG_MODEL
#include "ac_debug_model.H"

//#  define dbg_printf(args...) fprintf (stderr, "DBG: " args)
//#define dbg_printf printf();

#define Ra 1 
#define Sp 14

using namespace riscv_parms;

static int processors_started = 0;
#define DEFAULT_STACK_SIZE (512*1024); //check this parameter

//generic instruction behavior method
void ac_behavior(instruction){
	dbg_printf("---PC=%#x---%lld\n", (int) ac_pc, ac_instr_counter);
	ac_pc = ac_pc + 4;
}

//Instruction Format behavior methods
void ac_behavior(Type_R){}
void ac_behavior(Type_I){}
void ac_behavior(Type_S){}
void ac_behavior(Type_SB){}
void ac_behavior(Type_U){}
void ac_behavior(Type_UJ){}
void ac_behavior(Type_I_shamt){}

// Behavior called before starting simulation
void ac_behavior(begin)
{
  dbg_printf("@@@ begin behavior @@@\n");
  //ac_pc = ac_pc + 4;

  for (int regNum = 0; regNum < 32; regNum ++)
    RB[regNum] = 0;
}

// Behavior called after finishing simulation
void ac_behavior(end)
{
  dbg_printf("@@@ end behavior @@@\n");
}

void ac_behavior( ADD ){
	dbg_printf("ADD r%d, r%d, r%d", rd, rs1, rs2);
	RB[rd] = RB[rs1] + RB[rs2];
	dbg_printf("Result = %#x\n", RB[rd]);
	//RISCV doesn't check for overflow
}

void ac_behavior( SUB ){
	dbg_printf("SUB r%d, r%d, r%d", rd, rs1, rs2);
	RB[rd] = RB[rs1] - RB[rs2];
	dbg_printf("Result = %#x\n", RB[rd]);
	//RISCV doesn't check for overflow
}

void ac_behavior( SLL ){
	dbg_printf("SLL r%d, r%d, r%d", rd, rs1, rs2);
	RB[rd] = RB[rs1] << RB[rs2];
	dbg_printf("Result = %#x\n", RB[rd]);
}

void ac_behavior( SLT ){
	dbg_printf("SLT r%d, r%d, r%d", rd, rs1, rs2);
	if(RB[rs1] < RB[rs2])
		RB[rd] = 1;
	else
		RB[rd] = 0;
	dbg_printf("Result = %#x\n", RB[rd]);
}


void ac_behavior( SLTU ){
	dbg_printf("SLTU r%d, r%d, r%d", rd, rs1, rs2);
	if( (ac_Uword)RB[rs1] < (ac_Uword)RB[rs2])
		RB[rd] = 1;
	else
		RB[rd] = 0;
	dbg_printf("Result = %#x\n", RB[rd]);
}

void ac_behavior( XOR ){
	dbg_printf("XOR r%d, r%d, r%d", rd, rs1, rs2);
	RB[rd] = RB[rs1] ^ RB[rs2];
	dbg_printf("Result = %#x\n", RB[rd]);
}

void ac_behavior( SRL ){
	dbg_printf("SRL r%d, r%d, r%d", rd, rs1, rs2);
	RB[rd] = RB[rs1] >> RB[rs2];
	dbg_printf("Result = %#x\n", RB[rd]);
}

void ac_behavior( SRA ){
	dbg_printf("SRA r%d, r%d, r%d", rd, rs1, rs2);
	if((RB[rs1] >> 4) == 1)
		RB[rd] = (RB[rs1]>>RB[rs2]) | (0xFFFFFFFF<<RB[rs2]);
	else
		RB[rd] = RB[rs1]>>RB[rs2];
	dbg_printf("Result = %#x\n", RB[rd]);
}

void ac_behavior( OR ){
	dbg_printf("OR r%d, r%d, r%d", rd, rs1, rs2);
	RB[rd] = RB[rs1] | RB[rs2];
	dbg_printf("Result = %#x\n", RB[rd]);
}

void ac_behavior( AND ){
	dbg_printf("AND r%d, r%d, r%d", rd, rs1, rs2);
	RB[rd] = RB[rs1] & RB[rs2];
	dbg_printf("Result = %#x\n", RB[rd]);
}


void ac_behavior( LB ){
	char byte;
	dbg_printf("LB r%d, r%d, %d", rd, rs1, imm);
	byte = DATA_PORT->read_byte(RB[rs1] + (imm | 0xFFFFF000));
	RB[rd] = (ac_Sword)byte;
	dbg_printf("Result = %#x\n", RB[rd]);
}

void ac_behavior( LH ){
	short int half;
	dbg_printf("LH r%d, r%d, %d", rd, rs1, imm);
	half = DATA_PORT->read_half(RB[rs1] + (imm | 0xFFFFF000));
	RB[rd] = (ac_Sword)half;
	dbg_printf("Result = %#x\n", RB[rd]);
}

void ac_behavior( LW ){
	dbg_printf("LW r%d, r%d, %d", rd, rs1, imm);
	RB[rd] = DATA_PORT->read(RB[rs1] + (imm | 0xFFFFF000));
	dbg_printf("Result = %#x\n", RB[rd]);
}

void ac_behavior( LBU ){
	dbg_printf("LBU r%d, r%d, %d", rd, rs1, imm);
	unsigned char byte;
	byte = DATA_PORT->read_byte(RB[rs1] + (imm | 0xFFFFF000));
	RB[rd] = (ac_Uword)byte;
	dbg_printf("Result = %#x\n", RB[rd]);
}

void ac_behavior( LHU ){
	unsigned short int half;
	dbg_printf("LHU r%d, r%d, %d", rd, rs1, imm);
	half = DATA_PORT->read_half(RB[rs1] + (imm | 0xFFFFF000));
	RB[rd] = (ac_Uword)half;
	dbg_printf("Result = %#x\n", RB[rd]);	
}

void ac_behavior( ADDI ){
	dbg_printf("ADDI r%d, r%d, %d", rd, rs1, imm);
	if((rd == 0) && (rs1 == 0) && (imm == 0))
	{
		ac_pc = ac_pc + 4;
		dbg_printf("NOP executed!");
	}
	else
	{
		int sign_ext;
		if((imm >> 11) == 1)
			sign_ext = imm | 0xFFFFF000;
		else
			sign_ext = imm;
		RB[rd] = RB[rs1] + sign_ext;
		dbg_printf("Result = %#x\n", RB[rd]);
	}
	
}


void ac_behavior( SLTI ){
	dbg_printf("SLTI r%d, r%d, %d", rd, rs1, imm);
	int sign_ext;
	if((imm >> 11) == 1)
		sign_ext = imm | 0xFFFFF000;
	else
		sign_ext = imm;
	if(RB[rs1] < sign_ext)
		RB[rd] = 1;
	else
		RB[rd] = 0;
	dbg_printf("Result = %#x\n", RB[rd]);
}


void ac_behavior( SLTIU ){
	dbg_printf("SLTIU r%d, r%d, %d", rd, rs1, imm);
	int sign_ext;
	if((imm >> 11) == 1)
		sign_ext = imm | 0xFFFFF000;
	else
		sign_ext = imm;	
	if( (ac_Uword)RB[rs1] < (ac_Uword)sign_ext)
		RB[rd] = 1;
	else
		RB[rd] = 0;
	dbg_printf("Result = %#x\n", RB[rd]);
}


void ac_behavior( XORI ){
	dbg_printf("XORI r%d, r%d, %d", rd, rs1, imm);
	int sign_ext;
	if((imm >> 11) == 1)
		sign_ext = imm | 0xFFFFF000;
	else
		sign_ext = imm;
	RB[rd] = RB[rs1] ^ sign_ext;
	dbg_printf("Result = %#x\n", RB[rd]);
}

void ac_behavior( ORI ){
	dbg_printf("ORI r%d, r%d, %d", rd, rs1, imm);
	int sign_ext;
	if((imm >> 11) == 1)
		sign_ext = imm | 0xFFFFF000;
	else
		sign_ext = imm;
	RB[rd] = RB[rs1] + sign_ext;
	dbg_printf("Result = %#x\n", RB[rd]);
}

void ac_behavior( ANDI ){
	dbg_printf("ANDI r%d, r%d, %d", rd, rs1, imm);
	int sign_ext;
	if((imm >> 11) == 1)
		sign_ext = imm | 0xFFFFF000;
	else
		sign_ext = imm;
	RB[rd] = RB[rs1] & sign_ext;
	dbg_printf("Result = %#x\n", RB[rd]);
}

void ac_behavior( JALR ){
	int target_addr;
	dbg_printf("JALR r%d, r%d, %d", rd, rs1, imm);
	target_addr = (imm + RB[rs1]) & 0xD;
	ac_pc = target_addr;
	RB[rd] = ac_pc+4;
}


void ac_behavior( SLLI ){
	dbg_printf("SLLI r%d, r%d, %d", rd, rs1, shamt);
	RB[rd] = RB[rs1]<<shamt;
	dbg_printf("Result = %#x\n", RB[rd]);
}

void ac_behavior( SRLI ){
	dbg_printf("SRLI r%d, r%d, %d", rd, rs1, shamt);
	RB[rd] = RB[rs1]>>shamt;
	dbg_printf("Result = %#x\n", RB[rd]);
}

void ac_behavior( SRAI ){
	dbg_printf("SRAI r%d, r%d, %d", rd, rs1, shamt);
	if((imm >> 5) == 1)
		RB[rd] = (RB[rs1]>>shamt) | (0xFFFFFFFF<<shamt);
	else
		RB[rd] = RB[rs1]>>shamt;
	dbg_printf("Result = %#x\n", RB[rd]);
}

void ac_behavior( SCALL ){
	dbg_printf("SCALL");
	printf("System Call\n");
}

void ac_behavior( SBREAK ){
	dbg_printf("SBREAK");
	printf("Breakpoint\n");
}

void ac_behavior( RDCYCLE ){
	dbg_printf("RDCYCLE r%d", rd);
	RB[rd] = ac_pc;
	dbg_printf("Result = %#x\n", RB[rd]);
}

void ac_behavior( RDCYCLEH ){
	dbg_printf("RDCYCLEH r%d", rd);
	//still need to work on this one
}

void ac_behavior( RDTIME ){
	dbg_printf("RDTIME r%d", rd);
	//still need to work on this one
}

void ac_behavior( RDTIMEH ){
	dbg_printf("RDTIMEH r%d", rd);
	//still need to work on this one
} 

void ac_behavior( RDINSTRET ){
	dbg_printf("RDINSTRET r%d", rd);
	//Same as RDCYCLE
}

void ac_behavior( RDINSTRETH ){
	dbg_printf("RDINSTRETH r%d", rd);
	//Same as RDCYCLEH
}

void ac_behavior( SB ){
	unsigned char byte;
	uint32_t imm;
	imm = ( imm1 << 7) | imm2;
	dbg_printf("SB r%d, r%d, %d", rs1, rs2, imm);
	byte = RB[rs2];
	DATA_PORT->write_byte(RB[rs1] + (imm | 0xFFFFF000), byte);
}

void ac_behavior( SH ){
	uint32_t imm;
	imm = ( imm1 << 7) | imm2;
	dbg_printf("SH r%d, r%d, %d", rs1, rs2, imm);
	DATA_PORT->write_half(RB[rs1] + (imm | 0xFFFFF000), RB[rs2]);
}

void ac_behavior( SW ){
	uint32_t imm;
	imm = ( imm1 << 7) | imm2;
	dbg_printf("SW r%d, r%d, %d", rs1, rs2, imm);
	DATA_PORT->write(RB[rs1] + (imm | 0xFFFFF000), RB[rs2]);
}

void ac_behavior( BEQ ){
	uint32_t imm;
	imm = ( imm1 << 7) | imm2;
	dbg_printf("BEQ r%d, r%d, %d", rs1, rs2, imm);
	if(RB[rs1] == RB[rs2])
	{
		ac_pc = ac_pc + imm;
		//printf("PC updated to %d\n", ac_pc);
		printf("PC updated\n");
	}
}

void ac_behavior( BNE ){
	uint32_t imm;
	imm = ( imm1 << 7) | imm2;	
	dbg_printf("BNE r%d, r%d, %d", rs1, rs2, imm);
	if(RB[rs1] != RB[rs2])
	{
		ac_pc = ac_pc + imm;
		//printf("PC updated to %d\n", ac_pc);
		printf("PC updated\n");
	}
}

void ac_behavior( BLT ){
	uint32_t imm;
	imm = ( imm1 << 7) | imm2;	
	dbg_printf("BLT r%d, r%d, %d", rs1, rs2, imm);
	if(RB[rs1] < RB[rs2])
	{
		ac_pc = ac_pc + imm;
		//printf("PC updated to %d\n", ac_pc);
		printf("PC updated\n");
	}
}

void ac_behavior( BGE ){
	uint32_t imm;
	imm = ( imm1 << 7) | imm2;	
	dbg_printf("BGE r%d, r%d, %d", rs1, rs2, imm);
	if((RB[rs1] > RB[rs2]) || (RB[rs1] == RB[rs2])) 
	{
		ac_pc = ac_pc + imm;
		//printf("PC updated to %d\n", ac_pc);
		printf("PC updated\n");
	}
}

void ac_behavior( BLTU ){
	uint32_t imm;
	imm = ( imm1 << 7) | imm2;	
	dbg_printf("BLTU r%d, r%d, %d", rs1, rs2, imm);
	if((ac_Uword)RB[rs1] < (ac_Uword)RB[rs2])
	{
		ac_pc = ac_pc + imm;
		//printf("PC updated to %d\n", ac_pc);
		printf("PC updated\n");
	}
}

void ac_behavior( BGEU ){
	uint32_t imm;
	imm = ( imm1 << 7) | imm2;	
	dbg_printf("BGEU r%d, r%d, %d", rs1, rs2, imm);
	if(((ac_Uword)RB[rs1] > (ac_Uword)RB[rs2]) || ((ac_Uword)RB[rs1] == (ac_Uword)RB[rs2])) 
	{
		ac_pc = ac_pc + imm;
		//printf("PC updated to %d\n", ac_pc);
		printf("PC updated\n");
	}
}

void ac_behavior( LUI ){
	dbg_printf("LUI r%d, %d", rd, imm);
	RB[rd] = imm<<12; 
	dbg_printf("Result = %#x\n", RB[rd]);
}

void ac_behavior( AUIPC ){
	dbg_printf("AUIPC r%d, %d", rd, imm);
	int offset;
	offset = imm<<12;
	RB[rd] = ac_pc + offset;
	dbg_printf("Result = %#x\n", RB[rd]);
}

void ac_behavior( JAL ){
	dbg_printf("JAL r%d, %d", rd, imm);
	RB[rd] = ac_pc + 4;
	ac_pc = (imm & 0xFFFFFFFF) + ac_pc; 
	printf("--- Jump taken ---\n");
}

void ac_behavior( MUL ){
	dbg_printf("MUL r%d, r%d, r%d", rd, rs1, rs2);
	RB[rd] = RB[rs1] * RB[rs2];
	dbg_printf("Result = %#x\n", RB[rd]);	
}

void ac_behavior( MULH ){
	dbg_printf("MULH r%d, r%d, r%d", rd, rs1, rs2);
	long int mult;
	mult = RB[rs1] * RB[rs2];
	mult = mult>>32;
	RB[rd] = mult;
	dbg_printf("Result = %#x\n", RB[rd]);
}

void ac_behavior( MULHSU ){
	dbg_printf("MULHSU r%d, r%d, r%d", rd, rs1, rs2);
	long int mult;
	mult = RB[rs1] * (ac_Uword)(RB[rs2]);
	mult = mult >>32;
	RB[rd] = mult;
	dbg_printf("Result = %#x\n", RB[rd]);	
}

void ac_behavior( MULHU ){
	dbg_printf("MULHU r%d, r%d, r%d", rd, rs1, rs2);
	long int mult;
	mult = (ac_Uword)RB[rs1] * (ac_Uword)RB[rs2];
	mult = mult >>32;
	RB[rd] = mult;
	dbg_printf("Result = %#x\n", RB[rd]);	
}

void ac_behavior( DIV ){
	dbg_printf("DIV r%d, r%d, r%d", rd, rs1, rs2);
	if(RB[rs2] == 0)
		RB[rd]=-1;
	else if((RB[rs1] == -2^(31)) && (RB[rs2] == -1))
		RB[rd] = -2^(31);
	else
		RB[rd] = RB[rs1]/RB[rs2];
	dbg_printf("Result = %#x\n", RB[rd]);
}

void ac_behavior( DIVU ){
	dbg_printf("DIVU r%d, r%d, r%d", rd, rs1, rs2);
	if(RB[rs2] == 0)
		RB[rd]=-1;
	else if((RB[rs1] == -2^(31)) && (RB[rs2] == -1))
		dbg_printf("Wrong division exception!");
	else
		RB[rd] = (ac_Uword)RB[rs1]/(ac_Uword)RB[rs2];
	dbg_printf("Result = %#x\n", (ac_Uword)RB[rd]);	
}

void ac_behavior( REM ){
	dbg_printf("REM r%d, r%d, r%d", rd, rs1, rs2);
	if(RB[rs2] == 0)
		RB[rd]=RB[rs1];
	else if((RB[rs1] == -2^(31)) && (RB[rs2] == -1))
		RB[rd] = 0;
	else
		RB[rd] = RB[rs1]%RB[rs2];
	dbg_printf("Result = %#x\n", RB[rd]);	
}

void ac_behavior( REMU ){
	dbg_printf("REMU r%d, r%d, r%d", rd, rs1, rs2);
	if(RB[rs2] == 0)
		RB[rd]=RB[rs1];
	else if((RB[rs1] == -2^(31)) && (RB[rs2] == -1))
		dbg_printf("Wrong division exception!");
	else
		RB[rd] = (ac_Uword)RB[rs1]%(ac_Uword)RB[rs2];
	dbg_printf("Result = %#x\n", (ac_Uword)RB[rd]);		
}
