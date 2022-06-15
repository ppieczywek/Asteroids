#pragma once
#define GLEW_STATIC
#include <windows.h>
#include <GL/glew.h>
#include <GL/wglew.h>
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")


class OpenGLContext 
{  
public:  
OpenGLContext(void); 
				
				OpenGLContext(HWND hwnd); 
				~OpenGLContext(void); 
	bool		create30Context(HWND hwnd); 
	BOOL		Create(HWND hwnd);
	HDC			GetHDC() {return hdc;};
	
private:  
	BOOL		 SetupPixelFormat(HDC hdc);
	HGLRC		 hrc;   
	HDC			 hdc; 
	HWND		 hwnd;

};  

void setVSync(bool sync);


void			PrintString(const char *str);
unsigned int	CreateBitmapfont(int fontSize, HWND hwnd);
float			GetStringLength(const char *str);
void			ClearBitmapfont();