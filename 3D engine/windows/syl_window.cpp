#include "syl_window.h"

sOpenGL OGL;
sView VIEW;

float m_MposX;
float m_MposY;

int direction;

bool wireframe;
bool speed;






/* Windows Message System */

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)             /* Window procedure, handles messages */
{
    switch (msg)
    {
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_SIZE:
        glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
        break;

    case WM_KEYDOWN:
        switch ( wParam )
        {
        case 0x57:
            if (speed == false)
            {
                VIEW.m_pCamera->moveZ(-0.5);
            }
            else if (speed == true)
            {
                VIEW.m_pCamera->moveZ(-3.0);
            }
            break;

        case 0x41:
            if (speed == false)
            {
                VIEW.m_pCamera->moveX(-0.5);
            }
            else if (speed == true)
            {
                VIEW.m_pCamera->moveX(-3.0);
            }
            break;

        case 0x53:
            if (speed == false)
            {
                VIEW.m_pCamera->moveZ(0.5);
            }
            else if (speed == true)
            {
                VIEW.m_pCamera->moveZ(3.0);
            }
            break;

        case 0x44:
            if (speed == false)
            {
                VIEW.m_pCamera->moveX(0.5);
            }
            else if (speed == true)
            {
                VIEW.m_pCamera->moveX(3.0);
            }
            break;

        case 0x46:
            if (speed == false)
            {
                speed = true;
            }
            else if (speed == true)
            {
                speed = false;
            }
            break;

        case 0x52:
            VIEW.LIGHT.m_red = 1;
            VIEW.LIGHT.m_green = 0;
            VIEW.LIGHT.m_blue = 0;
            break;

        case 0x47:
            VIEW.LIGHT.m_red = 0;
            VIEW.LIGHT.m_green = 1;
            VIEW.LIGHT.m_blue = 0;
            break;

        case 0x42:
            VIEW.LIGHT.m_red = 0;
            VIEW.LIGHT.m_green = 0;
            VIEW.LIGHT.m_blue = 1;
            break;

        case 0x4E:
            VIEW.LIGHT.m_red = 1;
            VIEW.LIGHT.m_green = 1;
            VIEW.LIGHT.m_blue = 1;
            break;

        case VK_SUBTRACT:
            direction = -1;
            break;

        case VK_ADD:
            direction = 1;
            break;

        case 0x58:
            VIEW.m_fireStreamLeft.xGravity += 0.1 * direction;
            VIEW.m_fireStreamRight.xGravity += 0.1 * direction;
            break;

        case 0x59:
            VIEW.m_fireStreamLeft.yGravity += 0.1 * direction;
            VIEW.m_fireStreamRight.yGravity += 0.1 * direction;
            break;

        case 0x5A:
            VIEW.m_fireStreamLeft.zGravity += 0.1 * direction;
            VIEW.m_fireStreamRight.zGravity += 0.1 * direction;
            break;

        case 0x43:
            if (wireframe == true)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                wireframe = false;
                VIEW.wireOptions = false;
            }
            else if (wireframe == false)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                wireframe = true;
                VIEW.wireOptions = true;
            }
            break;

        case 0x4F:
            if (VIEW.waterColor == true)
            {
                VIEW.waterColor = false;
            }
            else if (VIEW.waterColor == false)
            {
                VIEW.waterColor = true;
            }
            break;
        }
        break;

    case WM_RBUTTONDOWN:
        m_MposX = LOWORD(lParam);
        m_MposY = HIWORD(lParam);
        break;

    case WM_MOUSEMOVE:
        switch (wParam)
        {
        case MK_RBUTTON:

            if (m_MposX > LOWORD(lParam))
            {
                VIEW.m_pCamera->yaw(1.5);
            }
            if (m_MposX < LOWORD(lParam))
            {
                VIEW.m_pCamera->yaw(-1.5);
            }
            if (m_MposY > HIWORD(lParam))
            {
                VIEW.m_pCamera->pitch(1.5);
            }
            if (m_MposY < HIWORD(lParam))
            {
                VIEW.m_pCamera->pitch(-1.5);
            }

            m_MposX = LOWORD(lParam);
            m_MposY = HIWORD(lParam);
            break;
        }
        break;

    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);                                /* Returns values of already processed messages */
    }

    /* Necessary return statement - should never be reached */

    return 0;
}






void sWindow::create(int m_width, int m_height)
{
    direction = 1;

    wireframe = false;
    speed = false;

    HINSTANCE hInstance = NULL;                                                         /* Program handle, refers to this instance */

    HWND hWnd = NULL;                                                                   /* Declaring and assigning the window handle */
    HDC hDC = NULL;                                                                     /* Declaring and assigning the device context */
    HGLRC hRC = NULL;

    MSG msg;                                                                            /* Declaring and assigning the message variable */
    WNDCLASSEX WndClsEx;                                                                /* Declaring and assigning the window class */

    WndClsEx.cbSize = sizeof(WNDCLASSEX);                                               /* Size of the window class */
    WndClsEx.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;		                        /* Redraw on move, device context owned by window */
    WndClsEx.lpfnWndProc = WndProc;                                                     /* Reference to the window procedure */
    WndClsEx.cbClsExtra = 0;                                                            /* Amount of extra data in memory */
    WndClsEx.cbWndExtra = 0;                                                            /* Amount of extra data in memory per window */
    WndClsEx.hInstance = hInstance;                                                     /* Handle to the program's instance */
    WndClsEx.hIcon = LoadIcon(NULL, IDI_APPLICATION);                                   /* Icon shown by Alt + Tab */
    WndClsEx.hCursor = LoadCursor(NULL, IDC_ARROW);
    WndClsEx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    WndClsEx.lpszMenuName = NULL;                                                       /* Name of the menu */
    WndClsEx.lpszClassName = "syl_engine";                                              /* Class identifier */
    WndClsEx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);                                 /* Small icon shown next to name in title bar */

    RegisterClassEx(&WndClsEx);                                                         /* Register the class */

    hWnd = CreateWindow("syl_engine",                                                   /* Name of the class we previously created */
                        "syl_engine",                                                   /* Window title */
                        WS_OVERLAPPEDWINDOW,                                            /* Window style */
                        CW_USEDEFAULT,                                                  /* X-coordinate */
                        CW_USEDEFAULT,                                                  /* Y-coordinate */
                        m_width,                                                        /* Width of the window */
                        m_height,                                                       /* Height of the window */
                        NULL,                                                           /* Parent window handle (NULL = this is the parent window) */
                        NULL,                                                           /* Menu handle (NULL = no menu existing) */
                        hInstance,                                                      /* Application instance handle */
                        NULL);                                                          /* Additional things used in the creation process; mostly NULL */

    OGL.bindContext(hWnd, hDC, hRC);
    VIEW.setup();

    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    while (quit == 0)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                quit = 1;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            VIEW.draw();
            SwapBuffers(hDC);
        }
    }

    OGL.destroyContext(hWnd, hDC, hRC);
}
