/*
 *  26/05/2022 - synced with jpcsp 23/05/2022 - 86c10922 Completed!
 */
#include "..\PCSPCommon.h"
#include "Elf32Relocate.h"

Elf32Relocate::Elf32Relocate() {}

Elf32Relocate::~Elf32Relocate() {}

void Elf32Relocate::read(std::ifstream &f) { f.read((char *)&data, sizeof(data)); }

const size_t Elf32Relocate::sizeOf() { return sizeof(data); }

std::string Elf32Relocate::toString() const {
    std::string str;
    char tmp[128];

    sprintf(tmp, "r_offset 0x%08X\n", getR_offset());
    str.append(tmp);
    sprintf(tmp, "r_info   0x%08X\n", getR_info());
    str.append(tmp);

    return str;
}

u32 Elf32Relocate::getR_offset() const { return data.r_offset; }

void Elf32Relocate::setR_offset(u32 offset) { data.r_offset = offset; }

u32 Elf32Relocate::getR_info() const { return data.r_info; }

void Elf32Relocate::setR_info(u32 info) { data.r_info = info; }
