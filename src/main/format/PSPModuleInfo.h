/*
 *  26/05/2022 - synced with jpcsp 23/05/2022 - 86c10922 - Small TODO requires
 */
#pragma once
class PSPModuleInfo //extends pspAbstractMemoryMappedStructure (JPCSP extends from that neccesary????)
{
   public:
    PSPModuleInfo();
    PSPModuleInfo(std::ifstream &f);
    PSPModuleInfo(u32 address);
    ~PSPModuleInfo();

    void read(std::ifstream &f);
    void read(u32 address);////JPCSP Inheritance that from pspAbstractMemoryMappedStructure

    u16 getM_attr() const;
    u16 getM_version() const;
    u32 getM_gp() const;
    u32 getM_exports() const;
    u32 getM_exp_end() const;
    u32 getM_imports() const;
    u32 getM_imp_end() const;
    std::string getM_namez() const;
    static const size_t sizeOf();

   private:
    std::string m_namez;  // string version of m_name
    struct {
        u16 m_attr;
        u16 m_version;
        char m_name[28];
        u32 m_gp;
        u32 m_exports;  // .lib.ent
        u32 m_exp_end;
        u32 m_imports;  // .lib.stub
        u32 m_imp_end;
    } data;
};

