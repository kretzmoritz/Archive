#ifndef _SYL_LOADER
#define _SYL_LOADER

#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "syl_math.h"
#include "syl_global.h"

#define GL_CLAMP_TO_EDGE 0x812F

class sImageLoader
{
public:

    /* Variable declaration */

    /* Compatibility with Microsoft Windows compilers needed -> usage of #pragma pack(2) for 2-byte alignment */

#pragma pack(2)

    struct m_struct_BITMAPFILEHEADER
    {
        unsigned short  bfType;
        unsigned int    bfSize;
        unsigned short  bfReserved1;
        unsigned short  bfReserved2;
        unsigned int    bfOffBits;
    } BITMAPFILEHEADER;

    struct m_struct_BITMAPINFOHEADER
    {
        unsigned int    biSize;
        int             biWidth;
        int             biHeight;
        unsigned short  biPlanes;
        unsigned short  biBitCount;
        unsigned int    biCompression;
        unsigned int    biSizeImage;
        int             biXPelsPerMeter;
        int             biYPelsPerMeter;
        unsigned int    biClrUsed;
        unsigned int    biClrImportant;
    } BITMAPINFOHEADERArg;

    /* Usage of unsigned char - exactly 1 byte in memory */

    struct m_struct_RGBA
    {
        unsigned char   rgbBlue;
        unsigned char   rgbGreen;
        unsigned char   rgbRed;
        unsigned char   rgbReserved;
    };

#pragma pack()

    FILE * m_pFileHandle;
    m_struct_RGBA * m_pColor;
    unsigned char * m_pReference;
    unsigned char * m_pBitmap;
    int m_IndexMemory;
    int m_byteJump;
    unsigned char m_tempRGB;

    unsigned char * m_pBitmapContainer;
    int m_imageID;

    float * m_pHeightmap;

    /* Function declaration */

    sImageLoader ()
    {
        m_imageID = 0;
    };

    unsigned char * loadBitmap (char * m_pFileName, m_struct_BITMAPINFOHEADER * BITMAPINFOHEADER, int heightMap);
    int initializeTexture(char * m_pFileName, int heightMap);
};

class sObjectLoader
{
public:

    /* Variable declaration */

    FILE * m_pFileHandle;
    char readLine[100];

    int m_vertexCount;
    int m_textureCoordCount;
    int m_normalCount;

    sVector * m_pVertex;
    sTexture * m_pTextureCoord;
    sVector * m_pNormal;

    int * m_pVertexIndex;
    int * m_pTextureCoordIndex;
    int * m_pNormalIndex;

    int m_vertexOffset;
    int m_textureCoordOffset;
    int m_normalOffset;
    int m_faceOffset;

    int loopCount;
    int objectIndex;

    /* Function declaration */

    void loadObject (char * m_pFileName, float scale, m_struct_OBJECT & OBJECT);
};

#endif
