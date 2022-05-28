/*
 *  28/05/2022 - synced with jpcsp 23/05/2022 - 86c10922 - Small TODO requires
 */
#pragma once
class FrameBufferSettings 
{
   public:
    FrameBufferSettings(u32 topAddr, u32 bufferWidth, int width, int height, int pixelFormat);
    ~FrameBufferSettings();

    u32 getTopAddr() const;
    u32 getBottomAddr() const;
    u32 getBufferWidth() const;
    int getPixelFormat() const;
    u8* getPixels() const;
    u8* getPixels(u32 topAddr);
    int getWidth() const;
    int getHeight() const;
    int getSize() const;

    bool isRawAddressInside(u32 address);
    bool isAddressInside(u32 address);
    void setDimension(int width, int height);
    std::string toString() const;
   private:
    void update();
    u32 topAddr;
    u32 bottomAddr;
    u32 bufferWidth;
    int width;
    int height;
    int pixelFormat;
    u8* pixels;
    int size;
};
