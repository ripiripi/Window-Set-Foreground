#pragma once
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <atlimage.h>
#include <chrono>
#include <vector>
#include <queue>
#include <unordered_set>

std::unordered_set<HWND> WindowSet;

static TCHAR szWindowClass[] = _T("DesktopApp");

static TCHAR szTitle[] = _T("WindowSetForeground");

HINSTANCE hInst;
static TCHAR colorstr[50];

POINT mouse_p;
static TCHAR mousestr[50];

void set_foreground();
void set_transparency(int);
HWND hWnd;
HWND puyoWnd;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int CALLBACK WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
http://wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("Windows Desktop Guided Tour"),
            NULL);

        return 1;
    }

    hInst = hInstance;

    hWnd = CreateWindow(
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        300, 100,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hWnd)
    {
        MessageBox(NULL,
            _T("Call to CreateWindow failed!"),
            _T("Puyo Puyo Chain Maker"),
            NULL);

        return 1;
    }

    SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    BOOL zero_keyflag = false;
    BOOL one_keyflag = false;
    BOOL two_keyflag = false;
    BOOL three_keyflag = false;
    BOOL trans_keyflag[9];
    while (true)
    {
        Sleep(50);
        if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
            if (GetMessage(&msg, NULL, 0, 0)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
            else {
                break;
            }
        }
        else {

            if (!(GetAsyncKeyState(VK_CONTROL) & 0x8000)) {
                zero_keyflag = false;
                for (int i = 0; i < 9; i++) {
                    trans_keyflag[i] = false;
                }

            }
            else {
                if (GetAsyncKeyState(0x30) & 0x8000 && !zero_keyflag) {
                    //キーを押した瞬間の処理
                    zero_keyflag = true;
                    set_foreground();
                }
                for (int i = 1; i < 10; i++) {
                    if (GetAsyncKeyState(0x30 + i) & 0x8000 && !trans_keyflag[i - 1]) {
                        trans_keyflag[i - 1] = true;

                        int transparency = (255 * i) / 9;
                        set_transparency(transparency);

                    }
                }

            }

        }
    }

    return (int)msg.wParam;
}

void set_transparency(int transparency) {
    HWND curWindow = GetForegroundWindow();

    // ウィンドウの長いスタイルを取得
    LONG_PTR lStyle = GetWindowLongPtr(curWindow, GWL_EXSTYLE);

    // WS_EX_LAYERED スタイルを設定
    SetWindowLongPtr(curWindow, GWL_EXSTYLE, lStyle | WS_EX_LAYERED);

    // 不透明度を設定（0（完全に透明）から255（完全に不透明））
    SetLayeredWindowAttributes(curWindow, 0, transparency, LWA_ALPHA);
}

void set_foreground() {

    HWND curWindow = GetForegroundWindow();//FindWindowA("Chrome_WidgetWin_1",NULL);
    if (curWindow == NULL) {
        TCHAR coldebug[50];
        _stprintf_s(coldebug, 50, TEXT("3"));
        SetWindowText(hWnd, coldebug);
        return;
    }
    if (WindowSet.find(curWindow) == WindowSet.end()) {
        WindowSet.insert(curWindow);
        SetWindowPos(curWindow, HWND_TOPMOST, 0, 0, 0, 0, (SWP_NOSIZE | SWP_SHOWWINDOW | SWP_NOMOVE));
        TCHAR coldebug[50];
        _stprintf_s(coldebug, 50, TEXT("TOP"));
        SetWindowText(hWnd, coldebug);
    }
    else {
        WindowSet.erase(curWindow);
        SetWindowPos(curWindow, HWND_NOTOPMOST, 0, 0, 0, 0, (SWP_NOSIZE | SWP_SHOWWINDOW | SWP_NOMOVE));
        TCHAR coldebug[50];
        _stprintf_s(coldebug, 50, TEXT("NOTOP"));
        SetWindowText(hWnd, coldebug);
    }

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    TCHAR coldebug[50];
    PAINTSTRUCT ps;
    HDC hdc;
    TCHAR greeting[] = _T("ACTIVEですー");
    switch (message)
    {
    case WM_PAINT:

        hdc = BeginPaint(hWnd, &ps);

        TextOut(hdc,
            5, 5,
            greeting, _tcslen(greeting));

        EndPaint(hWnd, &ps);

        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 0;
}