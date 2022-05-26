/*
 *  26/05/2022 - synced with jpcsp 23/05/2022 - 86c10922 - Very initial implementation
 */
#pragma once

#define PSF_IDENT 0x46535000;
#define PSF_DATA_TYPE_BINARY 0;
#define PSF_DATA_TYPE_STRING 2;
#define PSF_DATA_TYPE_INT32  4;

class PSF 
{
   public:
    PSF();
    PSF(u32 psfOffset);
    ~PSF();
   private:
    u32 psfOffset;
    u32 size;
    bool sizeDirty;
    bool tablesDirty;

    struct {
        u32 ident;
        u32 version;  // yapspd: 0x1100. actual: 0x0101.;
        u32 keyTableOffset;
        u32 valueTableOffset;
        u32 indexEntryCount;
    } data;
};
