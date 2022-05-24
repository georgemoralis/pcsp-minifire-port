#pragma once

class CRec : public Singleton<CRec> {
	friend class Singleton<CRec>;
public:
	~CRec() { Finalize(); }

	void Initialize();
	void Finalize();
	void RunBlock();

	u8 *recPtr;

private:
	typedef void (* RecFunc)();

	CRec() { }

	void Recompile();

	static void RecBlock1();
	static void RecBlock2();
	static void RecBlock3();
	static void RecBlock4();
	static void RecBlock5();
	static void RecBlock6();
	static void RecBlock7();

	u8 *recBlk;
	RecFunc *recRam;
	RecFunc **recTlb;

	u32 cycles;
	bool branch;

	static const u32 BlockSize = 1024 * 1024;

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

extern CRec &Rec;
