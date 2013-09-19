#include "window.h"

cGlConnect glContext;
cView handler;

float scale;
int scrolled;

/* --- MENU --- */

void addMenu(HWND hWnd)
{
    HMENU hMenubar;
    HMENU hFile;
    HMENU hOtherSettings;

    hMenubar = CreateMenu();
    hFile = CreateMenu();
    hOtherSettings = CreateMenu();

    AppendMenu(hFile, MF_STRING, IDM_FILE_OPEN_MODEL, "&Open Model");
    AppendMenu(hFile, MF_STRING, IDM_FILE_OPEN_TEXTURE, "&Open Texture");
    AppendMenu(hFile, MF_STRING, IDM_FILE_OPEN_SHADER, "&Open Shader");

    AppendMenu(hOtherSettings, MF_STRING, IDM_OTHERSETTINGS_WIREFRAME, "&Wireframe");
    AppendMenu(hOtherSettings, MF_STRING, IDM_OTHERSETTINGS_DISABLE_TEXTURE, "&Disable Texture");
    AppendMenu(hOtherSettings, MF_STRING, IDM_OTHERSETTINGS_DISABLE_SHADER, "&Disable Shader");

    AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hFile, "&File");
    AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hOtherSettings, "&Other Settings");
    SetMenu(hWnd, hMenubar);
}

void loadFile(LPSTR file, HWND hWnd)
{
    HANDLE hFile;
    DWORD dwSize;
    DWORD dw;

    LPBYTE lpBuffer = NULL;

    hFile = CreateFile(file, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
    dwSize = GetFileSize(hFile, NULL);
    lpBuffer = (LPBYTE) HeapAlloc(GetProcessHeap(), HEAP_GENERATE_EXCEPTIONS, dwSize + 1);
    ReadFile(hFile, (LPWSTR)lpBuffer, dwSize, &dw, NULL);
    CloseHandle(hFile);
    lpBuffer[dwSize] = 0;
    HeapFree(GetProcessHeap(), 0, lpBuffer);
}

void openDialog(int type, HWND hWnd)
{
    OPENFILENAME ofn;
    TCHAR szFile[MAX_PATH];

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.hwndOwner = hWnd;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = TEXT("All files(*.*)\0*.*\0");
    ofn.nFilterIndex = 1;
    ofn.lpstrInitialDir = NULL;
    ofn.lpstrFileTitle = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if(GetOpenFileName(&ofn))
    {
        if(type == 1)
        {
            scale = 0.01;
        }

        loadFile(ofn.lpstrFile, hWnd);
        handler.changeOutput(type, ofn);
        std::cout << ofn.lpstrFile << std::endl;
    }
}

/* --- MENU --- */

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_CREATE:
            addMenu(hWnd);
            break;

        case WM_CLOSE:
            DestroyWindow(hWnd);
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        case WM_SIZE:
            glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
            break;

        case WM_MOUSEWHEEL:
            scrolled = HIWORD (wParam) / 120;

            if(scrolled == 545)
            {
                if(scale > 0.002)
                {
                    scale -= 0.001;
                }
            }
            else if(scrolled == 1)
            {
                scale += 0.001;
            }

            handler.scaleOBJ(scale);
            break;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDM_FILE_OPEN_MODEL:
                    openDialog(1, hWnd);
                    break;
                case IDM_FILE_OPEN_TEXTURE:
                    openDialog(2, hWnd);
                    break;
                case IDM_FILE_OPEN_SHADER:
                    openDialog(3, hWnd);
                    break;
                case IDM_OTHERSETTINGS_WIREFRAME:
                    if(!handler.wireframe)
                    {
                        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                        handler.wireframe = true;

                        glDisable(GL_TEXTURE_2D);
                        glDisable(GL_LIGHTING);
                    }
                    else if(handler.wireframe)
                    {
                        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                        handler.wireframe = false;

                        glEnable(GL_TEXTURE_2D);
                        glEnable(GL_LIGHTING);
                    }
                    break;
                case IDM_OTHERSETTINGS_DISABLE_TEXTURE:
                    handler.disable(1);
                    break;
                case IDM_OTHERSETTINGS_DISABLE_SHADER:
                    handler.disable(2);
                    break;
            }
            break;

        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    return 0;
}

cWindow::cWindow()
{
    WndClsEx.cbSize = sizeof(WNDCLASSEX);
    WndClsEx.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    WndClsEx.lpfnWndProc = WndProc;
    WndClsEx.cbClsExtra = 0;
    WndClsEx.cbWndExtra = 0;
    WndClsEx.hInstance = hInstance;
    WndClsEx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    WndClsEx.hCursor = LoadCursor(NULL, IDC_ARROW);
    WndClsEx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    WndClsEx.lpszMenuName = NULL;
    WndClsEx.lpszClassName = "shader_test";
    WndClsEx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    quit = false;

    scale = 0.01;
}

void cWindow::create(int width, int height)
{
    RegisterClassEx(&WndClsEx);

    hWnd = CreateWindow("shader_test",
                        "shader_test",
                        WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT,
                        CW_USEDEFAULT,
                        width,
                        height,
                        NULL,
                        NULL,
                        hInstance,
                        NULL);

    glContext.bind(hWnd, hDC, hRC);
    handler.init();

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    while (!quit)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                quit = true;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            handler.draw();
            SwapBuffers(hDC);
        }
    }

    glContext.destroy(hWnd, hDC, hRC);
}
