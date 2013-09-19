#include "view.h"

cLoader file;

void cView::init()
{
    wireframe = false;
    file.initGlewLib();

    glClearColor(0.0, 0.0, 0.0, 0.0);

    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    position_light[0] = 0.0;
    position_light[1] = 0.0;
    position_light[2] = -3.5;
    position_light[3] = 1.0;

    ambient_light[0] = 0.5;
    ambient_light[1] = 0.5;
    ambient_light[2] = 0.5;
    ambient_light[3] = 1.0;

    glLightfv(GL_LIGHT0, GL_POSITION, position_light);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    rotation = 0;

    file.loadOBJ((char*)"spaceship.obj");
    texture = file.loadPNGLib((char*)"spaceship.png");
    shader = file.loadShader((char*)"light.vert", (char*)"light.frag");
    texture_ground = file.loadPNGLib((char*)"ground.png");
    shader_ground = file.loadShader((char*)"light.vert", (char*)"light.frag");

    scaleOBJ(0.01);
}

void cView::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glLoadIdentity();

    glPushMatrix();
        useShader(shader);
        object();
    glPopMatrix();

    glPushMatrix();
        useShader(shader_ground);
        ground();
    glPopMatrix();
}

void cView::useShader(GLuint program)
{
    glUseProgram(program);
}

void cView::object()
{
    glBindTexture(GL_TEXTURE_2D, texture);

    glTranslatef(0.0, -0.5, -5.5);

    glRotatef(-90.0, 1.0, 0.0, 0.0);
    glRotatef(180.0, 0.0, 0.0, 1.0);

    glRotatef(rotation, 0.0, 0.0, 1.0);
    rotation += 0.015;

    /* --- DRAW --- */

    glVertexPointer(3, GL_FLOAT, 0, file.model.vertex);
    glTexCoordPointer(2, GL_FLOAT, 0, file.model.texture);
    glNormalPointer(GL_FLOAT, 0, file.model.normal);

    glDrawArrays(GL_TRIANGLES, 0, file.model.faceCount);

    /* --- DRAW --- */
}

void cView::ground()
{
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    glBindTexture(GL_TEXTURE_2D, texture_ground);

    glTranslatef(0.0, -15.0, -15.0);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-10.0f,  10.0f, -10.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0f,  10.0f,  10.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 10.0f,  10.0f,  10.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 10.0f,  10.0f, -10.0f);
    glEnd();

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
}

void cView::changeOutput(int type, OPENFILENAME input)
{
    switch(type)
    {
        case 1:
            file.loadOBJ((char*)input.lpstrFile);
            scaleOBJ(0.01);
            break;
        case 2:
            previousTexture = texture;
            texture = file.loadPNGLib((char*)input.lpstrFile);

            if(texture == -1)
            {
                texture = previousTexture;
            }
            break;
        case 3:
            std::string temp = input.lpstrFile;
            temp = temp.substr(0, temp.size() - 4);
            std::string vert = temp+"vert";
            std::string frag = temp+"frag";

            previousShader = shader;
            shader = file.loadShader((char*)&vert[0], (char*)&frag[0]);

            if(shader == 0)
            {
                shader = previousShader;
            }
            break;
    }
}

void cView::disable(int type)
{
    switch(type)
    {
        case 1:
            texture = 0;
            break;
        case 2:
            shader = 0;
            break;
    }
}

void cView::scaleOBJ(float value)
{
    for(int i = 0; i < file.model.faceCount * 3; i++)
    {
        file.model.vertex[i] = file.model.const_vertex[i] * value;
    }
}
