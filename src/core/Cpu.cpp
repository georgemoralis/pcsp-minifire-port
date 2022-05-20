#include "PCSPCommon.h"
#include "Cpu.h"
#include "Memory.h"
#include "Video.h"
//#include "MainWindow.h"

#define _Rd_ ((Cpu.instr >> 11) & 0x1F)
#define _Rt_ ((Cpu.instr >> 16) & 0x1F)
#define _Rs_ ((Cpu.instr >> 21) & 0x1F)

#define Rd (Cpu.gpr[_Rd_])
#define Rt (Cpu.gpr[_Rt_])
#define Rs (Cpu.gpr[_Rs_])
#define Sa ((Cpu.instr >> 6) & 0x1F)
#define Im ((u16)Cpu.instr)
#define Im20 ((Cpu.instr >> 6) & 0x000FFFFF)

CCpu &Cpu = CCpu::GetSingleton();

void (* CCpu::mainTable[64])() = {
	CCpu::SPECIAL, CCpu::REGIMM, CCpu::J  , CCpu::UNK  ,
	CCpu::UNK    , CCpu::BNE   , CCpu::UNK, CCpu::BGTZ ,
	CCpu::UNK    , CCpu::ADDIU , CCpu::UNK, CCpu::SLTIU,
	CCpu::UNK    , CCpu::ORI   , CCpu::UNK, CCpu::LUI  ,
	CCpu::UNK    , CCpu::UNK   , CCpu::UNK, CCpu::UNK  ,
	CCpu::UNK    , CCpu::BNEL  , CCpu::UNK, CCpu::BGTZL,
	CCpu::UNK    , CCpu::UNK   , CCpu::UNK, CCpu::UNK  ,
	CCpu::UNK    , CCpu::UNK   , CCpu::UNK, CCpu::UNK  ,
	CCpu::UNK    , CCpu::UNK   , CCpu::UNK, CCpu::LW  ,
	CCpu::LBU    , CCpu::UNK   , CCpu::UNK, CCpu::UNK  ,
	CCpu::SB     , CCpu::UNK   , CCpu::UNK, CCpu::SW  ,
	CCpu::UNK    , CCpu::UNK   , CCpu::UNK, CCpu::UNK  ,
	CCpu::UNK    , CCpu::UNK   , CCpu::UNK, CCpu::UNK  ,
	CCpu::UNK    , CCpu::UNK   , CCpu::UNK, CCpu::UNK  ,
	CCpu::UNK    , CCpu::UNK   , CCpu::UNK, CCpu::UNK  ,
	CCpu::UNK    , CCpu::UNK   , CCpu::UNK, CCpu::UNK  ,
};

void (* CCpu::special[64])() = {
	CCpu::SLL    , CCpu::UNK , CCpu::SRL , CCpu::UNK ,
	CCpu::UNK    , CCpu::UNK , CCpu::UNK , CCpu::UNK ,
	CCpu::UNK    , CCpu::UNK , CCpu::UNK , CCpu::UNK ,
	CCpu::SYSCALL, CCpu::UNK , CCpu::UNK , CCpu::UNK ,
	CCpu::MFHI   , CCpu::UNK , CCpu::MFLO, CCpu::UNK ,
	CCpu::UNK    , CCpu::UNK , CCpu::UNK , CCpu::UNK ,
	CCpu::UNK    , CCpu::UNK , CCpu::UNK , CCpu::DIVU,
	CCpu::UNK    , CCpu::UNK , CCpu::UNK , CCpu::UNK ,
	CCpu::ADD    , CCpu::ADDU, CCpu::UNK , CCpu::UNK ,
	CCpu::AND    , CCpu::UNK , CCpu::XOR , CCpu::UNK ,
	CCpu::UNK    , CCpu::UNK , CCpu::UNK , CCpu::SLTU,
	CCpu::UNK    , CCpu::UNK , CCpu::UNK , CCpu::UNK ,
	CCpu::UNK    , CCpu::UNK , CCpu::UNK , CCpu::UNK ,
	CCpu::UNK    , CCpu::UNK , CCpu::UNK , CCpu::UNK ,
	CCpu::UNK    , CCpu::UNK , CCpu::UNK , CCpu::UNK ,
	CCpu::UNK    , CCpu::UNK , CCpu::UNK , CCpu::UNK ,
};

void (* CCpu::regimm[32])() = {
	CCpu::UNK, CCpu::BGEZ, CCpu::UNK, CCpu::UNK,
	CCpu::UNK, CCpu::UNK , CCpu::UNK, CCpu::UNK,
	CCpu::UNK, CCpu::UNK , CCpu::UNK, CCpu::UNK,
	CCpu::UNK, CCpu::UNK , CCpu::UNK, CCpu::UNK,
	CCpu::UNK, CCpu::UNK , CCpu::UNK, CCpu::UNK,
	CCpu::UNK, CCpu::UNK , CCpu::UNK, CCpu::UNK,
	CCpu::UNK, CCpu::UNK , CCpu::UNK, CCpu::UNK,
	CCpu::UNK, CCpu::UNK , CCpu::UNK, CCpu::UNK
};

void CCpu::Initialize() {
	ZeroMemory(gpr, sizeof(gpr));
	gpr[29] = 0x09FFC000;
	lo = 0;
	hi = 0;
	instr = 0;
	pc = 0x08900050; //skip Create Thread in minifire
	cycles = 0;
}

void CCpu::Step() {
	npc = pc + 4;
	Interpret();
	pc = npc;
	++cycles;
}

void CCpu::Interpret() {
	instr = Memory.Read32(pc);
	mainTable[instr >> 26]();
}

void CCpu::UNK() { assert(false); }

void CCpu::SPECIAL() { special[Cpu.instr & 0x3F](); }
void CCpu::SLL() { if (_Rd_) Rd = Rt << Sa; }
void CCpu::SRL() {
	if (!_Rd_) return;
	if (_Rs_)
		 Rd = _lrotr(Rt, Sa); 
	else
		Rd = Rt >> Sa;
}
void CCpu::SYSCALL() {
	switch (Im20) {
	case 0x020BF:
		srand(Cpu.gpr[4]);
		Cpu.gpr[2] = 0;
		break;
	case 0x020C0:
		Cpu.gpr[2] = (u32)rand() * (u32)rand();
		break;
	case 0x020EB:
		//MainWindow.Finalize();
		break;
	case 0x0213A:
		Cpu.gpr[2] = Video.sceSetDisplayMode(
			Cpu.gpr[4], Cpu.gpr[5], Cpu.gpr[6]);
		break;
	case 0x0213F:
		Cpu.gpr[2] = Video.sceDisplaySetFrameBuf(
			Cpu.gpr[4], Cpu.gpr[5],
			(PspDisplayPixelFormat)Cpu.gpr[6],
			(PspDisplaySetBufSync)Cpu.gpr[7]);
		break;
	case 0x02147: // sceDisplayWaitVblankStart
		Cpu.gpr[2] = 0;
		break;
	case 0x02150: // sceCtrlPeekBufferPositive
		{
			u32 i;
			u32 addr = Cpu.gpr[4];
			for (i = 0; i < Cpu.gpr[5]; ++i) {
				Memory.Write32(addr, 0);
				Memory.Write32(addr + 4, 0);
				Memory.Write8(addr + 8, 0);
				Memory.Write8(addr + 9, 0);
				addr += 16;
			}
			Cpu.gpr[2] = i;
		}
		break;
	default:
		assert(0);
	}
}
void CCpu::MFHI() { if (_Rd_) Rd = Cpu.hi; }
void CCpu::MFLO() { if (_Rd_) Rd = Cpu.lo; }
void CCpu::DIVU() { if (_Rt_) { Cpu.lo = Rs / Rt; Cpu.hi = Rs % Rt; } }
void CCpu::ADD()  { if (_Rd_) Rd = Rs + Rt; }
void CCpu::ADDU() { if (_Rd_) Rd = Rs + Rt; }
void CCpu::AND()  { if (_Rd_) Rd = Rs & Rt; }
void CCpu::XOR()  { if (_Rd_) Rd = Rs ^ Rt; }
void CCpu::SLTU() { if (_Rd_) Rd = Rs < Rt; }

void CCpu::REGIMM() { regimm[_Rt_](); }
void CCpu::BGEZ() {
	Cpu.pc += 4;
	if ((s32)Rs >= 0) {
		Cpu.npc = Cpu.pc + ((s32)(s16)Im << 2);
		Cpu.Interpret();
		++Cpu.cycles;
	}
}

void CCpu::J() {
	Cpu.pc += 4;
	Cpu.npc = (Cpu.pc & 0xF0000000) | ((Cpu.instr & 0x03FFFFFF) << 2);
	Cpu.Interpret();
	++Cpu.cycles;
}
void CCpu::BNE() {
	Cpu.pc += 4;
	if (Rs != Rt) {
		Cpu.npc = Cpu.pc + ((s32)(s16)Im << 2);
		Cpu.Interpret();
		++Cpu.cycles;
	}
}
void CCpu::BGTZ() {
	Cpu.pc += 4;
	if (Rs > 0) {
		Cpu.npc = Cpu.pc + ((s32)(s16)Im << 2);
		Cpu.Interpret();
		++Cpu.cycles;
	}
}
void CCpu::ADDIU() { if (_Rt_) Rt = Rs + (s16)Im; }
void CCpu::SLTIU() { if (_Rt_) Rt = Rs < Im; }
void CCpu::ORI()   { if (_Rt_) Rt = Rs | Im; }
void CCpu::LUI()   { if (_Rt_) Rt = Cpu.instr << 16; }
void CCpu::BNEL() {
	Cpu.pc += 4;
	if (Rs != Rt) {
		Cpu.npc = Cpu.pc + ((s32)(s16)Im << 2);
		Cpu.Interpret();
		++Cpu.cycles;
	}
	else Cpu.npc = Cpu.pc + 4;
}
void CCpu::BGTZL() {
	Cpu.pc += 4;
	if ((s32)Rs > 0) {
		Cpu.npc = Cpu.pc + ((s32)(s16)Im << 2);
		Cpu.Interpret();
		++Cpu.cycles;
	}
	else Cpu.npc = Cpu.pc + 4;
}

void CCpu::LW()  { u32 t = Memory.Read32(Rs + (s16)Im); if (_Rt_) Rt = t; }
void CCpu::LBU() { u8 t = Memory.Read8(Rs + (s16)Im); if (_Rt_)Rt = t; }
void CCpu::SB()  { Memory.Write8(Rs + (s16)Im, (u8)Rt); }
void CCpu::SW()  { Memory.Write32(Rs + (s16)Im, Rt); }
