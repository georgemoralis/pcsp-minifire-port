/*
*  25/05/2022 - synced with jpcsp 23/05/2022 - 86c10922
*/
#include "Elf32Header.h"

/*TODO*/  //    private int e_magic;
/*TODO*/  //    private int e_class;
/*TODO*/  //    private int e_data;
/*TODO*/  //    private int e_idver;
/*TODO*/  //    private byte[] e_pad = new byte[9];
/*TODO*/  //    private int e_type;
/*TODO*/  //    private int e_machine;
/*TODO*/  //    private int e_version;
/*TODO*/  //    private int e_entry;
/*TODO*/  //    private int e_phoff;
/*TODO*/  //    private int e_shoff;
/*TODO*/  //    private int e_flags;
/*TODO*/  //    private int e_ehsize;
/*TODO*/  //    private int e_phentsize;
/*TODO*/  //    private int e_phnum;
/*TODO*/  //    private int e_shentsize;
/*TODO*/  //    private int e_shnum;
/*TODO*/  //    private int e_shstrndx;
/*TODO*/  //
/*TODO*/  //    private void read(ByteBuffer f) throws IOException {
/*TODO*/  //        if (f.capacity() == 0) {
/*TODO*/  //            return;
/*TODO*/  //        }
/*TODO*/  //        e_magic = readUWord(f);
/*TODO*/  //        e_class = readUByte(f);
/*TODO*/  //        e_data = readUByte(f);
/*TODO*/  //        e_idver = readUByte(f);
/*TODO*/  //        f.get(getE_pad());         // can raise EOF exception
/*TODO*/  //        e_type = readUHalf(f);
/*TODO*/  //        e_machine = readUHalf(f);
/*TODO*/  //        e_version = readUWord(f);
/*TODO*/  //        e_entry = readUWord(f);
/*TODO*/  //        e_phoff = readUWord(f);
/*TODO*/  //        e_shoff = readUWord(f);
/*TODO*/  //        e_flags = readUWord(f);
/*TODO*/  //        e_ehsize = readUHalf(f);
/*TODO*/  //        e_phentsize = readUHalf(f);
/*TODO*/  //        e_phnum = readUHalf(f);
/*TODO*/  //        e_shentsize = readUHalf(f);
/*TODO*/  //        e_shnum = readUHalf(f);
/*TODO*/  //        e_shstrndx = readUHalf(f);
/*TODO*/  //    }
/*TODO*/  //
/*TODO*/  //    public Elf32Header(ByteBuffer f) throws IOException {
/*TODO*/  //        read(f);
/*TODO*/  //    }
/*TODO*/  //
/*TODO*/  //    public static int sizeof() {
/*TODO*/  //        return 52;
/*TODO*/  //    }
/*TODO*/  //
/*TODO*/  //    public boolean isValid(){
/*TODO*/  //        return getE_magic() == ELF_MAGIC;
/*TODO*/  //    }
/*TODO*/  //
/*TODO*/  //    public boolean isMIPSExecutable(){
/*TODO*/  //        return getE_machine() == E_MACHINE_MIPS;
/*TODO*/  //    }
/*TODO*/  //
/*TODO*/  //    public boolean isPRXDetected(){
/*TODO*/  //        return getE_type() == ET_SCE_PRX;
/*TODO*/  //    }
/*TODO*/  //
/*TODO*/  //    public boolean requiresRelocation(){
/*TODO*/  //        return isPRXDetected() || getE_entry() < MemoryMap.START_RAM;
/*TODO*/  //    }
/*TODO*/  //
/*TODO*/  //    @Override
/*TODO*/  //    public String toString() {
/*TODO*/  //        StringBuilder str = new StringBuilder();
/*TODO*/  //        str.append("-----ELF HEADER---------" + "\n");
/*TODO*/  //        str.append("e_magic " + "\t " + Utilities.formatString("long", Long.toHexString(getE_magic() &
          //        0xFFFFFFFFL).toUpperCase()) + "\n");
/*TODO*/  //        str.append("e_class " + "\t " + Utilities.integerToHex(getE_class() & 0xFF) + "\n");
/*TODO*/  //        // str.append("e_class " + "\t " +  Utilities.formatString("byte", Integer.toHexString(e_class &
          //        0xFF ).toUpperCase())+ "\n");
/*TODO*/  //        str.append("e_data " + "\t\t " + Utilities.formatString("byte", Integer.toHexString(getE_data() &
          //        0xFF).toUpperCase()) + "\n");
/*TODO*/  //        str.append("e_idver " + "\t " + Utilities.formatString("byte", Integer.toHexString(getE_idver() &
          //        0xFF).toUpperCase()) + "\n");
/*TODO*/  //        str.append("e_type " + "\t\t " + Utilities.formatString("short", Integer.toHexString(getE_type() &
          //        0xFFFF).toUpperCase()) + "\n");
/*TODO*/  //        str.append("e_machine " + "\t " + Utilities.formatString("short", Integer.toHexString(getE_machine()
          //        & 0xFFFF).toUpperCase()) + "\n");
/*TODO*/  //        str.append("e_version " + "\t " + Utilities.formatString("long", Long.toHexString(getE_version() &
          //        0xFFFFFFFFL).toUpperCase()) + "\n");
/*TODO*/  //        str.append("e_entry " + "\t " + Utilities.formatString("long", Long.toHexString(getE_entry() &
          //        0xFFFFFFFFL).toUpperCase()) + "\n");
/*TODO*/  //        str.append("e_phoff " + "\t " + Utilities.formatString("long", Long.toHexString(getE_phoff() &
          //        0xFFFFFFFFL).toUpperCase()) + "\n");
/*TODO*/  //        str.append("e_shoff " + "\t " + Utilities.formatString("long", Long.toHexString(getE_shoff() &
          //        0xFFFFFFFFL).toUpperCase()) + "\n");
/*TODO*/  //        str.append("e_flags " + "\t " + Utilities.formatString("long", Long.toHexString(getE_flags() &
          //        0xFFFFFFFFL).toUpperCase()) + "\n");
/*TODO*/  //        str.append("e_ehsize " + "\t " + Utilities.formatString("short", Integer.toHexString(getE_ehsize() &
          //        0xFFFF).toUpperCase()) + "\n");
/*TODO*/  //        str.append("e_phentsize " + "\t " + Utilities.formatString("short",
          //        Integer.toHexString(getE_phentsize() & 0xFFFF).toUpperCase()) + "\n");
/*TODO*/  //        str.append("e_phnum " + "\t " + Utilities.formatString("short", Integer.toHexString(getE_phnum() &
          //        0xFFFF).toUpperCase()) + "\n");
/*TODO*/  //        str.append("e_shentsize " + "\t " + Utilities.formatString("short",
          //        Integer.toHexString(getE_shentsize() & 0xFFFF).toUpperCase()) + "\n");
/*TODO*/  //        str.append("e_shnum " + "\t " + Utilities.formatString("short", Integer.toHexString(getE_shnum() &
          //        0xFFFF).toUpperCase()) + "\n");
/*TODO*/  //        str.append("e_shstrndx " + "\t " + Utilities.formatString("short",
          //        Integer.toHexString(getE_shstrndx() & 0xFFFF).toUpperCase()) + "\n");
/*TODO*/  //        return str.toString();
/*TODO*/  //    }
/*TODO*/  //
/*TODO*/  //    public int getE_magic() {
/*TODO*/  //        return e_magic;
/*TODO*/  //    }
/*TODO*/  //
/*TODO*/  //    public int getE_class() {
/*TODO*/  //        return e_class;
/*TODO*/  //    }
/*TODO*/  //
/*TODO*/  //    public int getE_data() {
/*TODO*/  //        return e_data;
/*TODO*/  //    }
/*TODO*/  //
/*TODO*/  //    public int getE_idver() {
/*TODO*/  //        return e_idver;
/*TODO*/  //    }
/*TODO*/  //
/*TODO*/  //    public byte[] getE_pad() {
/*TODO*/  //        return e_pad;
/*TODO*/  //    }
/*TODO*/  //
/*TODO*/  //    public int getE_type() {
/*TODO*/  //        return e_type;
/*TODO*/  //    }
/*TODO*/  //
/*TODO*/  //    public int getE_machine() {
/*TODO*/  //        return e_machine;
/*TODO*/  //    }
/*TODO*/  //
/*TODO*/  //    public int getE_version() {
/*TODO*/  //        return e_version;
/*TODO*/  //    }
/*TODO*/  //
/*TODO*/  //    public int getE_entry() {
/*TODO*/  //        return e_entry;
/*TODO*/  //    }
/*TODO*/  //
/*TODO*/  //    public int getE_phoff() {
/*TODO*/  //        return e_phoff;
/*TODO*/  //    }
/*TODO*/  //
/*TODO*/  //    public int getE_shoff() {
/*TODO*/  //        return e_shoff;
/*TODO*/  //    }
/*TODO*/  //
/*TODO*/  //    public int getE_flags() {
/*TODO*/  //        return e_flags;
/*TODO*/  //    }
/*TODO*/  //
/*TODO*/  //    public int getE_ehsize() {
/*TODO*/  //        return e_ehsize;
/*TODO*/  //    }
/*TODO*/  //
/*TODO*/  //    public int getE_phentsize() {
/*TODO*/  //        return e_phentsize;
/*TODO*/  //    }
/*TODO*/  //
/*TODO*/  //    public int getE_phnum() {
/*TODO*/  //        return e_phnum;
/*TODO*/  //    }
/*TODO*/  //
/*TODO*/  //    public int getE_shentsize() {
/*TODO*/  //        return e_shentsize;
/*TODO*/  //    }
/*TODO*/  //
/*TODO*/  //    public int getE_shnum() {
/*TODO*/  //        return e_shnum;
/*TODO*/  //    }
/*TODO*/  //
/*TODO*/  //    public int getE_shstrndx() {
/*TODO*/  //        return e_shstrndx;
/*TODO*/  //    }
