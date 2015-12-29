#ifndef SCHEME_FUNCTION_H
#define SCHEME_FUNCTION_H

class RawInputHandler;
class Camera;

class SchemeFunction
{
public:
	SchemeFunction();

	void setRawInputHandler(RawInputHandler* _rawInputHandler);
	void unsetRawInputHandler();
	void setCamera(Camera* _camera);
	void unsetCamera();

	void rotateCamera(float _sensitivity);

private:
	RawInputHandler* m_rawInputHandler;
	Camera* m_camera;
};

#endif