/*
*  25/05/2022 - synced with jpcsp 23/05/2022 - 86c10922
*/
#pragma once

#define ELF_MAGIC = 0x464C457F;

enum Elf32HeaderMachine {
	E_MACHINE_SPARC   = 0x0002,
	E_MACHINE_x86     = 0x0003,
	E_MACHINE_MIPS    = 0x0008,
	E_MACHINE_PowerPC = 0x0014,
	E_MACHINE_ARM     = 0x0028,
	E_MACHINE_SuperH  = 0x002A,
	E_MACHINE_IA_64   = 0x0032,
	E_MACHINE_x86_64  = 0x003E,
	E_MACHINE_AArch64 = 0x00B7
};

enum Elf32HeaderType { ET_SCE_PRX = 0xFFA0 };

class Elf32Header
{
};

