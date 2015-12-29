#ifndef GFX_INIT_H
#define GFX_INIT_H

class GfxInit
{
public:
	bool api();
	bool api_init_done();
	void glut();
	void settings();

private:
	static bool GLEW_INIT_DONE;
	static bool GLUT_INIT_DONE;
};

#endif