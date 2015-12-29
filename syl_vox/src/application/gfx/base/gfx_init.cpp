#include <gl/glew.h>
#include <gl/freeglut.h>

#include "gfx_init.h"
#include "../../../utility/log.h"

bool GfxInit::GLEW_INIT_DONE = false;
bool GfxInit::GLUT_INIT_DONE = false;

bool GfxInit::api()
{
	Log<GfxInit> log;

	if(!GLEW_INIT_DONE)
	{
		glewExperimental = TRUE;
		GLenum err = glewInit();
		if(GLEW_OK != err)
		{
			log.Out(LogType::LOG_ERR, "<GLEW> "+std::string((const char*)glewGetErrorString(err)));
			return false;
		}
	
		log.Out(LogType::LOG_MSG, "Current GLEW version: "+std::string((const char*)glewGetString(GLEW_VERSION)));

		GLEW_INIT_DONE = true;
	}
	else
		log.Out(LogType::LOG_MSG, "GLEW already initialized");

	return true;
}

bool GfxInit::api_init_done()
{
	return GLEW_INIT_DONE;
}

void GfxInit::glut()
{
	if (!GLUT_INIT_DONE)
	{
		int argcp = 1;
		char* argv[] = {"", NULL};
	
		glutInit(&argcp, argv);

		GLUT_INIT_DONE = true;
	}
}

void GfxInit::settings()
{
	glEnable(GL_DEPTH_TEST); //Applied to currently active OpenGL context
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_MULTISAMPLE);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_INDEX_ARRAY);
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}