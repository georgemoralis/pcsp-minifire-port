#pragma once

#include "Video.h"

const u32 PageShift = 12;
const u32 PageMask = (1 << PageShift) - 1;
const u32 PageCount = 1 << (32 - PageShift);

const u32 ScratchPadStart = 0x00010000;
const u32 ScratchPadSize  = 0x00004000;
const u32 ScratchPadEnd   = ScratchPadStart + ScratchPadSize - 1;

const u32 VramStart = 0x04000000;
const u32 VramSize  = 0x00200000;
const u32 VramEnd   = VramStart + VramSize - 1;

const u32 RamStart = 0x08000000;
const u32 RamSize  = 0x02000000;
const u32 RamEnd   = RamStart + RamSize - 1;

class CMemory : public Singleton<CMemory> {
	friend class Singleton<CMemory>;
	friend class CVideo;
	friend class CRec;
public:
	~CMemory() { Finalize(); }

	bool Initialize();
	void Finalize();

	inline u8 Read8(u32 addr) const {
		return tlb[addr >> PageShift][addr & PageMask];
	}

	inline u16 Read16(u32 addr) const {
		return *(u16 *)&tlb[addr >> PageShift][addr & PageMask];
	}

	inline u32 Read32(u32 addr) const {
		return *(u32 *)&tlb[addr >> PageShift][addr & PageMask];
	}

	inline void Write8(u32 addr, u8 value) {
		tlb[addr >> PageShift][addr & PageMask] = value;
	}

	inline void Write16(u32 addr, u16 value) {
		*(u16 *)&tlb[addr >> PageShift][addr & PageMask] = value;
	}

	inline void Write32(u32 addr, u32 value) {
		*(u32 *)&tlb[addr >> PageShift][addr & PageMask] = value;
	}

private:
	CMemory()
		: scratchpad(0), vram(0), ram(0), tlb(0) { }

	u8 *scratchpad;
	u8 *vram;
	u8 *ram;
	u8 **tlb;
};

extern CMemory &Memory;
