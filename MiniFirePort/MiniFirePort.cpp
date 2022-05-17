#include "StdAfx.h"
#include "MainWindow.h"

int APIENTRY _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int) {
	if (MainWindow.Initialize()) {
		MainWindow.Run();
	}
	return 0;
}
