#pragma once

class CMainWindow : public Singleton<CMainWindow> {
	friend class Singleton<CMainWindow>;
public:
	~CMainWindow() { Finalize(); }

	bool Initialize();
	void Finalize();
	bool Run();

	inline HWND GetHandle() const { return hWindow; }
	inline const TCHAR *GetTitle() const { return windowTitle; }
	inline void SetTitle(const TCHAR *title) { SetWindowText(hWindow, title); }

private:
	CMainWindow() : hWindow(NULL) { }

	static LRESULT CALLBACK WndProc(
		HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	LRESULT Proc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

	bool OnCreate();
	void OnDestroy();
	void OnIdle();

	static const TCHAR windowClass[];
	static const TCHAR windowTitle[];

	static const int windowWidth = 480;
	static const int windowHeight = 272;

	HWND hWindow;
};

extern CMainWindow &MainWindow;
