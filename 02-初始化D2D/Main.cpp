#include <d2d1.h>

#pragma comment(lib,"d2d1.lib")

ID2D1Factory* MainID2D1Factory = nullptr;
ID2D1HwndRenderTarget* MainID2D1HwndRenderTarget = nullptr;
HWND MyWindow = nullptr;

LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

void InitD2D()
{
	D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &MainID2D1Factory);
	RECT wnd_rect = {};
	GetWindowRect(MyWindow, &wnd_rect);
	const D2D1_SIZE_U wnd_size = D2D1::SizeU(wnd_rect.right, wnd_rect.bottom);
	const D2D1_RENDER_TARGET_PROPERTIES render_target_property = D2D1::RenderTargetProperties();
	const D2D1_HWND_RENDER_TARGET_PROPERTIES hwnd_render_target_property = D2D1::HwndRenderTargetProperties(MyWindow, wnd_size);
	MainID2D1Factory->CreateHwndRenderTarget(render_target_property, hwnd_render_target_property, &MainID2D1HwndRenderTarget);
}

void Draw()
{
	MainID2D1HwndRenderTarget->BeginDraw();
	const D2D1_COLOR_F clear_color = D2D1::ColorF(0.0f, 0.6f, 1.0f, 1.0f);
	MainID2D1HwndRenderTarget->Clear(clear_color);
	MainID2D1HwndRenderTarget->EndDraw();
}

int main()
{
	WNDCLASS wnd_class = {};
	wnd_class.lpfnWndProc = MainWindowProc;
	wnd_class.lpszClassName = L"MyWindowClass";
	wnd_class.hInstance = GetModuleHandle(nullptr);
	wnd_class.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wnd_class.hCursor = LoadCursor(nullptr, IDC_ARROW);
	RegisterClass(&wnd_class);

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
	ShowWindow(MyWindow, true);

	InitD2D();

	MSG msg = {};
	while (GetMessageW(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
		Draw();
	}
	return 0;
}