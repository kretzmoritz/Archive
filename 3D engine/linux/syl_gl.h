#ifndef _SYL_GL
#define _SYL_GL

#include <stdio.h>
#include <GL/glx.h>

class sOpenGL
{
public:

    /* Variable declaration */

    bool m_singleBuffer;
    XVisualInfo * m_pVinfo;
    GLXContext m_glxcontext;

    /* Function declaration */

    void bind(Display * m_pDisplayArg);
    void context(Display * m_pDisplayArg, Window & m_WindowArg);
};

#endif
