#ifndef _SYL_VIEW
#define _SYL_VIEW

#include <windows.h>
#include <stdio.h>
#include <GL/gl.h>

#include "syl_loader.h"
#include "syl_global.h"
#include "syl_particle.h"

#define GL_TEXTURE0                        0x84C0
#define GL_TEXTURE1                        0x84C1
#define GL_TEXTURE2                        0x84C2
#define GL_TEXTURE3                        0x84C3

/* #define GL_ARRAY_BUFFER_ARB 0x8892 */
/* #define GL_STATIC_DRAW_ARB 0x88E4 */

#define INT_MAP_SIZE 1024
#define MAP_SIZE 1024.0
#define	STEP_SIZE 16.0
#define HEIGHT_RATIO 40.0

#define MAX_TREE_COUNT 100

class sCamera
{
public:

    /* Variable declaration */

    sVector m_Angle;
    sVector m_vPosition;
    sVector m_vX;
    sVector m_vY;
    sVector m_vZ;

    sMatrix m_MatrixAccess;

    /* Function declaration */

    void angleToVector();
    sMatrix declareCamera();

    void moveX(float m_length);
    void moveY(float m_length);
    void moveZ(float m_length);

    void pitch(float m_amount);
    void yaw(float m_amount);
    void roll(float m_amount);
};

class sView
{
public:

    /* API declaration */

    typedef void (APIENTRY * PFNGLACTIVETEXTUREPROC) (GLenum texture);
    typedef void (APIENTRY * PFNGLCLIENTACTIVETEXTUREPROC) (GLenum texture);

    /* typedef INT_PTR GLintptrARB; */
    /* typedef INT_PTR GLsizeiptrARB; */
    /* typedef void (APIENTRY * PFNGLGENBUFFERSARBPROC) (GLsizei n, GLuint * buffers); */
    /* typedef void (APIENTRY * PFNGLBINDBUFFERARBPROC) (GLenum target, GLuint buffer); */
    /* typedef void (APIENTRY * PFNGLBUFFERDATAARBPROC) (GLenum target, GLsizeiptrARB size, const GLvoid * data, GLenum usage); */
    /* typedef void (APIENTRY * PFNGLBUFFERSUBDATAARBPROC) (GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid * data); */

    /* Declare VBO extensions */

    PFNGLACTIVETEXTUREPROC glActiveTexture;
    PFNGLCLIENTACTIVETEXTUREPROC glClientActiveTexture;

    /* PFNGLGENBUFFERSARBPROC glGenBuffersARB; */
    /* PFNGLBINDBUFFERARBPROC glBindBufferARB; */
    /* PFNGLBUFFERDATAARBPROC glBufferDataARB; */
    /* PFNGLBUFFERSUBDATAARBPROC glBufferSubDataARB; */

    /* Variable declaration */

    struct m_struct_LIGHT
    {
        sVector m_positon;
        float m_red;
        float m_green;
        float m_blue;
    } LIGHT;

    GLfloat POSITION_DIFFUSE[4];
    GLfloat DIFFUSE[4];

    GLfloat POSITION_AMBIENT[4];
    GLfloat AMBIENT[4];

    float shadow[16];

    sCamera * m_pCamera;

    m_struct_OBJECT m_objectSpacestation;
    m_struct_OBJECT m_objectSpaceship;
    m_struct_OBJECT m_objectTreeSmall;
    m_struct_OBJECT m_objectTreeLarge;

    int m_textureIDSkyboxDown;
    int m_textureIDSkyboxUp;
    int m_textureIDSkybox1;
    int m_textureIDSkybox2;
    int m_textureIDSkybox3;
    int m_textureIDSkybox4;
    int m_textureIDSpacestation;
    int m_textureIDSpaceship;
    int m_textureIDParticle;
    int m_textureIDTerrain;
    int m_textureIDTreeSmall;
    int m_textureIDTreeLarge;
    int m_textureIDWater1;
    int m_textureIDWater2;
    int m_textureIDWater3;
    int m_textureIDWater4;
    int m_textureIDNumber;
    int polygonCountX;
    int polygonCountZ;
    int objectCount;
    int fpsCount;
    float spacestationRotateY;

    m_struct_SETTING m_fireStreamLeft;
    m_struct_SETTING m_fireStreamRight;

    int polygonLoop;

    bool wireOptions;

    GLfloat extractMatrix[16];
    sVector m_right;
    sVector m_up;

    int x, y, z;
    float coordX;
    float coordZ;
    float coordAddX;
    float coordAddZ;

    vector<float> terrainVertex;
    vector<float> terrainTextureCoord;

    int treeGridX;
    int treeGridZ;
    int treeLoop;
    int whichTreeTexture[2];
    float * whichVAVertex[2];
    float * whichVATexture[2];
    float * whichVANormal[2];
    int whichVAFaceCount[2];
    int xPosTree[MAX_TREE_COUNT];
    int yPosTree[MAX_TREE_COUNT];
    int zPosTree[MAX_TREE_COUNT];
    int chooseTree[MAX_TREE_COUNT];

    float waterX;
    float waterY;
    bool waterColor;

    int tempContainer;
    int splitValue[4];

    /* Function declaration */

    sView()
    {
        fpsCount = 0;
        spacestationRotateY = 0;
        LIGHT.m_positon.m_x = -20;
        LIGHT.m_positon.m_y = 7.5;
        LIGHT.m_positon.m_z = -7.5;
        LIGHT.m_red = 1;
        LIGHT.m_green = 1;
        LIGHT.m_blue = 1;
        wireOptions = false;
        treeLoop = 0;
        waterX = 0.00125;
        waterY = 0.00125;
        waterColor = true;
    };

    void setup();
    void draw();
    void drawSkybox();
    void drawGround();
    void drawTerrain();
    void drawTree();
    void drawWater();
    void drawSpacestation();
    void drawSpaceship();
    void light();
    void shadowCalc(float * shadowMatrix, float * lightPosition, float * surface);
    void shadowDraw();
    void particleSetting();
    void particleMovement();
    int heightCalc(float * heightMap, int X, int Z);
    void setupTerrain(float * heightMap);
    void plantVegetation(float * heightMap);
    void interfaceWindows();
};

#endif
