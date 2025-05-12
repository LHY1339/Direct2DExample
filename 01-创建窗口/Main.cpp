#include <d2d1.h>

#pragma comment(lib,"d2d1.lib")

//窗口过程函数
LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

int main()
{
	//创建WNDCLASS
	WNDCLASS wnd_class = {};
	wnd_class.lpfnWndProc = MainWindowProc;
	wnd_class.lpszClassName = L"MyWindowClass";
	wnd_class.hInstance = GetModuleHandle(nullptr);
	wnd_class.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wnd_class.hCursor = LoadCursor(nullptr, IDC_ARROW);

	//注册WNDCLASS
	RegisterClass(&wnd_class);

	//创建窗口
	HWND MyWindow;
	MyWindow = CreateWindowEx(
		0,                
		L"MyWindowClass",   
		L"MyWindowTitle",   
		WS_ACTIVECAPTION,       
		CW_USEDEFAULT,  
		CW_USEDEFAULT,
		600,
		400,
		nullptr,
		nullptr,
		GetModuleHandle(nullptr),
		nullptr
	);

	//显示窗口
	ShowWindow(MyWindow, true);

	//消息循环
	MSG msg = {};
	while (GetMessageW(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	return 0;
}