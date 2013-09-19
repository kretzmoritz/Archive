#ifndef _SYL_VIEW
#define _SYL_VIEW

/* Allows the usage of function calls provided by OpenGL versions above 1.1 */

#define GL_GLEXT_PROTOTYPES

#include <stdio.h>
#include <GL/gl.h>
#include <GL/glx.h>

#include "syl_loader.h"
#include "syl_global.h"
#include "syl_particle.h"

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

    GLuint m_vboSkyboxID;
    GLuint m_vboGroundID;
    GLuint m_vboTerrainID;
    GLuint m_vboSpacestationID;
    GLuint m_vboSpaceshipID;
    GLuint m_vboTreeSmallID;
    GLuint m_vboTreeLargeID;
    GLuint m_vboWaterID;
    GLuint m_vboInterfaceID;
    int m_GroundNormalSize;
    int m_TerrainNormalSize;
    int m_vboDataSizeVertexSpacestation;
    int m_vboDataSizeTextureCoordSpacestation;
    int m_vboDataSizeNormalSpacestation;
    int m_vboDataSizeVertexSpaceship;
    int m_vboDataSizeTextureCoordSpaceship;
    int m_vboDataSizeNormalSpaceship;
    int m_vboDataSizeVertexTreeSmall;
    int m_vboDataSizeTextureCoordTreeSmall;
    int m_vboDataSizeNormalTreeSmall;
    int m_vboDataSizeVertexTreeLarge;
    int m_vboDataSizeTextureCoordTreeLarge;
    int m_vboDataSizeNormalTreeLarge;
    int m_vboDataSizeTextureCoordInterface;
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
    GLuint whichTree[2];
    int whichTreeTexture[2];
    int whichVBOVertex[2];
    int whichVBOTexture[2];
    int whichVBOFaceCount[2];
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
    void update(Display * m_pDisplayArg, Window & m_WindowArg, bool m_singleBufferArg);
    void light();
    void shadowCalc(float * shadowMatrix, float * lightPosition, float * surface);
    void shadowDraw();
    void particleSetting();
    void particleMovement();
    int heightCalc(float * heightMap, int X, int Z);
    void setupTerrain(float * heightMap);
    void plantVegetation(float * heightMap);
    void interface();
};

#endif
