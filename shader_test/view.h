#ifndef _VIEW
#define _VIEW

#include <windows.h>

#include "loader.h"

class cView
{
    private:

    int texture;
    int texture_ground;
    int previousTexture;
    GLuint shader;
    GLuint shader_ground;
    GLuint previousShader;

    float rotation;

    GLfloat position_light[4];
    GLfloat ambient_light[4];

    void object();
    void ground();
    void useShader(GLuint program);

    public:

    bool wireframe;

    void init();
    void draw();
    void changeOutput(int type, OPENFILENAME input);
    void disable(int type);
    void scaleOBJ(float value);
};

#endif
