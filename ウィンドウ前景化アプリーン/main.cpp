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

void change();
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
    wcex.style = CS_HREDRAW | CS_VREDRAW;
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
    BOOL keyflg = false;
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
            

            if (GetAsyncKeyState(0x30) & 0x8000 && GetAsyncKeyState(VK_CONTROL) & 0x8000){
                if (!keyflg) {
                    //キーを押した瞬間の処理
                    keyflg = true;
                    change();
                }
                //キーを押している間の処理
            }
            else {
                //キーを離している間の処理
                keyflg = false;
            }
        }
    }

    return (int)msg.wParam;
}


void change() {
    
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