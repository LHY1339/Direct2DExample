#include <d2d1.h>
#include <dwrite.h>
#include <wincodec.h>
#include <string>
#include <iostream>

#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")
#pragma comment(lib,"windowscodecs.lib")

HWND MyWindow = nullptr;
ID2D1Factory* MainID2D1Factory = nullptr;
ID2D1HwndRenderTarget* MainID2D1HwndRenderTarget = nullptr;
ID2D1SolidColorBrush* MainID2D1SolidColorBrush = nullptr;
ID2D1LinearGradientBrush* MainID2D1LinearGradientBrush = nullptr;
IDWriteFactory* MainIDWriteFactory = nullptr;
IDWriteTextFormat* MainIDWriteTextFormat = nullptr;
IWICImagingFactory* MainIWICImagingFactory = nullptr;
ID2D1BitmapBrush* MainID2D1BitmapBrush = nullptr;
ID2D1Bitmap* MainID2D1Bitmap = nullptr;

ID2D1Bitmap* LoadBitmapFromFile(const wchar_t* Url)
{
	IWICBitmapDecoder* pDecoder = nullptr;
	IWICBitmapFrameDecode* pSource = nullptr;
	IWICFormatConverter* pConverter = nullptr;
	ID2D1Bitmap* pBitmap = nullptr;

	MainIWICImagingFactory->CreateDecoderFromFilename(
		Url,
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder
	);
	pDecoder->GetFrame(0, &pSource);
	MainIWICImagingFactory->CreateFormatConverter(&pConverter);
	pConverter->Initialize(
		pSource,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		nullptr,
		0.0f,
		WICBitmapPaletteTypeCustom
	);

	MainID2D1HwndRenderTarget->CreateBitmapFromWicBitmap(
		pConverter,
		nullptr,
		&pBitmap
	);

	pDecoder->Release();
	pSource->Release();
	pConverter->Release();

	return pBitmap;
}

LRESULT CALLBACK MainWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

void InitD2D()
{
	D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &MainID2D1Factory);

	RECT wnd_rect = {};
	GetClientRect(MyWindow, &wnd_rect);
	const D2D1_SIZE_U wnd_size = D2D1::SizeU(wnd_rect.right, wnd_rect.bottom);
	const D2D1_RENDER_TARGET_PROPERTIES render_target_property = D2D1::RenderTargetProperties();
	const D2D1_HWND_RENDER_TARGET_PROPERTIES hwnd_render_target_property = D2D1::HwndRenderTargetProperties(MyWindow, wnd_size);
	MainID2D1Factory->CreateHwndRenderTarget(render_target_property, hwnd_render_target_property, &MainID2D1HwndRenderTarget);

	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&MainIDWriteFactory));

	HRESULT res = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&MainIWICImagingFactory));
}

void DrawSolidRect()
{
	if (!MainID2D1SolidColorBrush)
	{
		D2D1_COLOR_F def_color = D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f);
		MainID2D1HwndRenderTarget->CreateSolidColorBrush(def_color, &MainID2D1SolidColorBrush);
		return;
	}
	const D2D1_COLOR_F rect_color = D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.5f);
	MainID2D1SolidColorBrush->SetColor(rect_color);
	D2D1_RECT_F rect_size = D2D1::RectF(20.0f, 20.0f, 120.0f, 70.0f);
	MainID2D1HwndRenderTarget->FillRectangle(rect_size, MainID2D1SolidColorBrush);

	const D2D1_COLOR_F round_rect_color = D2D1::ColorF(1.0f, 0.0f, 0.0f, 0.5f);
	MainID2D1SolidColorBrush->SetColor(round_rect_color);
	D2D1_ROUNDED_RECT round_rect_size = D2D1::RoundedRect(D2D1::RectF(20.0f, 100.0f, 120.0f, 150.0f), 10.0f, 10.0f);
	MainID2D1HwndRenderTarget->FillRoundedRectangle(round_rect_size, MainID2D1SolidColorBrush);
}

void DrawLinearRect()
{
	if (!MainID2D1LinearGradientBrush)
	{
		ID2D1GradientStopCollection* stop_collection = nullptr;
		const D2D1_GRADIENT_STOP stops[3] = {
			D2D1::GradientStop(0.0f,D2D1::ColorF(1.0f,0.0f,0.0f,0.5f)),
			D2D1::GradientStop(0.5f,D2D1::ColorF(0.0f,1.0f,0.0f,0.5f)),
			D2D1::GradientStop(1.0f,D2D1::ColorF(0.0f,0.0f,1.0f,0.5f)),
		};

		MainID2D1HwndRenderTarget->CreateGradientStopCollection(
			stops,
			ARRAYSIZE(stops),
			&stop_collection
		);
		if (!stop_collection)
		{
			return;
		}
		const D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES linear_gradient_property = D2D1::LinearGradientBrushProperties(D2D1::Point2F(200.0f, 20.0f), D2D1::Point2F(300.0f, 70.0f));
		MainID2D1HwndRenderTarget->CreateLinearGradientBrush(linear_gradient_property, stop_collection, &MainID2D1LinearGradientBrush);
		return;
	}
	D2D1_ROUNDED_RECT round_rect_size = D2D1::RoundedRect(D2D1::RectF(200.0f, 20.0f, 300.0f, 70.0f), 10.0f, 10.0f);
	MainID2D1HwndRenderTarget->FillRoundedRectangle(round_rect_size, MainID2D1LinearGradientBrush);
}

void DrawD2DText()
{
	if (!MainIDWriteTextFormat)
	{
		MainIDWriteFactory->CreateTextFormat(
			L"Arial",
			nullptr,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			30.0f,
			L"",
			&MainIDWriteTextFormat
		);
		return;
	}
	const wchar_t in_text[] = L"MyText";
	D2D1_RECT_F text_rect = D2D1::RectF(
		400.0f, 20.0f, 500.0f, 70.0f
	);
	MainID2D1SolidColorBrush->SetColor(D2D1::ColorF(1.0f, 1.0f, 1.0f, 0.5f));
	MainID2D1HwndRenderTarget->DrawTextW(
		in_text,
		wcslen(in_text),
		MainIDWriteTextFormat,
		text_rect,
		MainID2D1SolidColorBrush
	);
}

void DrawD2DImage()
{
	static float i = 0.0f;
	if (!MainID2D1BitmapBrush)
	{
		MainID2D1Bitmap = LoadBitmapFromFile(L"image.jpeg");
		MainID2D1HwndRenderTarget->CreateBitmapBrush(
			MainID2D1Bitmap,
			&MainID2D1BitmapBrush
		);
		return;
	}
	D2D1_ROUNDED_RECT round_rect_size = D2D1::RoundedRect(D2D1::RectF(i + 20.0f, 150.0f, i + 220.0f, 350.0f), 10.0f, 10.0f);

	const D2D1_SIZE_F bitmapSize = MainID2D1Bitmap->GetSize();
	const float _width = (round_rect_size.rect.right - round_rect_size.rect.left) / bitmapSize.width;
	const float _height = (round_rect_size.rect.bottom - round_rect_size.rect.top) / bitmapSize.height;
	D2D1_MATRIX_3X2_F trans = D2D1::Matrix3x2F::Translation(round_rect_size.rect.left, round_rect_size.rect.top);
	D2D1_MATRIX_3X2_F scale = D2D1::Matrix3x2F::Scale(_width, _height, D2D1::Point2F(round_rect_size.rect.left, round_rect_size.rect.top));
	MainID2D1BitmapBrush->SetTransform(trans * scale);

	MainID2D1HwndRenderTarget->FillRoundedRectangle(round_rect_size, MainID2D1BitmapBrush);
	i += 0.2f;
}

void Draw()
{
	MainID2D1HwndRenderTarget->BeginDraw();
	const D2D1_COLOR_F clear_color = D2D1::ColorF(0.0f, 0.6f, 1.0f, 1.0f);
	MainID2D1HwndRenderTarget->Clear(clear_color);
	DrawSolidRect();
	DrawLinearRect();
	DrawD2DText();
	DrawD2DImage();
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
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
		Draw();
	}
	return 0;
}