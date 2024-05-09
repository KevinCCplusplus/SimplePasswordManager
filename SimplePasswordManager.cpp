#include <Windows.h>
#include "Resource.h"
#include "ConfigurePasswordDialog.h"

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[] = L"Simple Password Manager";   // The title bar text
WCHAR szWindowClass[] = L"SimplePasswordManager"; // the main window class name

// Forward declarations of functions included in this code module:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Main entry point
int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize global strings
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, NULL, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

// Register the window class
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);

    return RegisterClassExW(&wcex);
}

// Initialize the window instance
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, 800, 600, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    // Create the Configure Password button
    HWND hButtonConfigurePassword = CreateWindowW(L"BUTTON", L"Configure Password", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        0, 0, 200, 50, hWnd, (HMENU)IDD_CONFIGURE_PASSWORD, hInstance, nullptr);

    if (!hButtonConfigurePassword)
    {
        return FALSE;
    }

    // Create the Login button
    HWND hButtonLogin = CreateWindowW(L"BUTTON", L"Login", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        0, 0, 200, 50, hWnd, (HMENU)IDM_LOGIN, hInstance, nullptr);

    if (!hButtonLogin)
    {
        return FALSE;
    }

    // Calculate the position to place the buttons
    RECT rcClient;
    GetClientRect(hWnd, &rcClient);
    int buttonWidth = 200;
    int buttonHeight = 50;
    int xPos = (rcClient.right - rcClient.left - buttonWidth) / 2;
    int yPos = (rcClient.bottom - rcClient.top - 2 * buttonHeight - 20) / 2; // 20 is the vertical spacing between buttons

    // Set the position of the Configure Password button
    SetWindowPos(hButtonConfigurePassword, nullptr, xPos, yPos, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    // Set the position of the Login button
    yPos += buttonHeight + 20; // Move down for the second button
    SetWindowPos(hButtonLogin, nullptr, xPos, yPos, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    // Show the window
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}


// Window Procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDD_CONFIGURE_PASSWORD:
            // Handle the "Configure Password" button click event here
            ShowConfigurePasswordDialog(hWnd); // Roep de functie ShowConfigurePasswordDialog aan
            break;
        case IDM_LOGIN:
            // Handle the "Login" button click event here
            MessageBox(hWnd, L"Login button clicked!", L"Info", MB_OK);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
