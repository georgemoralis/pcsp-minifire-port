/*
 *  26/05/2022 - synced with jpcsp 23/05/2022 - 86c10922 Completed!
 */
#pragma once
class Elf32Relocate 
{
   public:
    Elf32Relocate();
    ~Elf32Relocate();
    void read(std::ifstream &f);

    static const size_t sizeOf();
    std::string toString() const;

    u32 getR_offset() const;
    void setR_offset(u32 offset);

    u32 getR_info() const;
    void setR_info(u32 info);

   private:
    struct {
        u32 r_offset;
        u32 r_info;
    } data;
};
