// Win32 GDI Helper
// Copyright (c) 2023 Isaiah

#include <windows.h>
#include <stdio.h>
#include <wingdi.h>

HFONT my_create_font() {
	return CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, 
                OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
}

SIZE my_text_size(HDC hdc, char* text, int textLength) {
	SIZE size;

	// select a font into the device context
	// HFONT hFont = my_create_font();
	// SelectObject(hdc, hFont);

	// get the size of the text
	GetTextExtentPoint32(hdc, text, textLength, &size);

	// the width and height of the text are in the size structure
	int width = size.cx;
	int height = size.cy;
	return size;
}

// set the background mode to transparent
void fix_text_background(HDC hdc) {
	SetBkMode(hdc, TRANSPARENT);
}

// set the text color
void set_text_color(HDC hdc, COLORREF color) {
	SetTextColor(hdc, color);
}

void my_scissor_rect(HDC hdc, int x, int y, int w, int h) {
	HRGN hRgn;
	RECT rect;

	// create a region that defines the scissor rectangle
	rect.left = x;
	rect.top = y;
	rect.right = x + w;
	rect.bottom = y + h;
	hRgn = CreateRectRgnIndirect(&rect);

	// set the scissor rectangle for the device context
	SelectClipRgn(hdc, hRgn);

	// draw some graphics that will be clipped by the scissor rectangle
	// ...

	// release the region and the device context
	// DeleteObject(hRgn);
}

int get_mouse_x(LPARAM lParam) {
	int x = LOWORD(lParam);
}

int get_mouse_y(LPARAM lParam) {
	int y = HIWORD(lParam);
}

void draw_background(HWND hWnd, WPARAM wParam) {
	 /*RECT rc;
     GetClientRect(hWnd, &rc);
     HDC hdc = (HDC)wParam;
     HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));//(HBRUSH)GetClassLongPtr(hWnd, GCLP_HBRBACKGROUND);
     FillRect(hdc, &rc, hBrush);*/
}

void set_background(HWND hWnd) {
	 //HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
     //SetClassLongPtr(hWnd, GCLP_HBRBACKGROUND, (LONG_PTR)hBrush);
}

static HBITMAP hBuffer;
// static HDC hdcc;
static HDC hBufferDC;

HDC get_bufferdc() {
	return hBufferDC;
}

HDC hdcc() {
	return hdcc;
}

void set_hdcc(HDC h) {
	//hdcc = h;
}

void bit_blt(HDC hdcc, PAINTSTRUCT ps, HDC hBufferDC) {
	BitBlt(hdcc, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right - ps.rcPaint.left,
                ps.rcPaint.bottom - ps.rcPaint.top, hBufferDC, ps.rcPaint.left, ps.rcPaint.top, SRCCOPY);
	
}

HDC do_paint(HWND hwnd, HDC hbufferdc) {
	PAINTSTRUCT ps;
	HDC hdcc = BeginPaint(hwnd, &ps);
	bit_blt(hdcc, ps, hbufferdc);
	EndPaint(hwnd, &ps);
	return hdcc;
}

int RegisterClassEx_(WNDCLASS* claz) {
	return RegisterClassEx(claz);
}

static HBITMAP hBuffer;
static HDC hDC;
static HDC hBufferDC;
static COLORREF background = RGB(255, 255, 255);
static is_win32_ui = false;
static HWND hwnd;

bool is_native_win32_ui() {
	return is_win32_ui;
}

int win32_width() {
	 RECT clientRect;
     GetClientRect(hwnd, &clientRect);
     int width = clientRect.right - clientRect.left;
     int height = clientRect.bottom - clientRect.top;
	 return width;
}

int win32_height() {
	 RECT clientRect;
     GetClientRect(hwnd, &clientRect);
     int width = clientRect.right - clientRect.left;
     int height = clientRect.bottom - clientRect.top;
	 return height;
}


LRESULT CALLBACK WndProc_A(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	HDC hdc = GetDC(hwnd);
	HDC hdcMem = CreateCompatibleDC(hdc);

	RECT clientRect;
	GetClientRect(hwnd, &clientRect);
	int width = clientRect.right - clientRect.left;
	int height = clientRect.bottom - clientRect.top;

	HBITMAP hbmMem = CreateCompatibleBitmap(hdc, width, height);
	SelectObject(hBufferDC, hbmMem);

	// Draw on the memory DC

	ReleaseDC(hwnd, hdc);
	DeleteDC(hdcMem);
	DeleteObject(hbmMem);

	return 0;
}

void WndProc_create(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	is_win32_ui = true;
	hwnd = hWnd;
	hDC = GetDC(hWnd);
            hBufferDC = CreateCompatibleDC(hDC);
            hBuffer = CreateCompatibleBitmap(hDC, 640, 480);
            SelectObject(hBufferDC, hBuffer);
            ReleaseDC(hWnd, hDC);
}

void WndProc_pa(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	        RECT rc;
            GetClientRect(hWnd, &rc);
            HBRUSH hBrush = CreateSolidBrush(background);
            FillRect(hBufferDC, &rc, hBrush);
            DeleteObject(hBrush);
}

void WndProc_pb(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	 // Copy the memory buffer to the screen
            PAINTSTRUCT ps;
            hDC = BeginPaint(hWnd, &ps);
            BitBlt(hDC, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right - ps.rcPaint.left,
                ps.rcPaint.bottom - ps.rcPaint.top, hBufferDC, ps.rcPaint.left, ps.rcPaint.top, SRCCOPY);
            EndPaint(hWnd, &ps);
            return 0;
}

void win32_set_bg(int r, int g, int b) {
	background = RGB(r, g, b);
}

void win32_draw_line(HDC hdc, int a, int b, int c, int d) {
	MoveToEx(hdc, a, b, NULL);
	LineTo(hdc, c, d);
}


HDC im(HDC hdc, int x, int y, int width, int height, unsigned char* image_data) {
	
	
	//ConvertBGRToRGB(image_data, width, height, 4);


//return image_data
return hdc;

}

void ConvertBGRToRGB(unsigned char* data, int width, int height, int bytesPerPixel) {
	for (int i = 0; i < width * height * bytesPerPixel; i += bytesPerPixel) {
        unsigned char temp = data[i];
        data[i] = data[i + 2];
        data[i + 2] = temp;
    }
}

HBITMAP CreateBitmapFromPixels(HDC hdc,int width,int height,void *pixels) {
	ConvertBGRToRGB(pixels, width, height, 4);

    BITMAPINFO bmi = {0};
    bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height; // top-down
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32; // 24 = RGB, 32 = RGBA
    bmi.bmiHeader.biCompression = BI_RGB;

    HBITMAP hbm = CreateDIBitmap(hdc,&bmi.bmiHeader,CBM_INIT,pixels,&bmi,DIB_RGB_COLORS);

    return hbm;
}

// https://learn.microsoft.com/en-us/windows/win32/controls/draw-an-image
void PaintImage(HDC hdc, HBITMAP hbm, int x, int y, int w, int h) {
    HDC hdcMem = CreateCompatibleDC(hdc);
    HGDIOBJ hbmOld = SelectObject(hdcMem,hbm);

    BITMAP bm;
    GetObject(hbm,sizeof(bm),&bm);

    BLENDFUNCTION bf;
    bf.BlendOp = AC_SRC_OVER;
    bf.BlendFlags = 0;
    bf.SourceConstantAlpha = 255; // use per-pixel alpha values
    bf.AlphaFormat = AC_SRC_ALPHA; // bitmap has alpha channel

    AlphaBlend(hdc, x, y, w, h, hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, bf);

    SelectObject(hdcMem,hbmOld);
    DeleteDC(hdcMem);
}

imd(HDC hdc, int x, int y, int width, int height, unsigned char* image_data) {
	HBITMAP map = CreateBitmapFromPixels(hdc, width, height, image_data);
	PaintImage(hdc, map, x, y, width, height);
}
