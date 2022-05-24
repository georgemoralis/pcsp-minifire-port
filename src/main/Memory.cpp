#include "PCSPCommon.h"
#include "Memory.h"

CMemory &Memory = CMemory::GetSingleton();

bool CMemory::Initialize() {
	u32 i, page;

	scratchpad = (u8 *)_aligned_malloc(ScratchPadSize, 16);
	vram       = (u8 *)_aligned_malloc(VramSize, 16);
	ram        = (u8 *)_aligned_malloc(RamSize, 16);

	tlb = new u8 *[PageCount];

	ZeroMemory(scratchpad, ScratchPadSize);
	ZeroMemory(vram, VramSize);
	ZeroMemory(ram, RamSize);
	ZeroMemory(tlb, PageCount * sizeof(u8 *));
	
	page = ScratchPadStart >> PageShift;
	for (i = 0; i < (ScratchPadSize >> PageShift); ++i) {
		u8 *addr = scratchpad + (i << PageShift);
		tlb[0x00000 + page + i] = addr;
		tlb[0x40000 + page + i] = addr;
		tlb[0x80000 + page + i] = addr;
	}

	page = VramStart >> PageShift;
	for (i = 0; i < (VramSize >> PageShift); ++i) {
		u8 *addr = vram + (i << PageShift);
		tlb[0x00000 + page + i] = addr;
		tlb[0x40000 + page + i] = addr;
		tlb[0x80000 + page + i] = addr;
	}

	page = RamStart >> PageShift;
	for (i = 0; i < (RamSize >> PageShift); ++i) {
		u8 *addr = ram + (i << PageShift);
		tlb[0x00000 + page + i] = addr;
		tlb[0x40000 + page + i] = addr;
		tlb[0x80000 + page + i] = addr;
	}

	return true;
}

void CMemory::Finalize() {
	if (scratchpad) {
		_aligned_free(scratchpad);
		scratchpad = 0;
	}
	if (vram) {
		_aligned_free(vram);
		vram = 0;
	}
	if (ram) {
		_aligned_free(ram);
		ram = 0;
	}
	if (tlb) {
		delete[] tlb;
		tlb = 0;
	}
}
