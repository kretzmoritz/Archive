#include "syl_view.h"

sCamera CAMERA;
sImageLoader IMAGELOADER;
sObjectLoader OBJECTLOADER;
sVector VECTOR;
sTime TIME;
sParticle PARTICLE;

float m_SkyboxVertex [] =       {       2,2,2,      -2,2,2,     -2,-2,2,    2,-2,2,
                                        2,2,2,      2,-2,2,     2,-2,-2,    2,2,-2,
                                        2,2,2,      2,2,-2,     -2,2,-2,    -2,2,2,
                                        -2,2,2,     -2,2,-2,    -2,-2,-2,   -2,-2,2,
                                        -2,-2,-2,   2,-2,-2,    2,-2,2,     -2,-2,2,
                                        2,-2,-2,    -2,-2,-2,   -2,2,-2,    2,2,-2
                                };

float m_SkyboxTextureCoord [] = {       0, 1, 1, 1,
                                        1, 0, 0, 0,

                                        1, 1, 1, 0,
                                        0, 0, 0, 1,

                                        0, 1, 1, 1,
                                        1, 0, 0, 0,

                                        0, 1, 1, 1,
                                        1, 0, 0, 0,

                                        1, 1, 1, 0,
                                        0, 0, 0, 1,

                                        1, 0, 0, 0,
                                        0, 1, 1, 1
                                };

float m_GroundVertex [] =       {       -1,-3,0,    0,-3,0,     0,-3,-1,    -1,-3,-1            };

float m_WaterVertex [] =        {       -210,0,0,   0,0,0,      0,0,-210,   -210,0,-210         };

float m_WaterTextureCoord [] =  {       0,5,        5,5,        5,0,        0,0                 };

float m_InterfaceVertex [] =    {       1,1,0,      1,0,0,      0,0,0,      0,1,0               };

float m_InterfaceTextureCoord [10][8] =
{
    {       0,0.5,      0.2,0.5,    0.2,1,      0,1                 },
    {       0.2,0.5,    0.4,0.5,    0.4,1,      0.2,1               },
    {       0.4,0.5,    0.6,0.5,    0.6,1,      0.4,1               },
    {       0.6,0.5,    0.8,0.5,    0.8,1,      0.6,1               },
    {       0.8,0.5,    1.0,0.5,    1.0,1,      0.8,1               },
    {       0,0.0,      0.2,0.0,    0.2,0.5,    0,0.5               },
    {       0.2,0.0,    0.4,0.0,    0.4,0.5,    0.2,0.5             },
    {       0.4,0.0,    0.6,0.0,    0.6,0.5,    0.4,0.5             },
    {       0.6,0.0,    0.8,0.0,    0.8,0.5,    0.6,0.5             },
    {       0.8,0.0,    1.0,0.0,    1.0,0.5,    0.8,0.5             }
};

float * m_GroundNormal;
float * m_TerrainNormal;






/* Calculates our vectors (axes) using angles; needed to create the ModelView matrix: Rz, Ry, Rx - http://www.songho.ca/opengl/gl_anglestoaxes.html */

void sCamera::angleToVector()
{

    const float angleToradian = 3.14159265 / 180;

    /* Pitch (x) */

    float cX = cosf(m_Angle.m_x * angleToradian);
    float sX = sinf(m_Angle.m_x * angleToradian);

    /* Yaw (y) */

    float cY = cosf(m_Angle.m_y * angleToradian);
    float sY = sinf(m_Angle.m_y * angleToradian);

    /* Roll (z) */

    float cZ = cosf(m_Angle.m_z * angleToradian);
    float sZ = sinf(m_Angle.m_z * angleToradian);

    /* XMatrix values */

    m_vX.m_x = cZ * cY;
    m_vX.m_y = sZ * cY;
    m_vX.m_z = -(sY);

    /* YMatrix values */

    m_vY.m_x = -(sZ * cX) + (cZ * sY * sX);
    m_vY.m_y = (cZ * cX) + (sZ * sY * sX);
    m_vY.m_z = cY * sX;

    /* ZMatrix values */

    m_vZ.m_x = (sZ * sX) + (cZ * sY * cX);
    m_vZ.m_y = -(cZ * sX) + (sZ * sY * cX);
    m_vZ.m_z = cY * cX;
}






/* Declares our view matrix and thus simulates the camera */

sMatrix sCamera::declareCamera()
{
    angleToVector();
    return m_MatrixAccess.fillView(m_vPosition, m_vX, m_vY, m_vZ);
}






/* Multiplying a vector with a scalar gives us a parallel vector with a different length and possibly opposite direction */

void sCamera::moveX(float m_length)
{
    sVector m_vTemp = CAMERA.m_vX * m_length;
    CAMERA.m_vPosition = CAMERA.m_vPosition + m_vTemp;
}






/* Multiplying a vector with a scalar gives us a parallel vector with a different length and possibly opposite direction */

void sCamera::moveY(float m_length)
{
    sVector m_vTemp = CAMERA.m_vY * m_length;
    CAMERA.m_vPosition = CAMERA.m_vPosition + m_vTemp;
}






/* Multiplying a vector with a scalar gives us a parallel vector with a different length and possibly opposite direction */

void sCamera::moveZ(float m_length)
{
    sVector m_vTemp = CAMERA.m_vZ * m_length;
    CAMERA.m_vPosition = CAMERA.m_vPosition + m_vTemp;
}






void sCamera::pitch(float m_amount)
{
    CAMERA.m_Angle.m_x += m_amount;
}






void sCamera::yaw(float m_amount)
{
    CAMERA.m_Angle.m_y += m_amount;
}






void sCamera::roll(float m_amount)
{
    CAMERA.m_Angle.m_z += m_amount;
}






void sView::setup()
{

    /* Load VBO extensions */

    glActiveTexture = (PFNGLACTIVETEXTUREPROC) wglGetProcAddress("glActiveTexture");
    glClientActiveTexture = (PFNGLCLIENTACTIVETEXTUREPROC) wglGetProcAddress("glClientActiveTexture");

    /* glGenBuffersARB = (PFNGLGENBUFFERSARBPROC) wglGetProcAddress("glGenBuffersARB"); */
    /* glBindBufferARB = (PFNGLBINDBUFFERARBPROC) wglGetProcAddress("glBindBufferARB"); */
    /* glBufferDataARB = (PFNGLBUFFERDATAARBPROC) wglGetProcAddress("glBufferDataARB"); */
    /* glBufferSubDataARB = (PFNGLBUFFERSUBDATAARBPROC) wglGetProcAddress("glBufferSubDataARB"); */

    /* Background color is now black */

    glClearColor(0.0, 0.0, 0.0, 0.0);

    /* Enable depth-buffering */

    glEnable(GL_DEPTH_TEST);

    /* Disable rendering of faces not in view */

    glEnable(GL_CULL_FACE);

    /* Enable the ability to draw the scene using vertex arrays */

    glEnableClientState(GL_VERTEX_ARRAY);

    /* Enable 2 light sources */

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    /* Set left, right, bottom, top, near and far clipping planes */

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 2000.0);

    /* Initialize the ModelView matrix */

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /* Declare pointer to camera */

    m_pCamera = &CAMERA;

    /* Move the camera before OpenGL starts to draw */

    CAMERA.m_vPosition.m_x = -20.0;
    CAMERA.m_vPosition.m_y =   5.0;
    CAMERA.m_vPosition.m_z =  10.0;
    CAMERA.m_Angle.m_y =     305.0;

    /* Begin .obj loading - must consist of vertex, texture and normal information */

    OBJECTLOADER.loadObject((char*)"spacestation.obj", 0.01, m_objectSpacestation);
    OBJECTLOADER.loadObject((char*)"spaceship.obj", 0.02, m_objectSpaceship);
    OBJECTLOADER.loadObject((char*)"treesmall.obj", 0.5, m_objectTreeSmall);
    OBJECTLOADER.loadObject((char*)"treelarge.obj", 0.5, m_objectTreeLarge);

    /* Begin texture loading - width and height should be powers of 2 (n^2), width CAN be different than height */

    m_textureIDSkyboxDown = IMAGELOADER.initializeTexture((char*)"down.bmp", 0);
    m_textureIDSkyboxUp = IMAGELOADER.initializeTexture((char*)"up.bmp", 0);
    m_textureIDSkybox1 = IMAGELOADER.initializeTexture((char*)"1.bmp", 0);
    m_textureIDSkybox2 = IMAGELOADER.initializeTexture((char*)"2.bmp", 0);
    m_textureIDSkybox3 = IMAGELOADER.initializeTexture((char*)"3.bmp", 0);
    m_textureIDSkybox4 = IMAGELOADER.initializeTexture((char*)"4.bmp", 0);
    m_textureIDSpacestation = IMAGELOADER.initializeTexture((char*)"spacestation.bmp", 0);
    m_textureIDSpaceship = IMAGELOADER.initializeTexture((char*)"spaceship.bmp", 0);
    m_textureIDParticle = IMAGELOADER.initializeTexture((char*)"particle.bmp", 0);
    m_textureIDTreeSmall = IMAGELOADER.initializeTexture((char*)"treesmall.bmp", 0);
    m_textureIDTreeLarge = IMAGELOADER.initializeTexture((char*)"treelarge.bmp", 0);
    m_textureIDWater1 = IMAGELOADER.initializeTexture((char*)"water1.bmp", 0);
    m_textureIDWater2 = IMAGELOADER.initializeTexture((char*)"water2.bmp", 0);
    m_textureIDWater3 = IMAGELOADER.initializeTexture((char*)"water3.bmp", 0);
    m_textureIDWater4 = IMAGELOADER.initializeTexture((char*)"water4.bmp", 0);
    m_textureIDNumber = IMAGELOADER.initializeTexture((char*)"number.bmp", 0);

    /* Begin heightmap creation and initialize our terrain texture */

    m_textureIDTerrain = IMAGELOADER.initializeTexture((char*)"terrain.bmp", 1);

    /* Calculate terrain */

    setupTerrain(IMAGELOADER.m_pHeightmap);

    /* Load QUAD normals */

    m_GroundNormal = VECTOR.normalUnit(m_GroundVertex, 12, 4, 0);
    m_TerrainNormal = VECTOR.normalUnit(&terrainVertex[0], terrainVertex.size(), 4, 1);

    /* Fill our light's position array with values */

    POSITION_DIFFUSE[0] = LIGHT.m_positon.m_x;
    POSITION_DIFFUSE[1] = LIGHT.m_positon.m_y;
    POSITION_DIFFUSE[2] = LIGHT.m_positon.m_z;
    POSITION_DIFFUSE[3] = 1;

    /* Initialize our particle system */

    particleSetting();

    /* Get coordinates for our plants */

    plantVegetation(IMAGELOADER.m_pHeightmap);

    /* Releases the allocated memory */

    delete [] IMAGELOADER.m_pHeightmap;

    /* Reset some of our unused pointers to NULL */

    m_objectTreeSmall.m_pVertex = NULL;
    m_objectTreeSmall.m_pTextureCoord = NULL;
    m_objectTreeSmall.m_pNormal = NULL;

    m_objectTreeLarge.m_pVertex = NULL;
    m_objectTreeLarge.m_pTextureCoord = NULL;
    m_objectTreeLarge.m_pNormal = NULL;
}






void sView::draw()
{

    /* Clear the color-, stencil- and depthBuffer (in order to actually see the scene correctly) */

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    /* Starts our fpsCount and rotation */

    TIME.perSecondTick(fpsCount, spacestationRotateY);

    /* Duplicates the current matrix */

    glPushMatrix();

    /* Camera matrix initialization */

    glLoadMatrixf((float*)CAMERA.declareCamera());

    /* Coloring, texturing and lighting in OpenGL */

    if (wireOptions == false)
    {
        glEnable(GL_COLOR_MATERIAL);
        glEnable(GL_TEXTURE_2D);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisable(GL_LIGHTING);
    }
    else if (wireOptions == true)
    {
        glDisable(GL_COLOR_MATERIAL);
        glDisable(GL_TEXTURE_2D);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisable(GL_LIGHTING);
    }

    /* Skybox */

    glDisable(GL_DEPTH_TEST);
    glCullFace(GL_FRONT);

    drawSkybox();

    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);

    /* Coloring, texturing and lighting in OpenGL */

    if (wireOptions == false)
    {
        glDisable(GL_TEXTURE_2D);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnable(GL_LIGHTING);
        glEnableClientState(GL_NORMAL_ARRAY);
    }

    /* Enable shadows */

    shadowDraw();

    /* Lighting options - position, color as well as which kind of lighting is used */

    light();

    /* Ground */

    drawGround();

    /* Coloring, texturing and lighting in OpenGL */

    if (wireOptions == false)
    {
        glDisable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    /* Vegetation */

    drawTree();

    /* Coloring, texturing and lighting in OpenGL */

    if (wireOptions == false)
    {
        glDisableClientState(GL_NORMAL_ARRAY);
    }

    /* Water */

    drawWater();

    /* Coloring, texturing and lighting in OpenGL */

    if (wireOptions == false)
    {
        glEnable(GL_LIGHTING);
        glEnableClientState(GL_NORMAL_ARRAY);
    }

    /* Terrain */

    drawTerrain();

    /* Spacestation */

    drawSpacestation();

    /* Spaceship */

    drawSpaceship();

    /* Get our matrix data used for billboarding */

    glGetFloatv(GL_MODELVIEW_MATRIX, extractMatrix);

    m_right.m_x = extractMatrix[0];
    m_right.m_y = extractMatrix[4];
    m_right.m_z = extractMatrix[8];

    m_up.m_x = extractMatrix[1];
    m_up.m_y = extractMatrix[5];
    m_up.m_z = extractMatrix[9];

    /* Emit our particles */

    particleMovement();

    PARTICLE.drawParticles(PARTICLE.particleListLeft, m_fireStreamLeft, m_textureIDParticle, wireOptions, true, m_right, m_up, streamCount);
    PARTICLE.drawParticles(PARTICLE.particleListRight, m_fireStreamRight, m_textureIDParticle, wireOptions, true, m_right, m_up, streamCount);

    /* Coloring, texturing and lighting in OpenGL */

    if (wireOptions == false)
    {
        glDisable(GL_LIGHTING);
        glDisableClientState(GL_NORMAL_ARRAY);
    }

    /* Interface */

    glDisable(GL_DEPTH_TEST);

    interfaceWindows();

    glEnable(GL_DEPTH_TEST);

    /* Replaces the current matrix with the one below it on the stack */

    glPopMatrix();

    /* Increments fpsCount everytime the scene is drawn */

    fpsCount++;
}






void sView::drawSkybox()
{

    /* Duplicates the current matrix */

    glPushMatrix();

    /* Move the cube with our camera */

    glTranslatef(CAMERA.m_vPosition.m_x, CAMERA.m_vPosition.m_y, CAMERA.m_vPosition.m_z);

    /* Drawing process */

    glVertexPointer(3, GL_FLOAT, 0, m_SkyboxVertex);
    glTexCoordPointer(2, GL_FLOAT, 0, m_SkyboxTextureCoord);

    glBindTexture(GL_TEXTURE_2D, m_textureIDSkybox1);
    glDrawArrays(GL_QUADS, 0, 4);

    glBindTexture(GL_TEXTURE_2D, m_textureIDSkybox2);
    glDrawArrays(GL_QUADS, 4, 4);

    glBindTexture(GL_TEXTURE_2D, m_textureIDSkyboxUp);
    glDrawArrays(GL_QUADS, 8, 4);

    glBindTexture(GL_TEXTURE_2D, m_textureIDSkybox3);
    glDrawArrays(GL_QUADS, 12, 4);

    glBindTexture(GL_TEXTURE_2D, m_textureIDSkyboxDown);
    glDrawArrays(GL_QUADS, 16, 4);

    glBindTexture(GL_TEXTURE_2D, m_textureIDSkybox4);
    glDrawArrays(GL_QUADS, 20, 4);

    /* Replaces the current matrix with the one below it on the stack */

    glPopMatrix();
}






void sView::drawGround()
{

    /* Duplicates the current matrix */

    glPushMatrix();

    /* Drawing process */

    glVertexPointer(3, GL_FLOAT, 0, m_GroundVertex);
    glNormalPointer(GL_FLOAT, 0, m_GroundNormal);

    /* Create 100 quads in order achieve high polygon ground */

    glTranslatef(-25.0, 0.0, -25.0);

    for (polygonCountZ = 0; polygonCountZ < 40; polygonCountZ++)
    {
        glTranslatef(0.0, 0.0, 1.0);

        for (polygonCountX = 0; polygonCountX < 65; polygonCountX++)
        {
            glTranslatef(1.0, 0.0, 0.0);
            glDrawArrays(GL_QUADS, 0, 4);
        }

        glTranslatef(-65.0, 0.0, 0.0);
    }

    /* Replaces the current matrix with the one below it on the stack */

    glPopMatrix();
}






void sView::drawTerrain()
{

    /* Duplicates the current matrix */

    glPushMatrix();

    /* Bind our currently used texture */

    glBindTexture(GL_TEXTURE_2D, m_textureIDTerrain);

    /* Starting position (centered) */

    glTranslatef(-MAP_SIZE / 2, -75, -MAP_SIZE / 2);

    /* Drawing process */

    glVertexPointer(3, GL_FLOAT, 0, &terrainVertex[0]);
    glTexCoordPointer(2, GL_FLOAT, 0, &terrainTextureCoord[0]);
    glNormalPointer(GL_FLOAT, 0, m_TerrainNormal);

    glDrawArrays(GL_QUADS, 0, terrainVertex.size() / 3);

    /* Replaces the current matrix with the one below it on the stack */

    glPopMatrix();
}






void sView::drawTree()
{

    /* Duplicates the current matrix */

    glPushMatrix();

    /* Starting position (centered) */

    glTranslatef(-MAP_SIZE / 2, -75, -MAP_SIZE / 2);

    /* Drawing process */

    for (treeLoop = 0; treeLoop < MAX_TREE_COUNT; treeLoop++)
    {

        /* Duplicates the current matrix */

        glPushMatrix();;

        /* Set the position of the tree we're currently rendering */

        glTranslatef(xPosTree[treeLoop], yPosTree[treeLoop], zPosTree[treeLoop]);

        /* Bind our currently used texture */

        glBindTexture(GL_TEXTURE_2D, whichTreeTexture[chooseTree[treeLoop]]);

        /* Vertex arrays */

        glVertexPointer(3, GL_FLOAT, 0, whichVAVertex[chooseTree[treeLoop]]);
        glTexCoordPointer(2, GL_FLOAT, 0, whichVATexture[chooseTree[treeLoop]]);
        glNormalPointer(GL_FLOAT, 0, whichVANormal[chooseTree[treeLoop]]);

        glDrawArrays(GL_TRIANGLES, 0, whichVAFaceCount[chooseTree[treeLoop]]);

        /* Replaces the current matrix with the one below it on the stack */

        glPopMatrix();
    }

    /* Replaces the current matrix with the one below it on the stack */

    glPopMatrix();
}






void sView::drawWater()
{

    /* Duplicates the current matrix */

    glPushMatrix();

    /* Lake creation starts here */

    glTranslatef(460, -65, -35);

    /* Color the water blue or red */

    if (wireOptions == false)
    {
        if (waterColor == true)
        {
            glColor3f(0.0, 0.4, 0.6);
        }

        if (waterColor == false)
        {
            glColor3f(0.9, 0.1, 0.0);
        }
    }
    else if (wireOptions == true)
    {
        /* Do nothing */
    }

    /* Bind our currently used textures (multi-texture) */

    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureIDWater1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_textureIDWater2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTranslatef(-waterX, waterY, 0);
    if (wireOptions == false)
    {
        glEnable(GL_TEXTURE_2D);
    }

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_textureIDWater3);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTranslatef(-waterX, -waterY, 0);
    if (wireOptions == false)
    {
        glEnable(GL_TEXTURE_2D);
    }

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, m_textureIDWater4);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTranslatef(waterX, -waterY, 0);
    if (wireOptions == false)
    {
        glEnable(GL_TEXTURE_2D);
    }

    glMatrixMode(GL_MODELVIEW);

    /* Drawing process */

    glVertexPointer(3, GL_FLOAT, 0, m_WaterVertex);

    glClientActiveTexture(GL_TEXTURE0);
    glTexCoordPointer(2, GL_FLOAT, 0, m_WaterTextureCoord);

    glClientActiveTexture(GL_TEXTURE1);
    glTexCoordPointer(2, GL_FLOAT, 0, m_WaterTextureCoord);
    if (wireOptions == false)
    {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    glClientActiveTexture(GL_TEXTURE2);
    glTexCoordPointer(2, GL_FLOAT, 0, m_WaterTextureCoord);
    if (wireOptions == false)
    {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    glClientActiveTexture(GL_TEXTURE3);
    glTexCoordPointer(2, GL_FLOAT, 0, m_WaterTextureCoord);
    if (wireOptions == false)
    {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    glDrawArrays(GL_QUADS, 0, 4);

    /* Reset color to white */

    if (wireOptions == false)
    {
        glColor3f(1.0, 1.0, 1.0);
    }
    else if (wireOptions == true)
    {
        /* Do nothing */
    }

    /* Replaces the current matrix with the one below it on the stack */

    glPopMatrix();

    /* Disable the texture units */

    glClientActiveTexture(GL_TEXTURE1);
    if (wireOptions == false)
    {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    glClientActiveTexture(GL_TEXTURE2);
    if (wireOptions == false)
    {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    glClientActiveTexture(GL_TEXTURE3);
    if (wireOptions == false)
    {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    glActiveTexture(GL_TEXTURE1);
    if (wireOptions == false)
    {
        glDisable(GL_TEXTURE_2D);
    }

    glActiveTexture(GL_TEXTURE2);
    if (wireOptions == false)
    {
        glDisable(GL_TEXTURE_2D);
    }

    glActiveTexture(GL_TEXTURE3);
    if (wireOptions == false)
    {
        glDisable(GL_TEXTURE_2D);
    }

    /* Switch back to the default texture unit */

    glActiveTexture(GL_TEXTURE0);
    glClientActiveTexture(GL_TEXTURE0);
}






void sView::drawSpacestation()
{

    /* Duplicates the current matrix */

    glPushMatrix();

    /* Bind our currently used texture */

    glBindTexture(GL_TEXTURE_2D, m_textureIDSpacestation);

    /* We set the position of the first spacestation */

    glTranslatef(-6.0, 1.0, 3.0);

    /* Spacestation rotation */

    glRotatef(spacestationRotateY, 0, 1, 0);
    glRotatef(270, 1, 0, 0);

    /* Drawing process */

    glVertexPointer(3, GL_FLOAT, 0, m_objectSpacestation.m_pVertex);
    glTexCoordPointer(2, GL_FLOAT, 0, m_objectSpacestation.m_pTextureCoord);
    glNormalPointer(GL_FLOAT, 0, m_objectSpacestation.m_pNormal);

    /* Triple our object */

    for (objectCount = 0; objectCount < 3; objectCount++)
    {
        glTranslatef(3.0, 0.0, 0.0);
        glDrawArrays(GL_TRIANGLES, 0, m_objectSpacestation.m_faceCount);
    }

    /* Replaces the current matrix with the one below it on the stack */

    glPopMatrix();
}






void sView::drawSpaceship()
{

    /* Duplicates the current matrix */

    glPushMatrix();

    /* Bind our currently used texture */

    glBindTexture(GL_TEXTURE_2D, m_textureIDSpaceship);

    /* We set the position of the spaceship */

    glTranslatef(-5.0, 4.0, -7.5);

    /* Spaceship rotation */

    glRotatef(270, 1, 0, 0);
    glRotatef(90, 0, 0, 1);

    /* Drawing process */

    glVertexPointer(3, GL_FLOAT, 0, m_objectSpaceship.m_pVertex);
    glTexCoordPointer(2, GL_FLOAT, 0, m_objectSpaceship.m_pTextureCoord);
    glNormalPointer(GL_FLOAT, 0, m_objectSpaceship.m_pNormal);

    glDrawArrays(GL_TRIANGLES, 0, m_objectSpaceship.m_faceCount);

    /* Replaces the current matrix with the one below it on the stack */

    glPopMatrix();
}






void sView::light()
{
    POSITION_DIFFUSE[0] = LIGHT.m_positon.m_x;
    POSITION_DIFFUSE[1] = LIGHT.m_positon.m_y;
    POSITION_DIFFUSE[2] = LIGHT.m_positon.m_z;
    POSITION_DIFFUSE[3] = 1;
    glLightfv(GL_LIGHT0, GL_POSITION, POSITION_DIFFUSE);

    DIFFUSE[0] = LIGHT.m_red;
    DIFFUSE[1] = LIGHT.m_green;
    DIFFUSE[2] = LIGHT.m_blue;
    DIFFUSE[3] = 1;
    glLightfv(GL_LIGHT0, GL_DIFFUSE, DIFFUSE);

    POSITION_AMBIENT[0] = 0;
    POSITION_AMBIENT[1] = 0;
    POSITION_AMBIENT[2] = 0;
    POSITION_AMBIENT[3] = 1;
    glLightfv(GL_LIGHT1, GL_POSITION, POSITION_AMBIENT);

    AMBIENT[0] = 0.25;
    AMBIENT[1] = 0.25;
    AMBIENT[2] = 0.25;
    AMBIENT[3] = 1;
    glLightfv(GL_LIGHT1, GL_AMBIENT, AMBIENT);
}






/* Calculates the projected planar shadow matrix based on values from the plane equation and our light's position */

/* Explanations:
http://www.devmaster.net/articles/shadow_techniques/
http://www.songho.ca/opengl/gl_projectionmatrix.html */

void sView::shadowCalc(float * shadowMatrix, float * lightPosition, float * surface)
{
    float dotProduct;

    dotProduct =    surface[0] * lightPosition[0] +
                    surface[1] * lightPosition[1] +
                    surface[2] * lightPosition[2] +
                    surface[3] * lightPosition[3];

    shadowMatrix[0] = dotProduct - lightPosition[0] * surface[0];
    shadowMatrix[4] = 0.0f - lightPosition[0] * surface[1];
    shadowMatrix[8] = 0.0f - lightPosition[0] * surface[2];
    shadowMatrix[12] = 0.0f - lightPosition[0] * surface[3];

    shadowMatrix[1] = 0.0f - lightPosition[1] * surface[0];
    shadowMatrix[5] = dotProduct - lightPosition[1] * surface[1];
    shadowMatrix[9] = 0.0f - lightPosition[1] * surface[2];
    shadowMatrix[13] = 0.0f - lightPosition[1] * surface[3];

    shadowMatrix[2] = 0.0f - lightPosition[2] * surface[0];
    shadowMatrix[6] = 0.0f - lightPosition[2] * surface[1];
    shadowMatrix[10] = dotProduct - lightPosition[2] * surface[2];
    shadowMatrix[14] = 0.0f - lightPosition[2] * surface[3];

    shadowMatrix[3] = 0.0f - lightPosition[3] * surface[0];
    shadowMatrix[7] = 0.0f - lightPosition[3] * surface[1];
    shadowMatrix[11] = 0.0f - lightPosition[3] * surface[2];
    shadowMatrix[15] = dotProduct - lightPosition[3] * surface[3];
}






void sView::shadowDraw()
{

    /* Ground normals are declared */

    float groundPlane[4] = {0, 1, 0, 3};

    /* Call to the shadow volume function */

    shadowCalc(shadow, POSITION_DIFFUSE, groundPlane);

    /* Turn off writing to color- and depthBuffer */

    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);

    /* Enable stencil testing */

    glEnable(GL_STENCIL_TEST);

    /* Tell the stencil function to set its value to 1 everywhere something is drawn */

    glStencilFunc(GL_ALWAYS, 1, 0xFFFFFFFF);
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);

    /* Draw our ground into the stencilBuffer */

    drawGround();

    /* Enable color- and depthBuffer again */

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);

    /* Tell the stencil function to only draw at a position where the buffer's value equals 1 */

    glStencilFunc(GL_EQUAL, 1, 0xFFFFFFFF);

    /* Saves the stencil data */

    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    /* Draw our ground into the color- and depthBuffer */

    drawGround();

    /* Duplicates the current matrix */

    glPushMatrix();

    /* Sets color to black with an alpha value of 0.5 */

    if (wireOptions == false)
    {
        glColor4f(0.0, 0.0, 0.0, 0.5f);
    }
    else if (wireOptions == true)
    {
        /* Do nothing */
    }

    /* Enable as well as disable some OpenGL specific options */

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);

    /* Specifies how we want GL_BLEND to work */

    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* As soon as something is drawn increase the stencil value in order to avoid double blending */

    glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);

    /* Multiply the current matrix with our shadow matrix */

    glMultMatrixf(shadow);

    /* Draw the spacestation shadow */

    drawSpacestation();

    /* Draw the spaceship shadow */

    drawSpaceship();

    /* Enable as well as disable some OpenGL specific options */

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    /* Reset color to white */

    if (wireOptions == false)
    {
        glColor3f(1.0, 1.0, 1.0);
    }
    else if (wireOptions == true)
    {
        /* Do nothing */
    }

    /* Replaces the current matrix with the one below it on the stack */

    glPopMatrix();

    /* Disable stencil testing */

    glDisable(GL_STENCIL_TEST);
}






void sView::particleSetting()
{
    m_fireStreamLeft.xPosition = -1.85;
    m_fireStreamLeft.yPosition = 4.25;
    m_fireStreamLeft.zPosition = -6.65;
    m_fireStreamLeft.xGravity = 0;
    m_fireStreamLeft.yGravity = 0;
    m_fireStreamLeft.zGravity = 0;
    m_fireStreamLeft.r = 0;
    m_fireStreamLeft.g = 0;
    m_fireStreamLeft.b = 1;
    m_fireStreamLeft.colorChangeR = 0.10;
    m_fireStreamLeft.colorChangeG = 0.05;
    m_fireStreamLeft.colorChangeB = 0;
    m_fireStreamLeft.reduce = 0.001;

    m_fireStreamRight.xPosition = -1.85;
    m_fireStreamRight.yPosition = 4.25;
    m_fireStreamRight.zPosition = -8.35;
    m_fireStreamRight.xGravity = 0;
    m_fireStreamRight.yGravity = 0;
    m_fireStreamRight.zGravity = 0;
    m_fireStreamRight.r = 0;
    m_fireStreamRight.g = 0;
    m_fireStreamRight.b = 1;
    m_fireStreamRight.colorChangeR = 0.10;
    m_fireStreamRight.colorChangeG = 0.05;
    m_fireStreamRight.colorChangeB = 0;
    m_fireStreamRight.reduce = 0.001;

    PARTICLE.dynamicSize();

    m_fireStreamLeft.xSpeed = new float [streamCount];
    m_fireStreamLeft.ySpeed = new float [streamCount];
    m_fireStreamLeft.zSpeed = new float [streamCount];
    m_fireStreamLeft.life = new float [streamCount];
    m_fireStreamLeft.lifePersist = new float [streamCount];

    m_fireStreamRight.xSpeed = new float [streamCount];
    m_fireStreamRight.ySpeed = new float [streamCount];
    m_fireStreamRight.zSpeed = new float [streamCount];
    m_fireStreamRight.life = new float [streamCount];
    m_fireStreamRight.lifePersist = new float [streamCount];

    for (polygonLoop = 0; polygonLoop < streamCount; polygonLoop++)
    {
        PARTICLE.particleListLeft.info[polygonLoop].newParticles = true;
        PARTICLE.particleListRight.info[polygonLoop].newParticles = true;
    }
}






void sView::particleMovement()
{
    for (polygonLoop = 0; polygonLoop < streamCount; polygonLoop++)
    {
        m_fireStreamLeft.xSpeed[polygonLoop] = rand()%150;
        m_fireStreamLeft.ySpeed[polygonLoop] = rand()%30 - float(15);
        m_fireStreamLeft.zSpeed[polygonLoop] = rand()%30 - float(15);
        m_fireStreamLeft.life[polygonLoop] = rand()%50000 / float(500000);
        m_fireStreamLeft.lifePersist[polygonLoop] = rand()%50000 / float(500000);

        m_fireStreamRight.xSpeed[polygonLoop] = rand()%150;
        m_fireStreamRight.ySpeed[polygonLoop] = rand()%30 - float(15);
        m_fireStreamRight.zSpeed[polygonLoop] = rand()%30 - float(15);
        m_fireStreamRight.life[polygonLoop] = rand()%50000 / float(500000);
        m_fireStreamRight.lifePersist[polygonLoop] = rand()%50000 / float(500000);
    }
}






int sView::heightCalc(float * heightMap, int X, int Z)
{

    /* Height values are indexed: X = 1, Z = 0 -> heightMap[1] | X = 0, Z = 1 -> heightMap[INT_MAP_SIZE]; INT_MAP_SIZE is currently 1024 */

    return heightMap[X + (Z * INT_MAP_SIZE)] * HEIGHT_RATIO;
}






void sView::setupTerrain(float * heightMap)
{

    /* Usage of vector class to create dynamic terrain information */

    for (int X = 0; X < (MAP_SIZE - STEP_SIZE); X += STEP_SIZE)
    {
        for (int Z = 0; Z < (MAP_SIZE - STEP_SIZE); Z += STEP_SIZE)
        {

            /* QUADS based on STEP_SIZE (polygon length) and height values; Texture is cut into small pieces */

            coordX = X / MAP_SIZE;
            coordZ = Z / MAP_SIZE;
            coordAddX = (X + STEP_SIZE) / MAP_SIZE;
            coordAddZ = (Z + STEP_SIZE) / MAP_SIZE;

            x = X;
            y = heightCalc(heightMap, X, Z);
            z = Z;

            /* Pushes the calculated data into our array */

            terrainTextureCoord.push_back(coordX);
            terrainTextureCoord.push_back(coordZ);

            terrainVertex.push_back(x);
            terrainVertex.push_back(y);
            terrainVertex.push_back(z);

            x = X;
            y = heightCalc(heightMap, X, Z + STEP_SIZE);
            z = Z + STEP_SIZE ;

            /* Pushes the calculated data into our array */

            terrainTextureCoord.push_back(coordX);
            terrainTextureCoord.push_back(coordAddZ);

            terrainVertex.push_back(x);
            terrainVertex.push_back(y);
            terrainVertex.push_back(z);

            x = X + STEP_SIZE;
            y = heightCalc(heightMap, X + STEP_SIZE, Z + STEP_SIZE);
            z = Z + STEP_SIZE ;

            /* Pushes the calculated data into our array */

            terrainTextureCoord.push_back(coordAddX);
            terrainTextureCoord.push_back(coordAddZ);

            terrainVertex.push_back(x);
            terrainVertex.push_back(y);
            terrainVertex.push_back(z);

            x = X + STEP_SIZE;
            y = heightCalc(heightMap, X + STEP_SIZE, Z);
            z = Z;

            /* Pushes the calculated data into our array */

            terrainTextureCoord.push_back(coordAddX);
            terrainTextureCoord.push_back(coordZ);

            terrainVertex.push_back(x);
            terrainVertex.push_back(y);
            terrainVertex.push_back(z);
        }
    }
}






void sView::plantVegetation(float * heightMap)
{

    /* Initializes our random function */

    srand(time(0));

    /* Fills our arrays with 2 different kinds of trees */

    whichTreeTexture[0] = m_textureIDTreeSmall;
    whichTreeTexture[1] = m_textureIDTreeLarge;

    whichVAVertex[0] = m_objectTreeSmall.m_pVertex;
    whichVAVertex[1] = m_objectTreeLarge.m_pVertex;

    whichVATexture[0] = m_objectTreeSmall.m_pTextureCoord;
    whichVATexture[1] = m_objectTreeLarge.m_pTextureCoord;

    whichVANormal[0] = m_objectTreeSmall.m_pNormal;
    whichVANormal[1] = m_objectTreeLarge.m_pNormal;

    whichVAFaceCount[0] = m_objectTreeSmall.m_faceCount;
    whichVAFaceCount[1] = m_objectTreeLarge.m_faceCount;

    /* Calculate coordinates of the trees based on our heightMap and a random value - creation of a grid */

    for (treeGridX = 0; treeGridX < sqrtf(MAX_TREE_COUNT); treeGridX++)
    {
        for (treeGridZ = 0; treeGridZ < sqrtf(MAX_TREE_COUNT); treeGridZ++)
        {
            xPosTree[treeLoop] = rand()%6 + 320 + treeGridX * 9;
            zPosTree[treeLoop] = rand()%6 + 320 + treeGridZ * 9;
            yPosTree[treeLoop] = heightCalc(heightMap, xPosTree[treeLoop], zPosTree[treeLoop]);
            chooseTree[treeLoop] = rand()%2;

            treeLoop++;
        }
    }
}






void sView::interfaceWindows()
{

    /* Split value into digits */

    tempContainer = TIME.saveFPS;

    splitValue[2] = tempContainer % 10;
    tempContainer /= 10;
    splitValue[1] = tempContainer % 10;
    tempContainer /= 10;
    splitValue[0] = tempContainer % 10;

    /* Duplicates the current matrix */

    glPushMatrix();

    /* We attach the interface to the camera */

    glTranslatef(CAMERA.m_vPosition.m_x, CAMERA.m_vPosition.m_y, CAMERA.m_vPosition.m_z);

    /* Rotate the interface with the camera */

    glRotatef(CAMERA.m_Angle.m_y, 0, 1, 0);
    glRotatef(CAMERA.m_Angle.m_x, 1, 0, 0);

    /* Bind our currently used texture */

    glBindTexture(GL_TEXTURE_2D, m_textureIDNumber);

    /* First element */

    glPushMatrix();

    /* Position on screen correctly */

    glTranslatef(6.0, 8.0, -8.0);

    /* Rotate correctly */

    glRotatef(180, 0, 1, 0);
    glRotatef(270, 0, 0, 1);

    /* Draw */

    glVertexPointer(3, GL_FLOAT, 0, m_InterfaceVertex);
    glTexCoordPointer(2, GL_FLOAT, 0, &m_InterfaceTextureCoord[splitValue[0]]);

    glDrawArrays(GL_QUADS, 0, 4);

    glPopMatrix();

    /* Second element */

    glPushMatrix();

    /* Position on screen correctly */

    glTranslatef(7.0, 8.0, -8.0);

    /* Rotate correctly */

    glRotatef(180, 0, 1, 0);
    glRotatef(270, 0, 0, 1);

    /* Draw */

    glVertexPointer(3, GL_FLOAT, 0, m_InterfaceVertex);
    glTexCoordPointer(2, GL_FLOAT, 0, &m_InterfaceTextureCoord[splitValue[1]]);

    glDrawArrays(GL_QUADS, 0, 4);

    glPopMatrix();

    /* Third element */

    glPushMatrix();

    /* Position on screen correctly */

    glTranslatef(8.0, 8.0, -8.0);

    /* Rotate correctly */

    glRotatef(180, 0, 1, 0);
    glRotatef(270, 0, 0, 1);

    /* Draw */

    glVertexPointer(3, GL_FLOAT, 0, m_InterfaceVertex);
    glTexCoordPointer(2, GL_FLOAT, 0, &m_InterfaceTextureCoord[splitValue[2]]);

    glDrawArrays(GL_QUADS, 0, 4);

    glPopMatrix();

    /* Replaces the current matrix with the one below it on the stack */

    glPopMatrix();
}
