#include "syl_gl.h"

/* Different graphic choices and settings - including single- or doubleBuffer */

static GLint graphicList1[] = {GLX_STENCIL_SIZE, 1, GLX_RGBA, GLX_DEPTH_SIZE, 16, 0};
static GLint graphicList2[] = {GLX_STENCIL_SIZE, 1, GLX_RGBA, GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, 0};
static GLint graphicList3[] = {GLX_STENCIL_SIZE, 1, GLX_RGBA, GLX_DEPTH_SIZE, 24, 0};
static GLint graphicList4[] = {GLX_STENCIL_SIZE, 1, GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, 0};






void sOpenGL::bind(Display * m_pDisplayArg)
{

    /* Set singleBuffer to equal false */

    m_singleBuffer = false;

    /* Checks for visual information */

    m_pVinfo = glXChooseVisual(m_pDisplayArg, DefaultScreen(m_pDisplayArg), graphicList4);

    /* Buffer and depth choices, error check */

    if (m_pVinfo == 0)
    {
        m_pVinfo = glXChooseVisual(m_pDisplayArg, DefaultScreen(m_pDisplayArg), graphicList2);
        if (m_pVinfo == 0)
        {
            m_pVinfo = glXChooseVisual(m_pDisplayArg, DefaultScreen(m_pDisplayArg), graphicList3);
            m_singleBuffer = true;
            if (m_pVinfo == 0)
            {
                m_pVinfo = glXChooseVisual(m_pDisplayArg, DefaultScreen(m_pDisplayArg), graphicList1);
                m_singleBuffer = true;
                if (m_pVinfo == 0)
                {
                    printf("Could not choose visual!\n");
                }
            }
        }
    }
}






void sOpenGL::context(Display * m_pDisplayArg, Window & m_WindowArg)
{

    /* Connects to the OpenGL context */

    m_glxcontext = glXCreateContext(m_pDisplayArg, m_pVinfo, 0, GL_TRUE);

    /* Binds this context to our window */

    glXMakeCurrent(m_pDisplayArg, m_WindowArg, m_glxcontext);
}
