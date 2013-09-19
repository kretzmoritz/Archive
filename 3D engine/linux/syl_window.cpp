#include "syl_window.h"

sOpenGL OGL;
sView VIEW;






void sWindow::create(int m_width, int m_height)
{

    /* Connects to the X11 server, 0 is default display */

    m_pDisplay = XOpenDisplay(0);

    /* Error Check */

    if (m_pDisplay == 0)
    {
        printf("Could not open display!\n");
    }

    /* Make this the default display */

    m_parent = DefaultRootWindow(m_pDisplay);

    /* Initialize vinfo and OpenGL */

    OGL.bind(m_pDisplay);

    /* Required window setup (colormap, events) */

    m_colormap = XCreateColormap(m_pDisplay, m_parent, OGL.m_pVinfo->visual, AllocNone);

    m_setWindow.colormap = m_colormap;
    m_setWindow.event_mask = StructureNotifyMask | Button3MotionMask | KeyPressMask | ButtonPressMask;

    /* Creates simple window, x*y, usage of visual information */

    m_window = XCreateWindow(m_pDisplay, m_parent, 0, 0, m_width, m_height, 0, OGL.m_pVinfo->depth, InputOutput, OGL.m_pVinfo->visual, CWColormap | CWEventMask, &m_setWindow);

    /* Sets the title of the program */

    XStoreName(m_pDisplay, m_window, "syl_engine");

    /* Apply current glxcontext */

    OGL.context(m_pDisplay, m_window);

    /* Shows the window on screen */

    XMapWindow(m_pDisplay, m_window);

    /* Call scene setup as soon as the window is fully created */

    VIEW.setup();
}






/* Event handler - similar to "Windows Message System" */

void sWindow::event()
{
    while (1)
    {
        while (XPending(m_pDisplay))
        {
            XNextEvent(m_pDisplay, &m_event);
            switch (m_event.type)
            {
            case ConfigureNotify:

                /* Resize the OpenGL scene to current window width and height */

                glViewport(0, 0, m_event.xconfigure.width, m_event.xconfigure.height);
                break;

            case MotionNotify:
                if (m_MposX > m_event.xmotion.x)
                {
                    VIEW.m_pCamera->yaw(1.0);
                }
                if (m_MposX < m_event.xmotion.x)
                {
                    VIEW.m_pCamera->yaw(-1.0);
                }
                if (m_MposY > m_event.xmotion.y)
                {
                    VIEW.m_pCamera->pitch(1.0);
                }
                if (m_MposY < m_event.xmotion.y)
                {
                    VIEW.m_pCamera->pitch(-1.0);
                }

                m_MposX = m_event.xmotion.x;
                m_MposY = m_event.xmotion.y;
                break;

            case KeyPress:
                switch (m_event.xkey.keycode)
                {

                    /* W */

                case 25:
                    if (speed == false)
                    {
                        VIEW.m_pCamera->moveZ(-0.5);
                    }
                    else if (speed == true)
                    {
                        VIEW.m_pCamera->moveZ(-3.0);
                    }
                    break;

                    /* A */

                case 38:
                    if (speed == false)
                    {
                        VIEW.m_pCamera->moveX(-0.5);
                    }
                    else if (speed == true)
                    {
                        VIEW.m_pCamera->moveX(-3.0);
                    }
                    break;

                    /* S */

                case 39:
                    if (speed == false)
                    {
                        VIEW.m_pCamera->moveZ(0.5);
                    }
                    else if (speed == true)
                    {
                        VIEW.m_pCamera->moveZ(3.0);
                    }
                    break;

                    /* D */

                case 40:
                    if (speed == false)
                    {
                        VIEW.m_pCamera->moveX(0.5);
                    }
                    else if (speed == true)
                    {
                        VIEW.m_pCamera->moveX(3.0);
                    }
                    break;

                    /* F */

                case 41:
                    if (speed == false)
                    {
                        speed = true;
                    }
                    else if (speed == true)
                    {
                        speed = false;
                    }
                    break;

                    /* LIGHT RED - r */

                case 27:
                    VIEW.LIGHT.m_red = 1;
                    VIEW.LIGHT.m_green = 0;
                    VIEW.LIGHT.m_blue = 0;
                    break;

                    /* LIGHT GREEN - g */

                case 42:
                    VIEW.LIGHT.m_red = 0;
                    VIEW.LIGHT.m_green = 1;
                    VIEW.LIGHT.m_blue = 0;
                    break;

                    /* LIGHT BLUE - b */

                case 56:
                    VIEW.LIGHT.m_red = 0;
                    VIEW.LIGHT.m_green = 0;
                    VIEW.LIGHT.m_blue = 1;
                    break;

                    /* LIGHT NORMAL - n */

                case 57:
                    VIEW.LIGHT.m_red = 1;
                    VIEW.LIGHT.m_green = 1;
                    VIEW.LIGHT.m_blue = 1;
                    break;

                    /* Minus - */

                case 82:
                    direction = -1;
                    break;

                    /* Plus + */

                case 86:
                    direction = 1;
                    break;

                    /* Gravity X */

                case 53:
                    VIEW.m_fireStreamLeft.xGravity += 0.1 * direction;
                    VIEW.m_fireStreamRight.xGravity += 0.1 * direction;
                    break;

                    /* Gravity Y */

                case 29:
                    VIEW.m_fireStreamLeft.yGravity += 0.1 * direction;
                    VIEW.m_fireStreamRight.yGravity += 0.1 * direction;
                    break;

                    /* Gravity Z */

                case 52:
                    VIEW.m_fireStreamLeft.zGravity += 0.1 * direction;
                    VIEW.m_fireStreamRight.zGravity += 0.1 * direction;
                    break;

                    /* Wireframe - c */

                case 54:
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

                    /* Water or lava - o */

                case 32:
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

            case ButtonPress:
                switch (m_event.xbutton.button)
                {

                    /* Right mouse button */

                case 3:
                    m_MposX = m_event.xmotion.x;
                    m_MposY = m_event.xmotion.y;
                    break;
                }
                break;
            }
        }

        /* Draws and updates the scene if needed */

        VIEW.draw();
        VIEW.update(m_pDisplay, m_window, OGL.m_singleBuffer);
    }
}
