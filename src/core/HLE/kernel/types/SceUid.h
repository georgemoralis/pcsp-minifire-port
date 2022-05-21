/*
   21/05/2022 - Synced with 15adb20686fd3d17a8b01292eb5db694b8bff048 19/05/2022
*/
#pragma once
#include <string>

class SceUid {
   private:
    std::string purpose;
    int uid;

   public:
    SceUid(std::string purpose, int uid) {
        this->purpose = purpose;
        this->uid = uid;
    }
    std::string getPurpose() { return purpose; }
    int getUid() { return uid; }
};
