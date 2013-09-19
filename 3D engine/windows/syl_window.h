#ifndef _SYL_WINDOW
#define _SYL_WINDOW

#include <stdio.h>

#include "syl_gl.h"
#include "syl_view.h"

class sWindow
{
public:

    /* Variable declaration */

    int quit;

    /* Function declaration */

    sWindow()
    {
        quit = 0;
    };

    void create(int m_width, int m_height);
};

#endif
