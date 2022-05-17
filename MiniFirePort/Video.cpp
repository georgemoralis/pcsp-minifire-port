#include "StdAfx.h"
#include "Video.h"
#include "MainWindow.h"
#include "Memory.h"
#include "Cpu.h"
#include <emmintrin.h>

CVideo &Video = CVideo::GetSingleton();

bool CVideo::Initialize() {
	DDSURFACEDESC2 ddsd;
	HRESULT hr;

	Finalize();

	hr = DirectDrawCreateEx(NULL, (LPVOID *)&ddraw, IID_IDirectDraw7, NULL);
	if (FAILED(hr)) return false;

	hr = ddraw->SetCooperativeLevel(
		MainWindow.GetHandle(),
		DDSCL_NORMAL);
	if (FAILED(hr)) return false;

	ZeroMemory(&ddsd, sizeof(DDSURFACEDESC2));
	ddsd.dwSize = sizeof(DDSURFACEDESC2);
	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_VIDEOMEMORY;
	hr = ddraw->CreateSurface(&ddsd, &primary, NULL);
	if (FAILED(hr)) return false;

	hr = ddraw->CreateClipper(0, &clipper, NULL);
	if (FAILED(hr)) return false;

	hr = clipper->SetHWnd(0, MainWindow.GetHandle());
	if (FAILED(hr)) return false;

	hr = primary->SetClipper(clipper);
	if (FAILED(hr)) return false;

	vram = Memory.vram;

	ZeroMemory(&ddsd, sizeof(DDSURFACEDESC2));
	ddsd.dwSize = sizeof(DDSURFACEDESC2);
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	ddsd.dwWidth = 480;
	ddsd.dwHeight = 272;
	hr = ddraw->CreateSurface(&ddsd, &fb, NULL);
	if (FAILED(hr)) return false;

	QueryPerformanceFrequency((LARGE_INTEGER *)&cntFreq);
	QueryPerformanceCounter((LARGE_INTEGER *)&cntStart);
	cntFrame = 0;
	prevCycles = 0;

	return true;
}

void CVideo::Finalize() {
	if (fb) {
		fb->Release();
		fb = NULL;
	}
	if (clipper) {
		clipper->Release();
		clipper = NULL;
	}
	if (primary) {
		primary->Release();
		primary = NULL;
	}
	if (ddraw) {
		ddraw->Release();
		ddraw = NULL;
	}
}

void CVideo::Draw() {
	if (primary->IsLost())
		primary->Restore();

	RECT rcDst;
	POINT pt = { 0, 0 };

	GetClientRect(MainWindow.GetHandle(), &rcDst);
	ClientToScreen(MainWindow.GetHandle(), &pt);
	OffsetRect(&rcDst, pt.x, pt.y);
	rcDst.right = rcDst.left + 480;
	rcDst.bottom = rcDst.top + 272;

	HRESULT hr = primary->Blt(&rcDst, fb, NULL, DDBLT_ASYNC, NULL);
	assert(SUCCEEDED(hr));
}

void CVideo::ConvertFormat() {
	DDSURFACEDESC2 ddsd;
	ddsd.dwSize = sizeof(DDSURFACEDESC2);
	fb->Lock(NULL, &ddsd, 0, NULL);
	__m128i *dst = (__m128i *)ddsd.lpSurface;

	static const __declspec(align(16)) u32 mask_ag[4] = {
		0xFF00FF00, 0xFF00FF00, 0xFF00FF00, 0xFF00FF00
	};
	static const __declspec(align(16)) u32 mask_r[4] = {
		0x00FF0000, 0x00FF0000, 0x00FF0000, 0x00FF0000
	};
	static const __declspec(align(16)) u32 mask_b[4] = {
		0x000000FF, 0x000000FF, 0x000000FF, 0x000000FF
	};

	__m128i ag = _mm_load_si128((__m128i *)mask_ag);
	__m128i r  = _mm_load_si128((__m128i *)mask_r);
	__m128i b  = _mm_load_si128((__m128i *)mask_b);

	__m128i *p = (__m128i *)(vram + pspAddr);
	for (u32 y = 0; y < 272; ++y) {
		for (u32 x = 0; x < 480 / 4; ++x) {
			__m128i px = _mm_load_si128(p + x);
			__m128i t1 = _mm_and_si128(px, r);
			__m128i t2 = _mm_and_si128(px, b);
			t1 = _mm_srai_epi32(t1, 16);
			t2 = _mm_slli_epi32(t2, 16);
			px = _mm_and_si128(px, ag);
			px = _mm_or_si128(px, t1);
			px = _mm_or_si128(px, t2);
			_mm_storeu_si128(dst + x, px);
		}
		dst += ddsd.lPitch / 16;
		p += 512 / 4;
	}

	fb->Unlock(NULL);
}

PspDisplayErrorCodes CVideo::sceSetDisplayMode(u32 mode, u32 width, u32 height) {
	if (mode != 0 || width == 0 || height == 0 || width > 480 || height > 272)
		return SCE_DISPLAY_ERROR_ARGUMENT;

	pspMode = mode;
	pspWidth = width;
	pspHeight = height;

	return SCE_DISPLAY_ERROR_OK;
}

PspDisplayErrorCodes CVideo::sceDisplaySetFrameBuf(
	u32 topaddr, u32 bufferwidth,
	PspDisplayPixelFormat pixelformat,
	PspDisplaySetBufSync sync)
{
	if (pixelformat != PSP_DISPLAY_PIXEL_FORMAT_8888)
		return SCE_DISPLAY_ERROR_ARGUMENT;
	if (bufferwidth != 512)
		return SCE_DISPLAY_ERROR_ARGUMENT;
	else
		pspPitch = bufferwidth;

	topaddr &= 0x0FFFFFFF;
	if (topaddr < VramStart || (topaddr + pspPitch * pspHeight * 4) > VramEnd ||
		topaddr & (bufferwidth - 1))
	{
		return SCE_DISPLAY_ERROR_POINTER;
	}
	else pspAddr = topaddr - VramStart;


	ConvertFormat();
	Draw();
	
	++cntFrame;
	QueryPerformanceCounter((LARGE_INTEGER *)&cntEnd);
	if ((cntEnd - cntStart) > cntFreq) {
		double fix = (double)cntFreq / (cntEnd - cntStart);
		_stprintf_s(
			windowTitle,
			sizeof(windowTitle) / sizeof(TCHAR),
			_T("%s (FPS: %.2lf MIPS: %.5lf)"),
			MainWindow.GetTitle(),
			cntFrame * fix,
			(Cpu.cycles - prevCycles) * fix /
			1000000.0f);
		MainWindow.SetTitle(windowTitle);
		cntFrame = 0;
		cntStart = cntEnd;
		prevCycles = Cpu.cycles;
	}

	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) break;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return SCE_DISPLAY_ERROR_OK;
}
