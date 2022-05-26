/*
 *  26/05/2022 - synced with jpcsp 23/05/2022 - 86c10922
 */
#include "PSPModuleInfo.h"


/*TODO*/  // public class PSPModuleInfo extends pspAbstractMemoryMappedStructure {
/*TODO*/  //	private static final int NAME_LENGTH = 28;
/*TODO*/  //    private int m_attr;
/*TODO*/  //    private int m_version;
/*TODO*/  //    private int m_gp;
/*TODO*/  //    private int m_exports;
/*TODO*/  //    private int m_exp_end;
/*TODO*/  //    private int m_imports;
/*TODO*/  //    private int m_imp_end;
/*TODO*/  //    private String m_namez = ""; // String version of m_name
/*TODO*/  //
/*TODO*/  //    public void read(ByteBuffer f) throws IOException {
/*TODO*/  //        m_attr = readUHalf(f);
/*TODO*/  //        m_version = readUHalf(f);
/*TODO*/  //        byte[] m_name = new byte[NAME_LENGTH];
/*TODO*/  //        f.get(m_name);
/*TODO*/  //        m_gp = readUWord(f);
/*TODO*/  //        m_exports = readUWord(f); // .lib.ent
/*TODO*/  //        m_exp_end = readUWord(f);
/*TODO*/  //        m_imports = readUWord(f); // .lib.stub
/*TODO*/  //        m_imp_end = readUWord(f);
/*TODO*/  //
/*TODO*/  //        // Convert the array of bytes used for the module name to a Java String
/*TODO*/  //        // Calculate the length of the printable portion of the string, otherwise
/*TODO*/  //        // any extra trailing characters may be printed as garbage.
/*TODO*/  //        int len = 0;
/*TODO*/  //        while (len < 28 && m_name[len] != 0)
/*TODO*/  //            len++;
/*TODO*/  //        m_namez = new String(m_name, 0, len);
/*TODO*/  //    }
/*TODO*/  //
/*TODO*/  //	@Override
/*TODO*/  //	protected void read() {
/*TODO*/  //		m_attr = read16();
/*TODO*/  //		m_version = read16();
/*TODO*/  //		m_namez = readStringNZ(NAME_LENGTH);
/*TODO*/  //		m_gp = read32();
/*TODO*/  //		m_exports = read32();
/*TODO*/  //		m_exp_end = read32();
/*TODO*/  //		m_imports = read32();
/*TODO*/  //		m_imp_end = read32();
/*TODO*/  //	}
/*TODO*/  //
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
/*TODO*/  //	public int getM_attr() {
/*TODO*/  //        return m_attr;
/*TODO*/  //    }
/*TODO*/  //
/*TODO*/  //    public int getM_version() {
/*TODO*/  //        return m_version;
/*TODO*/  //    }
/*TODO*/  //
/*TODO*/  //    public int getM_gp() {
/*TODO*/  //        return m_gp;
/*TODO*/  //    }
/*TODO*/  //
/*TODO*/  //    public int getM_exports() {
/*TODO*/  //        return m_exports;
/*TODO*/  //    }
/*TODO*/  //
/*TODO*/  //    public int getM_exp_end() {
/*TODO*/  //        return m_exp_end;
/*TODO*/  //    }
/*TODO*/  //
/*TODO*/  //    public int getM_imports() {
/*TODO*/  //        return m_imports;
/*TODO*/  //    }
/*TODO*/  //
/*TODO*/  //    public int getM_imp_end() {
/*TODO*/  //        return m_imp_end;
/*TODO*/  //    }
/*TODO*/  //
/*TODO*/  //    public String getM_namez() {
/*TODO*/  //        return m_namez;
/*TODO*/  //    }
/*TODO*/  //
/*TODO*/  //	@Override
/*TODO*/  //	public int sizeof() {
/*TODO*/  //		return 52;
/*TODO*/  //	}
/*TODO*/  //}
/*TODO*/  //
