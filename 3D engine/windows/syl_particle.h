#ifndef _SYL_PARTICLE
#define _SYL_PARTICLE

#include <stdlib.h>
#include <string.h>
#include <GL/gl.h>

#include "syl_global.h"
#include "syl_math.h"

class sParticle
{
public:

    /* Variable declaration */

    m_struct_EFFECT particleListLeft;
    m_struct_EFFECT particleListRight;

    int loop;

    /* Function declaration */

    void dynamicSize();
    void drawParticles(m_struct_EFFECT & particleList, m_struct_SETTING & setting, int particleTexture, bool wireOptions, bool billboard, sVector right, sVector up, int polygonCount);
};

#endif
