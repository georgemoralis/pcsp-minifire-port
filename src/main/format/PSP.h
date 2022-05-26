/*
 *  26/05/2022 - synced with jpcsp 23/05/2022 - 86c10922 - Very initial implementation
 */
#pragma once

#define PSP_MAGIC 0x5053507E

class PSP {

public:
    PSP();
    PSP(std::ifstream &f);
    ~PSP();

    bool isValid() const;

   private:
    struct {
        u32 e_magic;
    } data;

};
