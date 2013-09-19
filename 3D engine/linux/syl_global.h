#ifndef _SYL_GLOBAL
#define _SYL_GLOBAL

#include <stdio.h>
#include <time.h>

#define	streamCount 10000

struct m_struct_OBJECT
{
    float * m_pVertex;
    float * m_pTextureCoord;
    float * m_pNormal;
    int m_faceCount;
};

struct m_struct_PARTICLE
{
    float xPosition;
    float yPosition;
    float zPosition;
    float xSpeed;
    float ySpeed;
    float zSpeed;
    float xGravity;
    float yGravity;
    float zGravity;
    float r;
    float g;
    float b;
    float life;
    float reduce;
    bool newParticles;
};

struct m_struct_SETTING
{
    float xPosition;
    float yPosition;
    float zPosition;
    float * xSpeed;
    float * ySpeed;
    float * zSpeed;
    float xGravity;
    float yGravity;
    float zGravity;
    float r;
    float g;
    float b;
    float colorChangeR;
    float colorChangeG;
    float colorChangeB;
    float * life;
    float * lifePersist;
    float reduce;
};

struct m_struct_EFFECT
{
    m_struct_PARTICLE * info;
};

class sTime
{
public:

    /* Variable declaration */

    clock_t countSecond;
    clock_t rotationTick;

    int saveFPS;

    /* Function declaration */

    sTime()
    {
        countSecond = clock() + 1 * CLOCKS_PER_SEC;
        rotationTick = clock() + 0.025 * CLOCKS_PER_SEC;
    };

    void perSecondTick(int & fpsCountArg, float & spacestationRotateYArg);
};

#endif
