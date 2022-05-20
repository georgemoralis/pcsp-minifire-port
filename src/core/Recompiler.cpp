#include "PCSPCommon.h"
#include "Recompiler.h"
#include "Cpu.h"
#include "Memory.h"

#define _Rd_ ((Cpu.instr >> 11) & 0x1F)
#define _Rt_ ((Cpu.instr >> 16) & 0x1F)
#define _Rs_ ((Cpu.instr >> 21) & 0x1F)

#define Rd ((u32)&Cpu.gpr[_Rd_])
#define Rt ((u32)&Cpu.gpr[_Rt_])
#define Rs ((u32)&Cpu.gpr[_Rs_])
#define Sa ((Cpu.instr >> 6) & 0x1F)
#define Im ((u16)Cpu.instr)
#define Im20 ((Cpu.instr >> 6) & 0x000FFFFF)

CRec &Rec = CRec::GetSingleton();

void (* CRec::mainTable[64])() = {
	CRec::SPECIAL, CRec::REGIMM, CRec::J  , CRec::UNK  ,
	CRec::UNK    , CRec::BNE   , CRec::UNK, CRec::BGTZ ,
	CRec::UNK    , CRec::ADDIU , CRec::UNK, CRec::SLTIU,
	CRec::UNK    , CRec::ORI   , CRec::UNK, CRec::LUI  ,
	CRec::UNK    , CRec::UNK   , CRec::UNK, CRec::UNK  ,
	CRec::UNK    , CRec::BNEL  , CRec::UNK, CRec::BGTZL,
	CRec::UNK    , CRec::UNK   , CRec::UNK, CRec::UNK  ,
	CRec::UNK    , CRec::UNK   , CRec::UNK, CRec::UNK  ,
	CRec::UNK    , CRec::UNK   , CRec::UNK, CRec::LW  ,
	CRec::LBU    , CRec::UNK   , CRec::UNK, CRec::UNK  ,
	CRec::SB     , CRec::UNK   , CRec::UNK, CRec::SW  ,
	CRec::UNK    , CRec::UNK   , CRec::UNK, CRec::UNK  ,
	CRec::UNK    , CRec::UNK   , CRec::UNK, CRec::UNK  ,
	CRec::UNK    , CRec::UNK   , CRec::UNK, CRec::UNK  ,
	CRec::UNK    , CRec::UNK   , CRec::UNK, CRec::UNK  ,
	CRec::UNK    , CRec::UNK   , CRec::UNK, CRec::UNK  ,
};

void (* CRec::special[64])() = {
	CRec::SLL    , CRec::UNK , CRec::SRL , CRec::UNK ,
	CRec::UNK    , CRec::UNK , CRec::UNK , CRec::UNK ,
	CRec::UNK    , CRec::UNK , CRec::UNK , CRec::UNK ,
	CRec::SYSCALL, CRec::UNK , CRec::UNK , CRec::UNK ,
	CRec::MFHI   , CRec::UNK , CRec::MFLO, CRec::UNK ,
	CRec::UNK    , CRec::UNK , CRec::UNK , CRec::UNK ,
	CRec::UNK    , CRec::UNK , CRec::UNK , CRec::DIVU,
	CRec::UNK    , CRec::UNK , CRec::UNK , CRec::UNK ,
	CRec::ADD    , CRec::ADDU, CRec::UNK , CRec::UNK ,
	CRec::AND    , CRec::UNK , CRec::XOR , CRec::UNK ,
	CRec::UNK    , CRec::UNK , CRec::UNK , CRec::SLTU,
	CRec::UNK    , CRec::UNK , CRec::UNK , CRec::UNK ,
	CRec::UNK    , CRec::UNK , CRec::UNK , CRec::UNK ,
	CRec::UNK    , CRec::UNK , CRec::UNK , CRec::UNK ,
	CRec::UNK    , CRec::UNK , CRec::UNK , CRec::UNK ,
	CRec::UNK    , CRec::UNK , CRec::UNK , CRec::UNK ,
};

void (* CRec::regimm[32])() = {
	CRec::UNK, CRec::BGEZ, CRec::UNK, CRec::UNK,
	CRec::UNK, CRec::UNK , CRec::UNK, CRec::UNK,
	CRec::UNK, CRec::UNK , CRec::UNK, CRec::UNK,
	CRec::UNK, CRec::UNK , CRec::UNK, CRec::UNK,
	CRec::UNK, CRec::UNK , CRec::UNK, CRec::UNK,
	CRec::UNK, CRec::UNK , CRec::UNK, CRec::UNK,
	CRec::UNK, CRec::UNK , CRec::UNK, CRec::UNK,
	CRec::UNK, CRec::UNK , CRec::UNK, CRec::UNK
};

#define zr Cpu.gpr[0]
#define at Cpu.gpr[1]
#define v0 Cpu.gpr[2]
#define v1 Cpu.gpr[3]
#define a0 Cpu.gpr[4]
#define a1 Cpu.gpr[5]
#define a2 Cpu.gpr[6]
#define a3 Cpu.gpr[7]
#define t0 Cpu.gpr[8]
#define t1 Cpu.gpr[9]
#define t2 Cpu.gpr[10]
#define t3 Cpu.gpr[11]
#define t4 Cpu.gpr[12]
#define t5 Cpu.gpr[13]
#define t6 Cpu.gpr[14]
#define t7 Cpu.gpr[15]
#define s0 Cpu.gpr[16]
#define s1 Cpu.gpr[17]
#define s2 Cpu.gpr[18]
#define s3 Cpu.gpr[19]
#define s4 Cpu.gpr[20]
#define s5 Cpu.gpr[21]
#define s6 Cpu.gpr[22]
#define s7 Cpu.gpr[23]
#define t8 Cpu.gpr[24]
#define t9 Cpu.gpr[25]
#define k0 Cpu.gpr[26]
#define k1 Cpu.gpr[27]
#define gp Cpu.gpr[28]
#define sp Cpu.gpr[29]
#define fp Cpu.gpr[30]
#define ra Cpu.gpr[31]

void CRec::RecBlock1() {
	sp -= 2528;
	a0 = sp;
	Cpu.instr = 0x00082FCC;
	Cpu.SYSCALL();
	s1 = 0x08A00000;
	s2 = 0x08B00000;
	s0 = 0x44000000;
	a0 = 0;
	a1 = 480;
	a2 = 272;
	Cpu.instr = 0x00084E8C;
	Cpu.SYSCALL();
	v0 = s1;
	t0 = 0xB600;
	Memory.Write8(v0, 0);
	t0 -= 1;
	if ((s32)t0 >= 0) {
		v0 += 1;
		Cpu.pc = 0x08900080;
	}
	else Cpu.pc = 0x0890008C;
}

void CRec::RecBlock2() {
	Memory.Write8(v0, 0);
	t0 -= 1;
	if ((s32)t0 >= 0) {
		v0 += 1;
		Cpu.pc = 0x08900080;
	}
	else Cpu.pc = 0x0890008C;
}

void CRec::RecBlock3() {
	v0 += 1;
	s4 = s1 | 0x0000B400;
	s3 = s4 + 511;
	a0 = sp;
	Cpu.instr = 0x0008300C;
	Cpu.SYSCALL();
	s4 += 1;
	v1 = s4 < s3;
	if (v1 != 0) {
		Memory.Write8(s4, v0);
		Cpu.pc = 0x08900098;
	} else Cpu.pc = 0x089000AC;
}

void CRec::RecBlock4() {
	a0 = sp;
	Cpu.instr = 0x0008300C;
	Cpu.SYSCALL();
	s4 += 1;
	v1 = s4 < s3;
	if (v1 != 0) {
		Memory.Write8(s4, v0);
		Cpu.pc = 0x08900098;
	} else Cpu.pc = 0x089000AC;
}

void CRec::RecBlock5() {
	Memory.Write8(s4, v0);
	s5 = 510;
	a0 = sp;
	Cpu.instr = 0x0008300C;
	Cpu.SYSCALL();
	v1 = 89;
	if (v1 != 0) {
		Cpu.lo = v0 / v1;
		Cpu.hi = v0 % v1;
		Cpu.pc = 0x089000CC;
	} else Cpu.pc = 0x089000C4;
}

void CRec::RecBlock6() {
	v0 = Cpu.lo;
	v1 = Cpu.hi;
	v1 += 1;
	v1 <<= 9;
	s4 = v1 + s1;
	v0 &= s5;
	v0 += 1;
	s4 += v0;
	Memory.Write8(s4, s5);
	t0 = 89;
	t1 = s1 | 0xB200;
	t3 = s2 | 0xB200;
	t2 = t1 + 510;
	a0 = Memory.Read8(t1 + 512);
	a1 = Memory.Read8(t1 + 513);
	a0 += a1;
	a1 = Memory.Read8(t1 + 514);
	a0 += a1;
	a1 = Memory.Read8(t1 + 1);
	a0 += a1;
	a0 >>= 2;
	if (a0 > 0)
		a0 -= 1;
	Cpu.pc = 0x8900128;
}

void CRec::RecBlock7() {
	Memory.Write8(t3 + 1, a0);
	t1 += 1;
	v0 = t1 < t2;
}

void CRec::Initialize() {
	//recBlk = new u8[BlockSize];
	recBlk = (u8 *)VirtualAlloc(
		NULL, BlockSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	recRam = new RecFunc[RamSize];
	recTlb = new RecFunc *[PageCount];

	ZeroMemory(recBlk, BlockSize);
	ZeroMemory(recRam, sizeof(RecFunc) * RamSize);
	ZeroMemory(recTlb, sizeof(RecFunc *) * PageCount);

	recPtr = recBlk;

	u32 page = RamStart >> PageShift;
	for (u32 i = 0; i < (RamSize >> PageShift); ++i) {
		RecFunc *addr = recRam + (i << PageShift);
		recTlb[0x00000 + page + i] = addr;
		recTlb[0x40000 + page + i] = addr;
		recTlb[0x80000 + page + i] = addr;
	}

	/*
	recTlb[0x08900][0x050] = RecBlock1;
	recTlb[0x08900][0x080] = RecBlock2;
	recTlb[0x08900][0x08C] = RecBlock3;
	recTlb[0x08900][0x098] = RecBlock4;
	recTlb[0x08900][0x0AC] = RecBlock5;
	recTlb[0x08900][0x0CC] = RecBlock6;
	recTlb[0x08900][0x128] = RecBlock8;*/
}

void CRec::Finalize() {
	recPtr = 0;
	if (recBlk) {
		//delete[] recBlk;
		VirtualFree(recBlk, BlockSize, MEM_RELEASE);
		recBlk = 0;
	}
	if (recRam) {
		delete[] recRam;
		recRam = 0;
	}
	if (recTlb) {
		delete[] recTlb;
		recTlb = 0;
	}
}

void CRec::RunBlock() {
	u32 page = Cpu.pc >> PageShift;
	u32 ofs = Cpu.pc & PageMask;
	RecFunc func = recTlb[page][ofs];
	if (!func) {
		func = (RecFunc)recPtr;
		recTlb[page][ofs] = func;
		cycles = 0;
		branch = false;
		while (!branch) {
			Cpu.npc = Cpu.pc + 4;
			++cycles;
			Recompile();
			Cpu.pc = Cpu.npc;
		}
		Cpu.pc = (page << PageShift) | ofs;
	}
	func();
}

void CRec::Recompile() {
	Cpu.instr = Memory.Read32(Cpu.pc);
	mainTable[Cpu.instr >> 26]();
}

void CRec::UNK() {
	assert(false);
}

void CRec::SPECIAL() {
	special[Cpu.instr & 0x3F]();
}

void CRec::SLL() {
	if (!_Rd_) return;

	// mov eax, Rt
	*Rec.recPtr = 0xA1;
	*(u32 *)(Rec.recPtr + 1) = Rt;

	// shl eax, Sa
	*(u16 *)(Rec.recPtr + 5) = 0xE0C1;
	*(Rec.recPtr + 7) = Sa;

	// mov Rd, eax
	*(Rec.recPtr + 8) = 0xA3;
	*(u32 *)(Rec.recPtr + 9) = Rd;
	Rec.recPtr += 13;
}

void CRec::SRL() {
	if (!_Rd_) return;
	if (_Rs_) assert(0);

	// mov eax, Rt
	*Rec.recPtr = 0xA1;
	*(u32 *)(Rec.recPtr + 1) = Rt;

	// shr eax, Sa
	*(u16 *)(Rec.recPtr + 5) = 0xE8C1;
	*(Rec.recPtr + 7) = Sa;

	// mov Rd, eax
	*(Rec.recPtr + 8) = 0xA3;
	*(u32 *)(Rec.recPtr + 9) = Rd;
	Rec.recPtr += 13;
}

void CRec::SYSCALL() {
	if (Im20 == 0x020BF) {
		// push Cpu.gpr[4]
		*(u16 *)Rec.recPtr = 0x35FF;
		*(u32 *)(Rec.recPtr + 2) = (u32)&Cpu.gpr[4];
		Rec.recPtr += 6;
		
		// call srand
		*Rec.recPtr = 0xE8;
		*(u32 *)(Rec.recPtr + 1) = (u32)srand - (u32)Rec.recPtr - 5;

		// add esp, 4
		*(Rec.recPtr + 5) = 0x83;
		*(u16 *)(Rec.recPtr + 6) = 0x04C4;

		// xor eax, eax
		*(u16 *)(Rec.recPtr + 8) = 0xC033;

		// mov Cpu.gpr[2], eax
		*(Rec.recPtr + 10) = 0xA3;
		*(u32 *)(Rec.recPtr + 11) = (u32)&Cpu.gpr[2];
		Rec.recPtr += 15;
	}
	else if (Im20 == 0x020C0) {
		// call rand
		*Rec.recPtr = 0xE8;
		*(u32 *)(Rec.recPtr + 1) = (u32)rand - (u32)Rec.recPtr - 5;

		// push eax
		*(Rec.recPtr + 5) = 0x50;
		Rec.recPtr += 6;

		// call rand
		*Rec.recPtr = 0xE8;
		*(u32 *)(Rec.recPtr + 1) = (u32)rand - (u32)Rec.recPtr - 5;

		// cdq | pop ecx | mul ecx
		*(u32 *)(Rec.recPtr + 5) = 0xE1F75999;

		// mov Cpu.gpr[2], eax
		*(Rec.recPtr + 9) = 0xA3;
		*(u32 *)(Rec.recPtr + 10) = (u32)&Cpu.gpr[2];
		Rec.recPtr += 14;
	}
#ifdef _DEBUG
	else if (Im20 == 0x0213A) {
		// mov ecx, 
	}
#endif
	else {
		*(u16 *)Rec.recPtr = 0x05C7;
		*(u32 *)(Rec.recPtr + 2) = (u32)&Cpu.instr;
		*(u32 *)(Rec.recPtr + 6) = Cpu.instr;

		*(u16 *)(Rec.recPtr + 10) = 0x0581;
		*(u32 *)(Rec.recPtr + 12) = (u32)&Cpu.cycles;
		*(u32 *)(Rec.recPtr + 16) = Rec.cycles;
		Rec.recPtr += 20;
		Rec.cycles = 0;

		*Rec.recPtr = 0xE8;
		*(u32 *)(Rec.recPtr + 1) = (u32)CCpu::SYSCALL - (u32)Rec.recPtr - 5;
		Rec.recPtr += 5;
	}
}

void CRec::MFHI() {
	if (!_Rd_) return;

	// mov eax, hi
	*Rec.recPtr = 0xA1;
	*(u32 *)(Rec.recPtr + 1) = (u32)&Cpu.hi;

	// mov Rd, eax
	*(Rec.recPtr + 5) = 0xA3;
	*(u32 *)(Rec.recPtr + 6) = Rd;
	Rec.recPtr += 10;
}

void CRec::MFLO() {
	if (!_Rd_) return;

	// mov eax, lo
	*Rec.recPtr = 0xA1;
	*(u32 *)(Rec.recPtr + 1) = (u32)&Cpu.lo;

	// mov Rd, eax
	*(Rec.recPtr + 5) = 0xA3;
	*(u32 *)(Rec.recPtr + 6) = Rd;
	Rec.recPtr += 10;
}

void CRec::DIVU() {
	if (!_Rt_) return;
	
	// mov eax, Rs
	*Rec.recPtr = 0xA1;
	*(u32 *)(Rec.recPtr + 1) = Rs;

	// cdq
	*(Rec.recPtr + 5) = 0x99;

	// div Rt
	*(u16 *)(Rec.recPtr + 6) = 0x35F7;
	*(u32 *)(Rec.recPtr + 8) = Rt;

	// mov lo, eax
	*(Rec.recPtr + 12) = 0xA3;
	*(u32 *)(Rec.recPtr + 13) = (u32)&Cpu.lo;

	// mov hi, edx
	*(u16 *)(Rec.recPtr + 17) = 0x1589;
	*(u32 *)(Rec.recPtr + 19) = (u32)&Cpu.hi;
	Rec.recPtr += 23;
}

void CRec::ADD() {
	CRec::ADDU();
}

void CRec::ADDU() {
	if (!_Rd_) return;

	// mov eax, Rs
	*Rec.recPtr = 0xA1;
	*(u32 *)(Rec.recPtr + 1) = Rs;

	// add eax, Rt
	*(u16 *)(Rec.recPtr + 5) = 0x0503;
	*(u32 *)(Rec.recPtr + 7) = Rt;

	// mov Rd, eax
	*(Rec.recPtr + 11) = 0xA3;
	*(u32 *)(Rec.recPtr + 12) = Rd;
	Rec.recPtr += 16;
}

void CRec::AND() {
	if (!_Rd_) return;

	// mov eax, Rs
	*Rec.recPtr = 0xA1;
	*(u32 *)(Rec.recPtr + 1) = Rs;

	// and eax, Rt
	*(u16 *)(Rec.recPtr + 5) = 0x0523;
	*(u32 *)(Rec.recPtr + 7) = Rs;
	
	// mov Rd, eax
	*(Rec.recPtr + 11) = 0xA3;
	*(u32 *)(Rec.recPtr + 12) = Rd;
	Rec.recPtr += 16;
}

void CRec::XOR() {
	if (!_Rd_) return;

	// mov eax, Rs
	*Rec.recPtr = 0xA1;
	*(u32 *)(Rec.recPtr + 1) = Rs;

	// xor eax, Rt
	*(u16 *)(Rec.recPtr + 5) = 0x0533;
	*(u32 *)(Rec.recPtr + 7) = Rt;
	
	// mov Rd, eax
	*(Rec.recPtr + 11) = 0xA3;
	*(u32 *)(Rec.recPtr + 12) = Rd;
	Rec.recPtr += 16;
}

void CRec::SLTU() {
	if (!_Rd_) return;

	// mov eax, Rs
	*Rec.recPtr = 0xA1;
	*(u32 *)(Rec.recPtr + 1) = Rs;

	// cmp eax, Rt
	*(u16 *)(Rec.recPtr + 5) = 0x053B;
	*(u32 *)(Rec.recPtr + 7) = Rt;

	// setl al
	*(Rec.recPtr + 11) = 0x0F;
	*(u16 *)(Rec.recPtr + 12) = 0xC09C;

	// movzx eax, al
	*(Rec.recPtr + 14) = 0x0F;
	*(u16 *)(Rec.recPtr + 15) = 0xC0B6;
		
	// mov Rd, eax
	*(Rec.recPtr + 17) = 0xA3;
	*(u32 *)(Rec.recPtr + 18) = Rd;
	Rec.recPtr += 22;
}

void CRec::REGIMM() {
	regimm[_Rt_]();
}

void CRec::BGEZ() {
	Rec.branch = true;

	Cpu.pc += 4;
	Cpu.npc = Cpu.pc + ((s32)(s16)Im << 2);

	// mov eax, Rs
	*Rec.recPtr = 0xA1;
	*(u32 *)(Rec.recPtr + 1) = Rs;

	// cmp eax, 0
	*(u16 *)(Rec.recPtr + 5) = 0xF883;
	*(Rec.recPtr + 7) = 0x00;

	// jl
	*(Rec.recPtr + 8) = 0x7C;
	Rec.recPtr += 10; // 9+1
	u8 *jPtr = Rec.recPtr - 1;

	Rec.Recompile();

	*(u16 *)Rec.recPtr = 0x05C7;
	*(u32 *)(Rec.recPtr + 2) = (u32)&Cpu.pc;
	*(u32 *)(Rec.recPtr + 6) = Cpu.npc;

	*(u16 *)(Rec.recPtr + 10) = 0x0581;
	*(u32 *)(Rec.recPtr + 12) = (u32)&Cpu.cycles;
	*(u32 *)(Rec.recPtr + 16) = Rec.cycles + 1;
	*(Rec.recPtr + 20) = 0xC3;
	Rec.recPtr += 21;

	*jPtr = (u32)Rec.recPtr - (u32)jPtr - 1;

	*(u16 *)Rec.recPtr = 0x05C7;
	*(u32 *)(Rec.recPtr + 2) = (u32)&Cpu.pc;
	*(u32 *)(Rec.recPtr + 6) = Cpu.pc;
	
	*(u16 *)(Rec.recPtr + 10) = 0x0581;
	*(u32 *)(Rec.recPtr + 12) = (u32)&Cpu.cycles;
	*(u32 *)(Rec.recPtr + 16) = Rec.cycles;
	*(Rec.recPtr + 20) = 0xC3;
	Rec.recPtr += 21;
}

void CRec::J() {
	Cpu.pc += 4;
	Cpu.npc = (Cpu.pc & 0xF0000000) | ((Cpu.instr & 0x03FFFFFF) << 2);
	Rec.Recompile();

	*(u16 *)Rec.recPtr = 0x05C7;
	*(u32 *)(Rec.recPtr + 2) = (u32)&Cpu.pc;
	*(u32 *)(Rec.recPtr + 6) = Cpu.npc;
	
	*(u16 *)(Rec.recPtr + 10) = 0x0581;
	*(u32 *)(Rec.recPtr + 12) = (u32)&Cpu.cycles;
	*(u32 *)(Rec.recPtr + 16) = Rec.cycles + 1;
	*(Rec.recPtr + 20) = 0xC3;
	Rec.recPtr += 21;
}

void CRec::BNE() {
	Rec.branch = true;

	Cpu.pc += 4;
	Cpu.npc = Cpu.pc + ((s32)(s16)Im << 2);

	// mov eax, Rs
	*Rec.recPtr = 0xA1;
	*(u32 *)(Rec.recPtr + 1) = Rs;

	// cmp eax, Rt
	*(u16 *)(Rec.recPtr + 5) = 0x053B;
	*(u32 *)(Rec.recPtr + 7) = Rt;

	// je
	*(Rec.recPtr + 11) = 0x74;
	Rec.recPtr += 13; // 12+1
	u8 *jPtr = Rec.recPtr - 1;

	Rec.Recompile();

	*(u16 *)Rec.recPtr = 0x05C7;
	*(u32 *)(Rec.recPtr + 2) = (u32)&Cpu.pc;
	*(u32 *)(Rec.recPtr + 6) = Cpu.npc;
	
	*(u16 *)(Rec.recPtr + 10) = 0x0581;
	*(u32 *)(Rec.recPtr + 12) = (u32)&Cpu.cycles;
	*(u32 *)(Rec.recPtr + 16) = Rec.cycles + 1;
	*(Rec.recPtr + 20) = 0xC3;
	Rec.recPtr += 21;

	*jPtr = (u32)Rec.recPtr - (u32)jPtr - 1;

	*(u16 *)Rec.recPtr = 0x05C7;
	*(u32 *)(Rec.recPtr + 2) = (u32)&Cpu.pc;
	*(u32 *)(Rec.recPtr + 6) = Cpu.pc;
	
	*(u16 *)(Rec.recPtr + 10) = 0x0581;
	*(u32 *)(Rec.recPtr + 12) = (u32)&Cpu.cycles;
	*(u32 *)(Rec.recPtr + 16) = Rec.cycles;
	*(Rec.recPtr + 20) = 0xC3;
	Rec.recPtr += 21;
}

void CRec::BGTZ() {
	Rec.branch = true;

	Cpu.pc += 4;
	Cpu.npc = Cpu.pc + ((s32)(s16)Im << 2);

	// mov eax, Rs
	*Rec.recPtr = 0xA1;
	*(u32 *)(Rec.recPtr + 1) = Rs;

	// cmp eax, 0
	*(u16 *)(Rec.recPtr + 5) = 0xF883;
	*(Rec.recPtr + 7) = 0x00;

	// jle
	*(Rec.recPtr + 8) = 0x7E;
	Rec.recPtr += 10; // 9+1
	u8 *jPtr = Rec.recPtr - 1;

	Rec.Recompile();

	*(u16 *)Rec.recPtr = 0x05C7;
	*(u32 *)(Rec.recPtr + 2) = (u32)&Cpu.pc;
	*(u32 *)(Rec.recPtr + 6) = Cpu.npc;
	
	*(u16 *)(Rec.recPtr + 10) = 0x0581;
	*(u32 *)(Rec.recPtr + 12) = (u32)&Cpu.cycles;
	*(u32 *)(Rec.recPtr + 16) = Rec.cycles + 1;
	*(Rec.recPtr + 20) = 0xC3;
	Rec.recPtr += 21;

	*jPtr = (u32)Rec.recPtr - (u32)jPtr - 1;

	*(u16 *)Rec.recPtr = 0x05C7;
	*(u32 *)(Rec.recPtr + 2) = (u32)&Cpu.pc;
	*(u32 *)(Rec.recPtr + 6) = Cpu.pc;

	*(u16 *)(Rec.recPtr + 10) = 0x0581;
	*(u32 *)(Rec.recPtr + 12) = (u32)&Cpu.cycles;
	*(u32 *)(Rec.recPtr + 16) = Rec.cycles;
	*(Rec.recPtr + 20) = 0xC3;
	Rec.recPtr += 21;
}

void CRec::ADDIU() {
	if (!_Rt_) return;

	// mov eax, Rs
	*Rec.recPtr = 0xA1;
	*(u32 *)(Rec.recPtr + 1) = Rs;

	// add eax, Im
	*(Rec.recPtr + 5) = 0x05;
	*(u32 *)(Rec.recPtr + 6) = (s32)(s16)Im;
	
	// mov Rt, eax
	*(Rec.recPtr + 10) = 0xA3;
	*(u32 *)(Rec.recPtr + 11) = Rt;
	Rec.recPtr += 15;
}

void CRec::SLTIU() {
	if (!_Rt_) return;

	// mov eax, Rs
	*Rec.recPtr = 0xA1;
	*(u32 *)(Rec.recPtr + 1) = Rs;

	*(Rec.recPtr + 5) = 0x3D;
	*(u32 *)(Rec.recPtr + 6) = Im;

	// setl al
	*(Rec.recPtr + 10) = 0x0F;
	*(u16 *)(Rec.recPtr + 11) = 0xC09C;

	// movzx eax, al
	*(Rec.recPtr + 13) = 0x0F;
	*(u16 *)(Rec.recPtr + 14) = 0xC0B6;
		
	// mov Rt, eax
	*(Rec.recPtr + 16) = 0xA3;
	*(u32 *)(Rec.recPtr + 17) = Rt;
	Rec.recPtr += 21;
}

void CRec::ORI() {
	if (!_Rt_) return;

	// mov eax, Rs
	*Rec.recPtr = 0xA1;
	*(u32 *)(Rec.recPtr + 1) = Rs;

	// or eax, Im
	*(Rec.recPtr + 5) = 0x0D;
	*(u32 *)(Rec.recPtr + 6) = Im;

	// mov Rt, eax
	*(Rec.recPtr + 10) = 0xA3;
	*(u32 *)(Rec.recPtr + 11) = Rt;
	Rec.recPtr += 15;
}

void CRec::LUI() {
	if (!_Rt_) return;
	// mov Rt, Im << 16
	*(u16 *)Rec.recPtr = 0x05C7;
	*(u32 *)(Rec.recPtr + 2) = Rt;
	*(u32 *)(Rec.recPtr + 6) = Cpu.instr << 16;
	Rec.recPtr += 10;
}

void CRec::BNEL() {
	Rec.branch = true;

	Cpu.pc += 4;
	Cpu.npc = Cpu.pc + ((s32)(s16)Im << 2);

	// mov eax, Rs
	*Rec.recPtr = 0xA1;
	*(u32 *)(Rec.recPtr + 1) = Rs;

	// cmp eax, Rt
	*(u16 *)(Rec.recPtr + 5) = 0x053B;
	*(u32 *)(Rec.recPtr + 7) = Rt;

	// je
	*(Rec.recPtr + 11) = 0x74;
	Rec.recPtr += 13; // 12+1
	u8 *jPtr = Rec.recPtr - 1;

	Rec.Recompile();

	*(u16 *)Rec.recPtr = 0x05C7;
	*(u32 *)(Rec.recPtr + 2) = (u32)&Cpu.pc;
	*(u32 *)(Rec.recPtr + 6) = Cpu.npc;
	
	*(u16 *)(Rec.recPtr + 10) = 0x0581;
	*(u32 *)(Rec.recPtr + 12) = (u32)&Cpu.cycles;
	*(u32 *)(Rec.recPtr + 16) = Rec.cycles + 1;
	*(Rec.recPtr + 20) = 0xC3;
	Rec.recPtr += 21;

	*jPtr = (u32)Rec.recPtr - (u32)jPtr - 1;

	*(u16 *)Rec.recPtr = 0x05C7;
	*(u32 *)(Rec.recPtr + 2) = (u32)&Cpu.pc;
	*(u32 *)(Rec.recPtr + 6) = Cpu.pc + 4;
	
	*(u16 *)(Rec.recPtr + 10) = 0x0581;
	*(u32 *)(Rec.recPtr + 12) = (u32)&Cpu.cycles;
	*(u32 *)(Rec.recPtr + 16) = Rec.cycles;
	*(Rec.recPtr + 20) = 0xC3;
	Rec.recPtr += 21;
}

void CRec::BGTZL() {
	Rec.branch = true;

	Cpu.pc += 4;
	Cpu.npc = Cpu.pc + ((s32)(s16)Im << 2);

	// mov eax, Rs
	*Rec.recPtr = 0xA1;
	*(u32 *)(Rec.recPtr + 1) = Rs;

	// cmp eax, 0
	*(u16 *)(Rec.recPtr + 5) = 0xF883;
	*(Rec.recPtr + 7) = 0x00;

	// jle
	*(Rec.recPtr + 8) = 0x7E;
	Rec.recPtr += 10; // 9+1
	u8 *jPtr = Rec.recPtr - 1;

	Rec.Recompile();

	*(u16 *)Rec.recPtr = 0x05C7;
	*(u32 *)(Rec.recPtr + 2) = (u32)&Cpu.pc;
	*(u32 *)(Rec.recPtr + 6) = Cpu.npc;
	
	*(u16 *)(Rec.recPtr + 10) = 0x0581;
	*(u32 *)(Rec.recPtr + 12) = (u32)&Cpu.cycles;
	*(u32 *)(Rec.recPtr + 16) = Rec.cycles + 1;
	*(Rec.recPtr + 20) = 0xC3;
	Rec.recPtr += 21;

	*jPtr = (u32)Rec.recPtr - (u32)jPtr - 1;

	*(u16 *)Rec.recPtr = 0x05C7;
	*(u32 *)(Rec.recPtr + 2) = (u32)&Cpu.pc;
	*(u32 *)(Rec.recPtr + 6) = Cpu.pc + 4;
	
	*(u16 *)(Rec.recPtr + 10) = 0x0581;
	*(u32 *)(Rec.recPtr + 12) = (u32)&Cpu.cycles;
	*(u32 *)(Rec.recPtr + 16) = Rec.cycles;
	*(Rec.recPtr + 20) = 0xC3;
	Rec.recPtr += 21;
}

void CRec::LW() {
	if (!_Rt_) return;
	/*
	*(u16 *)Rec.recPtr = 0x05C7;
	*(u32 *)(Rec.recPtr + 2) = (u32)&Cpu.instr;
	*(u32 *)(Rec.recPtr + 6) = Cpu.instr;
	Rec.recPtr += 10;

	*Rec.recPtr = 0xE8;
	*(u32 *)(Rec.recPtr + 1) = (u32)&CCpu::LW - (u32)Rec.recPtr - 5;
	Rec.recPtr += 5;*/

	// mov eax, Rs
	*Rec.recPtr = 0xA1;
	*(u32 *)(Rec.recPtr + 1) = Rs;

	// add eax, Im
	*(Rec.recPtr + 5) = 0x05;
	*(u32 *)(Rec.recPtr + 6) = (s32)(s16)Im;

	// mov edx, eax
	*(u16 *)(Rec.recPtr + 10) = 0xD08B;

	// shr eax, 12
	*(u16 *)(Rec.recPtr + 12) = 0xE8C1;
	*(Rec.recPtr + 14) = 0x0C;

	// and edx, 0x00000FFF
	*(u16 *)(Rec.recPtr + 15) = 0xE281;
	*(u32 *)(Rec.recPtr + 17) = 0x00000FFF;

	// mov eax, [tlb + eax * 4]
	*(Rec.recPtr + 21) = 0x8B;
	*(u16 *)(Rec.recPtr + 22) = 0x8504;
	*(u32 *)(Rec.recPtr + 24) = (u32)Memory.tlb;

	// mov eax, [eax + edx]
	*(Rec.recPtr + 28) = 0x8B;
	*(u16 *)(Rec.recPtr + 29) = 0x1004;

	// mov Rt, eax
	*(Rec.recPtr + 31) = 0xA3;
	*(u32 *)(Rec.recPtr + 32) = Rt;
	Rec.recPtr += 36;
}

void CRec::LBU() {
	/*
	*(u16 *)Rec.recPtr = 0x05C7;
	*(u32 *)(Rec.recPtr + 2) = (u32)&Cpu.instr;
	*(u32 *)(Rec.recPtr + 6) = Cpu.instr;
	Rec.recPtr += 10;

	*Rec.recPtr = 0xE8;
	*(u32 *)(Rec.recPtr + 1) = (u32)&CCpu::LBU - (u32)Rec.recPtr - 5;
	Rec.recPtr += 5;*/

	// mov eax, Rs
	*Rec.recPtr = 0xA1;
	*(u32 *)(Rec.recPtr + 1) = Rs;

	// add eax, Im
	*(Rec.recPtr + 5) = 0x05;
	*(u32 *)(Rec.recPtr + 6) = (s32)(s16)Im;

	// mov edx, eax
	*(u16 *)(Rec.recPtr + 10) = 0xD08B;

	// shr eax, 12
	*(u16 *)(Rec.recPtr + 12) = 0xE8C1;
	*(Rec.recPtr + 14) = 0x0C;

	// and edx, 0x00000FFF
	*(u16 *)(Rec.recPtr + 15) = 0xE281;
	*(u32 *)(Rec.recPtr + 17) = 0x00000FFF;

	// mov eax, [tlb + eax * 4]
	*(Rec.recPtr + 21) = 0x8B;
	*(u16 *)(Rec.recPtr + 22) = 0x8504;
	*(u32 *)(Rec.recPtr + 24) = (u32)Memory.tlb;

	// movzx eax, byte ptr [eax + edx]
	*(u32 *)(Rec.recPtr + 28) = 0x1004B60F;

	// mov Rt, eax
	*(Rec.recPtr + 32) = 0xA3;
	*(u32 *)(Rec.recPtr + 33) = Rt;
	Rec.recPtr += 37;
}

void CRec::SB() {
	/*
	*(u16 *)Rec.recPtr = 0x05C7;
	*(u32 *)(Rec.recPtr + 2) = (u32)&Cpu.instr;
	*(u32 *)(Rec.recPtr + 6) = Cpu.instr;
	Rec.recPtr += 10;

	*Rec.recPtr = 0xE8;
	*(u32 *)(Rec.recPtr + 1) = (u32)&CCpu::SB - (u32)Rec.recPtr - 5;
	Rec.recPtr += 5;*/

	// mov cl, Rt
	*(u16 *)Rec.recPtr = 0x0D8A;
	*(u32 *)(Rec.recPtr + 2) = Rt;

	// mov eax, Rs
	*(Rec.recPtr + 6) = 0xA1;
	*(u32 *)(Rec.recPtr + 7) = Rs;

	// add eax, Im
	*(Rec.recPtr + 11) = 0x05;
	*(u32 *)(Rec.recPtr + 12) = (s32)(s16)Im;

	// mov edx, eax
	*(u16 *)(Rec.recPtr + 16) = 0xD08B;

	// shr eax, 12
	*(u16 *)(Rec.recPtr + 18) = 0xE8C1;
	*(Rec.recPtr + 20) = 0x0C;

	// and edx, 0x00000FFF
	*(u16 *)(Rec.recPtr + 21) = 0xE281;
	*(u32 *)(Rec.recPtr + 23) = 0x00000FFF;

	// mov eax, [tlb + eax * 4]
	*(Rec.recPtr + 27) = 0x8B;
	*(u16 *)(Rec.recPtr + 28) = 0x8504;
	*(u32 *)(Rec.recPtr + 30) = (u32)Memory.tlb;

	// mov byte ptr [eax + edx], cl
	*(Rec.recPtr + 34) = 0x88;
	*(u16 *)(Rec.recPtr + 35) = 0x100C;
	Rec.recPtr += 37;
}

void CRec::SW() {
	/*
	*(u16 *)Rec.recPtr = 0x05C7;
	*(u32 *)(Rec.recPtr + 2) = (u32)&Cpu.instr;
	*(u32 *)(Rec.recPtr + 6) = Cpu.instr;
	Rec.recPtr += 10;

	*Rec.recPtr = 0xE8;
	*(u32 *)(Rec.recPtr + 1) = (u32)&CCpu::SW - (u32)Rec.recPtr - 5;
	Rec.recPtr += 5;*/

	// mov ecx, Rt
	*(u16 *)Rec.recPtr = 0x0D8B;
	*(u32 *)(Rec.recPtr + 2) = Rt;

	// mov eax, Rs
	*(Rec.recPtr + 6) = 0xA1;
	*(u32 *)(Rec.recPtr + 7) = Rs;

	// add eax, Im
	*(Rec.recPtr + 11) = 0x05;
	*(u32 *)(Rec.recPtr + 12) = (s32)(s16)Im;

	// mov edx, eax
	*(u16 *)(Rec.recPtr + 16) = 0xD08B;

	// shr eax, 12
	*(u16 *)(Rec.recPtr + 18) = 0xE8C1;
	*(Rec.recPtr + 20) = 0x0C;

	// and edx, 0x00000FFF
	*(u16 *)(Rec.recPtr + 21) = 0xE281;
	*(u32 *)(Rec.recPtr + 23) = 0x00000FFF;

	// mov eax, [tlb + eax * 4]
	*(Rec.recPtr + 27) = 0x8B;
	*(u16 *)(Rec.recPtr + 28) = 0x8504;
	*(u32 *)(Rec.recPtr + 30) = (u32)Memory.tlb;

	// mov [eax + edx], ecx
	*(Rec.recPtr + 34) = 0x89;
	*(u16 *)(Rec.recPtr + 35) = 0x100C;
	Rec.recPtr += 37;
}
