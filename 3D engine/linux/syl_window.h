#ifndef _SYL_WINDOW
#define _SYL_WINDOW

#include <stdio.h>
#include <X11/Xlib.h>

#include "syl_gl.h"
#include "syl_view.h"

class sWindow
{
public:

    /* Variable declaration */

    Display * m_pDisplay;
    Window m_parent;
    Window m_window;

    Colormap m_colormap;
    XSetWindowAttributes m_setWindow;

    XEvent m_event;

    float m_MposX;
    float m_MposY;

    int direction;

    bool wireframe;
    bool speed;

    /* Function declaration */

    sWindow ()
    {
        direction = 1;
        wireframe = false;
        speed = false;
    };

    void create(int m_width, int m_height);
    void event();
};

#endif
