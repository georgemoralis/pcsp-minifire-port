#pragma once

class CCpu : public Singleton<CCpu> {
	friend class Singleton<CCpu>;
	friend class CRec;
public:
	void Initialize();
	void Step();
	void Interpret();

	u32 gpr[32];
	u32 lo, hi;
	u32 instr;
	u32 pc, npc;
	u32 cycles;

private:
	CCpu() { }

	static void (* mainTable[64])();
	static void (* special[64])();
	static void (* regimm[32])();

	static void UNK();

	static void SPECIAL();
	static void SLL();
	static void SRL();
	static void SYSCALL();
	static void MFHI();
	static void MFLO();
	static void DIVU();
	static void ADD();
	static void ADDU();
	static void AND();
	static void XOR();
	static void SLTU();

	static void REGIMM();
	static void BGEZ();

	static void J();
	static void BNE();
	static void BGTZ();
	static void ADDIU();
	static void SLTIU();
	static void ORI();
	static void LUI();
	static void BNEL();
	static void BGTZL();
	static void LW();
	static void LBU();
	static void SB();
	static void SW();
};

extern CCpu &Cpu;
