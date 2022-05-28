/*
 *  28/05/2022 - synced with jpcsp 23/05/2022 - 86c10922 - Small TODO requires
 */
#include "..\PCSPCommon.h"
#include "..\Memory.h"
#include "FrameBufferSettings.h"
/**
 * Simple class containing the attributes related to a display frame buffer.
 */
FrameBufferSettings::FrameBufferSettings(u32 topAddr, u32 bufferWidth, int width, int height,
                                         int pixelFormat) {
    this->topAddr = topAddr & Memory::addressMask;
    this->bufferWidth = bufferWidth;
    this->width = width;
    this->height = height;
    this->pixelFormat = pixelFormat;
    update();
}
FrameBufferSettings::~FrameBufferSettings(){

}


/*TODO*/// public FrameBufferSettings(FrameBufferSettings copy) {
/*TODO*/  //        topAddr = copy.topAddr;
/*TODO*/  //        bottomAddr = copy.bottomAddr;
/*TODO*/  //        bufferWidth = copy.bufferWidth;
/*TODO*/  //        width = copy.width;
/*TODO*/  //        height = copy.height;
/*TODO*/  //        pixelFormat = copy.pixelFormat;
/*TODO*/  //        pixels = copy.pixels;
/*TODO*/  //        size = copy.size;
/*TODO*/  //    }

void FrameBufferSettings::update() {
/*TODO*/  //     size = bufferWidth * height * getPixelFormatBytes(pixelFormat);
    bottomAddr = topAddr + size;
    memcpy(&pixels, Memory::getPointer(topAddr), size);//pixels = Memory.getInstance().getBuffer(topAddr, size);
}

u32 FrameBufferSettings::getTopAddr() const { return topAddr; }
u32 FrameBufferSettings::getBottomAddr() const { return bottomAddr; }
u32 FrameBufferSettings::getBufferWidth() const { return bufferWidth; }
int FrameBufferSettings::getPixelFormat() const { return pixelFormat; }
u8* FrameBufferSettings::getPixels() const { return pixels; }
u8* FrameBufferSettings::getPixels(u32 topAddr)  {
    if (this->topAddr == topAddr) {
        return pixels;
    }
    memcpy(&pixels, Memory::getPointer(topAddr), size);//Memory.getInstance().getBuffer(topAddr, size);
    return pixels;
}

int FrameBufferSettings::getWidth() const { return width; }
int FrameBufferSettings::getHeight() const { return height; }
int FrameBufferSettings::getSize() const { return size; }


bool FrameBufferSettings::isRawAddressInside(u32 address) {
// vram address is lower than main memory so check the end of the buffer first, it's more likely to fail
   return address >= topAddr && address < bottomAddr;
}

bool FrameBufferSettings::isAddressInside(u32 address) {
   return isRawAddressInside(address & Memory::addressMask);
}

void FrameBufferSettings::setDimension(int width, int height){
   this->width = width;
   this->height = height;
   update();
}

std::string FrameBufferSettings::toString() const {
   std::string str;
   char tmp[128];

   sprintf(tmp, "0x%08X-0x%08X, %dx%d, bufferWidth=%d, pixelFormat=%d", topAddr, bottomAddr, width, height,
            bufferWidth, pixelFormat);
   str.append(tmp);
   return str;
}
