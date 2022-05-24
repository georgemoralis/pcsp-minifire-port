#pragma once

enum PspDisplayPixelFormat {
	PSP_DISPLAY_PIXEL_FORMAT_565 = 0,
	PSP_DISPLAY_PIXEL_FORMAT_5551,
	PSP_DISPLAY_PIXEL_FORMAT_4444,
	PSP_DISPLAY_PIXEL_FORMAT_8888,

	_PspDisplayPixelFormat_force_u32 = 0x7FFFFFFF,
};

enum PspDisplaySetBufSync {
	PSP_DISPLAY_SETBUF_IMMEDIATE = 0,
	PSP_DISPLAY_SETBUF_NEXTFRAME = 1,

	_PspDisplaySetBufSync_force_u32 = 0x7FFFFFFF,
};

enum PspDisplayErrorCodes {
	SCE_DISPLAY_ERROR_OK = 0,
	SCE_DISPLAY_ERROR_POINTER = 0x80000103,
	SCE_DISPLAY_ERROR_ARGUMENT = 0x80000107
};

class CVideo : public Singleton<CVideo> {
	friend class Singleton<CVideo>;
public:
	~CVideo() { Finalize(); }

	bool Initialize();
	void Finalize();

	void Draw();

	PspDisplayErrorCodes sceSetDisplayMode(u32 mode, u32 width, u32 height);
	PspDisplayErrorCodes sceDisplaySetFrameBuf(
		u32 topaddr, u32 bufferwidth,
		PspDisplayPixelFormat pixelformat,
		PspDisplaySetBufSync sync);

private:
	CVideo(){ }

	u8 *vram;

	u32 pspMode;
	u32 pspWidth;
	u32 pspPitch;
	u32 pspHeight;
	u32 pspAddr;
	u32 pixelFormatFb;
	u32 bufferWidthFb;


	TCHAR windowTitle[1024];
	u64 cntFreq;
	u64 cntStart;
	u64 cntEnd;
	u32 cntFrame;

	u32 prevCycles;
};

extern CVideo &Video;
