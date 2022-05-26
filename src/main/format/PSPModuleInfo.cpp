/*
 *  26/05/2022 - synced with jpcsp 23/05/2022 - 86c10922 - Small TODO requires
 */
#include "..\PCSPCommon.h"
#include "..\Memory.h"
#include "PSPModuleInfo.h"

PSPModuleInfo::PSPModuleInfo() {}

PSPModuleInfo::PSPModuleInfo(std::ifstream &f) { read(f); }

PSPModuleInfo::PSPModuleInfo(u32 address) { read(address); }

PSPModuleInfo::~PSPModuleInfo() {}

void PSPModuleInfo::read(std::ifstream &f) {
    f.read((char *)&data, sizeof(data));

    // Convert the array of bytes used for the module to C++ std::string.
    // Calculate the length of printable portion of the string, otherwise
    // any extra trailing characters may be printed as garbage.
    size_t len = 0;
    while (len < 28 && data.m_name[len] != 0) ++len;
    m_namez = std::string(data.m_name, 0, len);
}

void PSPModuleInfo::read(u32 address) { //JPCSP Inheritance that from pspAbstractMemoryMappedStructure
    memcpy(&data, Memory::getPointer(address), sizeof(data));

    size_t len = 0;
    while (len < 28 && data.m_name[len] != 0) ++len;
    m_namez = std::string(data.m_name, 0, len);
}
/*TODO*/  //	@Override
/*TODO*/  //	protected void write() {
/*TODO*/  //		write16((short) m_attr);
/*TODO*/  //		write16((short) m_version);
/*TODO*/  //		writeStringNZ(NAME_LENGTH, m_namez);
/*TODO*/  //		write32(m_gp);
/*TODO*/  //		write32(m_exports);
/*TODO*/  //		write32(m_exp_end);
/*TODO*/  //		write32(m_imports);
/*TODO*/  //		write32(m_imp_end);
/*TODO*/  //	}
/*TODO*/  //
u16 PSPModuleInfo::getM_attr() const { return data.m_attr; }

u16 PSPModuleInfo::getM_version() const { return data.m_version; }

u32 PSPModuleInfo::getM_gp() const { return data.m_gp; }

u32 PSPModuleInfo::getM_exports() const { return data.m_exports; }

u32 PSPModuleInfo::getM_exp_end() const { return data.m_exp_end; }

u32 PSPModuleInfo::getM_imports() const { return data.m_imports; }

u32 PSPModuleInfo::getM_imp_end() const { return data.m_imp_end; }

std::string PSPModuleInfo::getM_namez() const { return m_namez; }

const size_t PSPModuleInfo::sizeOf() { return sizeof(data); }
