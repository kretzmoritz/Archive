#ifndef _SYL_MATH
#define _SYL_MATH

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <vector>

using namespace std;

class sVector
{
public:

    /* Variable declaration */

    float m_x;
    float m_y;
    float m_z;

    /* Function declaration */

    sVector()
    {
        m_x = 0;
        m_y = 0;
        m_z = 0;
    };

    sVector(float a, float b, float c);

    sVector operator + (sVector);
    sVector operator * (float a);

    float * normalUnit(float * input, int size, int corner, int terrainCalc);
};

class sTexture
{
public:

    /* Variable declaration */

    float m_u;
    float m_v;

    /* Function declaration */

    sTexture()
    {
        m_u = 0;
        m_v = 0;
    };

    sTexture(float a, float b);
};

class sMatrix
{
public:

    /* Variable declaration */

    float m_main[4][4];

    /* Function declaration */

    sMatrix();

    sMatrix operator * (sMatrix);
    operator float* ()
    {
        return (float*) m_main;
    }

    sMatrix fillModel(float x, float y, float z);
    sMatrix fillView(sVector & m_vArgPosition, sVector & m_vArgX, sVector & m_vArgY, sVector & m_vArgZ);
};

#endif
