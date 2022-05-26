/*
 *  26/05/2022 - synced with jpcsp 23/05/2022 - 86c10922 - Very initial implementation
 */
#include "..\PCSPCommon.h"
#include "..\Memory.h"
#include "PSP.h"

PSP::PSP() {}



PSP::~PSP() {}



/*TODO*/  //    public static final int PSP_HEADER_SIZE = 336;
/*TODO*/  //    public static final int SCE_KERNEL_MAX_MODULE_SEGMENT = 4;
/*TODO*/  //    public static final int AES_KEY_SIZE = 16;
/*TODO*/  //    public static final int CMAC_KEY_SIZE = 16;
/*TODO*/  //    public static final int CMAC_HEADER_HASH_SIZE = 16;
/*TODO*/  //    public static final int CMAC_DATA_HASH_SIZE = 16;
/*TODO*/  //    public static final int CHECK_SIZE = 88;
/*TODO*/  //    public static final int SHA1_HASH_SIZE = 20;
/*TODO*/  //    public static final int KEY_DATA_SIZE = 16;
/*TODO*/  //    private int magic;
/*TODO*/  //    private int mod_attr;
/*TODO*/  //    private int comp_mod_attr;
/*TODO*/  //    private int mod_ver_lo;
/*TODO*/  //    private int mod_ver_hi;
/*TODO*/  //    private String modname;
/*TODO*/  //    private int mod_version;
/*TODO*/  //    private int nsegments;
/*TODO*/  //    private int elf_size;
/*TODO*/  //    private int psp_size;
/*TODO*/  //    private int boot_entry;
/*TODO*/  //    private int modinfo_offset;
/*TODO*/  //    private int bss_size;
/*TODO*/  //    private int[] seg_align = new int[SCE_KERNEL_MAX_MODULE_SEGMENT];
/*TODO*/  //    private int[] seg_address = new int[SCE_KERNEL_MAX_MODULE_SEGMENT];
/*TODO*/  //    private int[] seg_size = new int[SCE_KERNEL_MAX_MODULE_SEGMENT];
/*TODO*/  //    private int[] reserved = new int[5];
/*TODO*/  //    private int devkit_version;
/*TODO*/  //    private int dec_mode;
/*TODO*/  //    private int pad;
/*TODO*/  //    private int overlap_size;
/*TODO*/  //    private int[] aes_key = new int[AES_KEY_SIZE];
/*TODO*/  //    private int[] cmac_key = new int[CMAC_KEY_SIZE];
/*TODO*/  //    private int[] cmac_header_hash = new int[CMAC_HEADER_HASH_SIZE];
/*TODO*/  //    private int comp_size;
/*TODO*/  //    private int comp_offset;
/*TODO*/  //    private int unk1;
/*TODO*/  //    private int unk2;
/*TODO*/  //    private int[] cmac_data_hash = new int[CMAC_DATA_HASH_SIZE];
/*TODO*/  //    private int tag;
/*TODO*/  //    private int[] sig_check = new int[CHECK_SIZE];
/*TODO*/  //    private int[] sha1_hash = new int[SHA1_HASH_SIZE];
/*TODO*/  //    private int[] key_data = new int[KEY_DATA_SIZE];
/*TODO*/  //
/*TODO*/  //    public PSP(ByteBuffer f) throws IOException {
/*TODO*/  //        read(f);
/*TODO*/  //    }
/*TODO*/  //
PSP::PSP(std::ifstream &f) { f.read((char *)&data, sizeof(data)); } //partial 

/*TODO*/  //    private void read(ByteBuffer f) throws IOException {
/*TODO*/  //        if (f.capacity() == 0) {
/*TODO*/  //            return;
/*TODO*/  //        }
/*TODO*/  //
/*TODO*/  //        magic = readWord(f);
/*TODO*/  //        mod_attr = readUHalf(f);
/*TODO*/  //        comp_mod_attr = readUHalf(f);
/*TODO*/  //        mod_ver_lo = readUByte(f);
/*TODO*/  //        mod_ver_hi = readUByte(f);
/*TODO*/  //        modname = readStringNZ(f, 28);
/*TODO*/  //        mod_version = readUByte(f);
/*TODO*/  //        nsegments = readUByte(f);
/*TODO*/  //        elf_size = readWord(f);
/*TODO*/  //        psp_size = readWord(f);
/*TODO*/  //        boot_entry = readWord(f);
/*TODO*/  //        modinfo_offset = readWord(f);
/*TODO*/  //        bss_size = readWord(f);
/*TODO*/  //        seg_align[0] = readUHalf(f);
/*TODO*/  //        seg_align[1] = readUHalf(f);
/*TODO*/  //        seg_align[2] = readUHalf(f);
/*TODO*/  //        seg_align[3] = readUHalf(f);
/*TODO*/  //        seg_address[0] = readWord(f);
/*TODO*/  //        seg_address[1] = readWord(f);
/*TODO*/  //        seg_address[2] = readWord(f);
/*TODO*/  //        seg_address[3] = readWord(f);
/*TODO*/  //        seg_size[0] = readWord(f);
/*TODO*/  //        seg_size[1] = readWord(f);
/*TODO*/  //        seg_size[2] = readWord(f);
/*TODO*/  //        seg_size[3] = readWord(f);
/*TODO*/  //        reserved[0] = readWord(f);
/*TODO*/  //        reserved[1] = readWord(f);
/*TODO*/  //        reserved[2] = readWord(f);
/*TODO*/  //        reserved[3] = readWord(f);
/*TODO*/  //        reserved[4] = readWord(f);
/*TODO*/  //        devkit_version = readWord(f);
/*TODO*/  //        dec_mode = readUByte(f);
/*TODO*/  //        pad = readUByte(f);
/*TODO*/  //        overlap_size = readUHalf(f);
/*TODO*/  //        for (int i = 0; i < AES_KEY_SIZE; i++) {
/*TODO*/  //            aes_key[i] = readUByte(f);
/*TODO*/  //        }
/*TODO*/  //        for (int i = 0; i < CMAC_KEY_SIZE; i++) {
/*TODO*/  //            cmac_key[i] = readUByte(f);
/*TODO*/  //        }
/*TODO*/  //        for (int i = 0; i < CMAC_HEADER_HASH_SIZE; i++) {
/*TODO*/  //            cmac_header_hash[i] = readUByte(f);
/*TODO*/  //        }
/*TODO*/  //        comp_size = readWord(f);
/*TODO*/  //        comp_offset = readWord(f);
/*TODO*/  //        unk1 = readWord(f);
/*TODO*/  //        unk2 = readWord(f);
/*TODO*/  //        for (int i = 0; i < CMAC_DATA_HASH_SIZE; i++) {
/*TODO*/  //            cmac_data_hash[i] = readUByte(f);
/*TODO*/  //        }
/*TODO*/  //        tag = readWord(f);
/*TODO*/  //        for (int i = 0; i < CHECK_SIZE; i++) {
/*TODO*/  //            sig_check[i] = readUByte(f);
/*TODO*/  //        }
/*TODO*/  //        for (int i = 0; i < SHA1_HASH_SIZE; i++) {
/*TODO*/  //            sha1_hash[i] = readUByte(f);
/*TODO*/  //        }
/*TODO*/  //        for (int i = 0; i < KEY_DATA_SIZE; i++) {
/*TODO*/  //            key_data[i] = readUByte(f);
/*TODO*/  //        }
/*TODO*/  //    }
/*TODO*/  //
/*TODO*/  //    public ByteBuffer decrypt(ByteBuffer f, boolean isSignChecked, byte[] key) {
/*TODO*/  //        if (f.capacity() == 0) {
/*TODO*/  //            return null;
/*TODO*/  //        }
/*TODO*/  //
/*TODO*/  //        CryptoEngine crypto = new CryptoEngine();
/*TODO*/  //        byte[] inBuf;
/*TODO*/  //        if (f.hasArray() && f.position() == 0 && f.arrayOffset() == 0) {
/*TODO*/  //    		inBuf = f.array();
/*TODO*/  //        } else {
/*TODO*/  //        	int currentPosition = f.position();
/*TODO*/  //        	int size = Math.min(Math.max(psp_size, elf_size), f.remaining());
/*TODO*/  //        	inBuf = new byte[size];
/*TODO*/  //        	f.get(inBuf);
/*TODO*/  //        	f.position(currentPosition);
/*TODO*/  //        }
/*TODO*/  //
/*TODO*/  //        int inSize = inBuf.length;
/*TODO*/  //        byte[] elfBuffer = crypto.getPRXEngine().DecryptAndUncompressPRX(inBuf, inSize, isSignChecked, key);
/*TODO*/  //
/*TODO*/  //        if (elfBuffer == null) {
/*TODO*/  //        	return null;
/*TODO*/  //        }
/*TODO*/  //
/*TODO*/  //        if (CryptoEngine.getExtractEbootStatus()) {
/*TODO*/  //            try {
/*TODO*/  //                String ebootPath = Settings.getInstance().getDiscTmpDirectory();
/*TODO*/  //                new File(ebootPath).mkdirs();
/*TODO*/  //                RandomAccessFile raf = new RandomAccessFile(ebootPath + "EBOOT.BIN", "rw");
/*TODO*/  //                raf.write(elfBuffer);
/*TODO*/  //                raf.close();
/*TODO*/  //            } catch (IOException e) {
/*TODO*/  //                // Ignore.
/*TODO*/  //            }
/*TODO*/  //        }
/*TODO*/  //
/*TODO*/  //        return ByteBuffer.wrap(elfBuffer);
/*TODO*/  //    }
/*TODO*/  //
bool PSP::isValid() const { return data.e_magic == PSP_MAGIC; }
/*TODO*/  //    public String getModname() {
/*TODO*/  //    	return modname;
/*TODO*/  //    }
/*TODO*/  //
/*TODO*/  //    public int getDevkitVersion() {
/*TODO*/  //    	return devkit_version;
/*TODO*/  //    }
/*TODO*/  //
/*TODO*/  //    public int getModuleElfVersion() {
/*TODO*/  //    	return (mod_ver_hi << 8) | mod_ver_lo;
/*TODO*/  //    }
