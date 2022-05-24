#include "PCSPCommon.h"
#include "Video.h"
#include "Memory.h"
#include "Cpu.h"
#include <emmintrin.h>
#include <tchar.h>

#define GLEW_STATIC
#include <gl/glew.h>
#include <glfw3.h>
#include <sstream>

CVideo &Video = CVideo::GetSingleton();


HDC			hDC = NULL;		// Private GDI Device Context
HGLRC		hRC = NULL;		// Permanent Rendering Context
GLuint texFb;
float texS, texT;
std::string windowtitle = "Emulated Minifire";

static u32 makePow2(u32 n) {
	--n;
	n = (n >> 1) | n;
	n = (n >> 2) | n;
	n = (n >> 4) | n;
	n = (n >> 8) | n;
	n = (n >> 16) | n;
	return ++n;
}

static int getPixelFormatBytes(int pixelformat) {
	return pixelformat == PSP_DISPLAY_PIXEL_FORMAT_8888 ? 4 : 2;
}

static int getPixelFormatGL(int pixelformat) {
	switch (pixelformat) {
	case PSP_DISPLAY_PIXEL_FORMAT_565:
		return GL_UNSIGNED_SHORT_5_6_5_REV;
	case PSP_DISPLAY_PIXEL_FORMAT_5551:
		return GL_UNSIGNED_SHORT_1_5_5_5_REV;
	case PSP_DISPLAY_PIXEL_FORMAT_4444:
		return GL_UNSIGNED_SHORT_4_4_4_4_REV;
	default:
		return GL_UNSIGNED_BYTE;
	}
}

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}
GLFWwindow* window;
bool CVideo::Initialize() {
	

	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	window = glfwCreateWindow(480, 272, windowtitle.c_str(), NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);


	GLenum glewErr;
	if ((glewErr = glewInit()) != GLEW_OK) {
		//MessageBox(NULL, (char*)glewGetErrorString(glewErr), "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	vram = Memory.vram;

	glGenTextures(1, &texFb);

	QueryPerformanceFrequency((LARGE_INTEGER*)&cntFreq);
	QueryPerformanceCounter((LARGE_INTEGER*)&cntStart);
	cntFrame = 0;
	prevCycles = 0;

	return true;

}

void CVideo::Finalize() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void CVideo::Draw() {
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glViewport(0, 0, pspWidth, pspHeight);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_FOG);
	glDisable(GL_LIGHTING);
	glDisable(GL_LOGIC_OP);
	glDisable(GL_STENCIL_TEST);

	glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE, 1.0f);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_ALPHA, GL_TEXTURE);

	glPixelStorei(GL_UNPACK_ALIGNMENT, getPixelFormatBytes(pixelFormatFb));
	glPixelStorei(GL_UNPACK_ROW_LENGTH, pspPitch);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(0.0, pspWidth, pspHeight, 0.0, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
	glFrontFace(GL_CW);
	glBindTexture(GL_TEXTURE_2D, texFb);
	glBegin(GL_QUADS);

	glColor3f(1.0f, 1.0f, 1.0f);

	glTexCoord2f(texS, texT);
	glVertex2i(pspWidth, pspHeight);

	glTexCoord2f(0.0f, texT);
	glVertex2i(0, pspHeight);

	glTexCoord2f(0.0f, 0.0f);
	glVertex2i(0, 0);

	glTexCoord2f(texS, 0.0f);
	glVertex2i(pspWidth, 0);

	glEnd();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopAttrib();

	glfwSwapBuffers(window);
	glfwPollEvents();
	if (glfwWindowShouldClose(window))
	{
		glfwDestroyWindow(window);

		glfwTerminate();
                exit(0);
	}
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


	if (pixelFormatFb != pixelformat ||
		pspPitch != bufferwidth) {
		glBindTexture(GL_TEXTURE_2D, texFb);
		glTexImage2D(GL_TEXTURE_2D, 0,
			GL_RGB,
			bufferwidth, makePow2(pspHeight), 0,
			GL_RGBA,
			getPixelFormatGL(pixelformat), NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		pixelFormatFb = pixelformat;
		pspPitch = bufferwidth;
	}

	glBindTexture(GL_TEXTURE_2D, texFb);
	int pixelFormatGL = getPixelFormatGL(pixelformat);
	glTexSubImage2D(
		GL_TEXTURE_2D, 0,
		0, 0, bufferwidth, pspHeight,
		pixelFormatGL == GL_UNSIGNED_SHORT_5_6_5_REV ? GL_RGB : GL_RGBA,
		pixelFormatGL, vram + pspAddr);

	texS = (float)pspWidth / (float)bufferwidth;
	texT = (float)pspHeight / (float)makePow2(pspHeight);

	Draw();

	++cntFrame;

	QueryPerformanceCounter((LARGE_INTEGER*)&cntEnd);
	if ((cntEnd - cntStart) > cntFreq) {
		double fix = (double)cntFreq / (cntEnd - cntStart);
        char title[256];
        title[255] = '\0';
        snprintf(title, 255, "%s (FPS: %.2lf MIPS: %.5lf)", windowtitle.c_str(),cntFrame * fix, (Cpu.cycles - prevCycles) * fix / 1000000.0f);
        glfwSetWindowTitle(window, title);
		cntFrame = 0;
		cntStart = cntEnd;
		prevCycles = Cpu.cycles;
	}

	return SCE_DISPLAY_ERROR_OK;
}
