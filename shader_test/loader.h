#ifndef _LOADER
#define _LOADER

#define GLEW_STATIC

#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <cstring>

#include "libs/picopng.h"

class cLoader
{
    private:

    struct VECTOR
    {
        float x;
        float y;
        float z;
    };

    struct TEXTURE
    {
        float u;
        float v;
    };

    struct OBJECT
    {
        float *vertex;
        float *const_vertex;
        float *texture;
        float *normal;
        int faceCount;
    };

    int m_iTexture;

    bool checkLittleEndian();
    int byteToInt(unsigned char *input);
    char *readShader(char *input);
    int initTexture(unsigned char *input, int w, int h);

    public:

    OBJECT model;

    cLoader();
    void initGlewLib();
    GLuint loadShader(char *input_vert, char *input_frag);
    int loadPNGLib(char *input);
    void loadOBJ(char *input);

    /* --- NOT FINISHED --- */

    unsigned char *loadPNG(char *input);

    /* --- NOT FINISHED --- */
};

#endif
