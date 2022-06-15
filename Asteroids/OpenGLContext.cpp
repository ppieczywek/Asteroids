#pragma once
#include "stdafx.h"
#include "OpenGLContext.h"

GLYPHMETRICSFLOAT	gmf[256];
unsigned int		TextListBase;

void setVSync(bool sync){

	typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALPROC)(int);
	PFNWGLSWAPINTERVALPROC wglSwapIntervalEXT = 0;

	const char *extensions = (char*)glGetString(GL_EXTENSIONS);

	if( strstr( extensions, "WGL_EXT_swap_control" ) == 0 ) {
		return;
	} else {
		wglSwapIntervalEXT = (PFNWGLSWAPINTERVALPROC)wglGetProcAddress( "wglSwapIntervalEXT" );
		if(wglSwapIntervalEXT) wglSwapIntervalEXT(sync);
	}
};

OpenGLContext::OpenGLContext() : hrc(NULL), hdc(NULL) {};

OpenGLContext::OpenGLContext(HWND hwnd) {
	create30Context(hwnd); 
}

BOOL OpenGLContext::Create(HWND hwnd)
{
	hdc = GetDC(hwnd);
	if( SetupPixelFormat(hdc) == FALSE ) {
		MessageBox(NULL, TEXT("SetupPixelFormat Failed."), TEXT("OpenGL Message"), MB_OK | MB_ICONERROR);
		return FALSE;
	};

	hrc = wglCreateContext(hdc);
	if ( wglMakeCurrent(hdc, hrc) == FALSE ) {	
		MessageBox(NULL, TEXT("MakeCurrent Failed."), TEXT("OpenGL Message"), MB_OK | MB_ICONERROR);
		return FALSE;
	};

	RECT rect;
	if(GetWindowRect(hwnd, &rect) == FALSE) {
		MessageBox(NULL, TEXT("GetWindowRect Failed."), TEXT("OpenGL Message"), MB_OK | MB_ICONERROR);
		return FALSE;
	} else {
		
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;
		glViewport( 0 , 0 , width , height );
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		
		float aspect = (float)width / (float)height;
		glOrtho(-aspect, aspect, -1, 1, -50.0f, 50.0f);

		//glOrtho(0.0f, 150.0f, 0.0f, 112.5f, -50.0f, 50.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
				
		glEnable( GL_BLEND );
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		glEnable(GL_DEPTH_TEST);
		glShadeModel(GL_SMOOTH);
		
		setVSync(1);
		TextListBase = CreateBitmapfont(0, hwnd); 
		SetFocus(hwnd);
	}; 
	return TRUE;
};


bool OpenGLContext::create30Context(HWND hwnd) 
{
	this->hwnd = hwnd;
	hdc = GetDC(hwnd); 

	PIXELFORMATDESCRIPTOR pfd; 
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR)); 
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR); 
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA; 
	pfd.cColorBits = 32; 
	pfd.cDepthBits = 32; 
	pfd.iLayerType = PFD_MAIN_PLANE; 

	int nPixelFormat = ChoosePixelFormat(hdc, &pfd);
	if (nPixelFormat == 0) return false;

	bool bResult = SetPixelFormat(hdc, nPixelFormat, &pfd); 
	if (!bResult) return false;

	HGLRC tempOpenGLContext = wglCreateContext(hdc);
	wglMakeCurrent(hdc, tempOpenGLContext);

	GLenum error = glewInit(); 
	if (error != GLEW_OK) return false;

	int attributes[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3, 
		WGL_CONTEXT_MINOR_VERSION_ARB, 2, 
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB, 
		0
	};

	if (wglewIsSupported("WGL_ARB_create_context") == 1) { 
		hrc = wglCreateContextAttribsARB(hdc, NULL, attributes);
		wglMakeCurrent(NULL, NULL); 
		wglDeleteContext(tempOpenGLContext); 
		wglMakeCurrent(hdc, hrc); 
	}
	else {
		hrc = tempOpenGLContext; 
	};

	int glVersion[2] = {-1, -1}; 
	glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]); 
	glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]); 

	return true; 
}


OpenGLContext::~OpenGLContext() 
{
	ClearBitmapfont();
	wglMakeCurrent(hdc, NULL);
	wglDeleteContext(hrc);
	ReleaseDC(hwnd, hdc);
};




BOOL OpenGLContext::SetupPixelFormat(HDC hdc)
{
    PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    
        PFD_TYPE_RGBA,            
        32,                        
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        32,                        
        0,                        
        0,                       
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };

	int  iPixelFormat; 
	iPixelFormat = ChoosePixelFormat(hdc, &pfd);	
	if ( SetPixelFormat(hdc, iPixelFormat, &pfd) == FALSE ) {
		return FALSE;
	} else {
		return TRUE;
	};
};



unsigned int CreateBitmapfont(int fontSize, HWND hwnd) {

	HFONT hFont;
	unsigned int base;

	base = glGenLists(256);

	
	hFont = CreateFont(fontSize,0,0,0,FW_BOLD,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,
                CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY, VARIABLE_PITCH,TEXT("Arial"));

	if (!hFont) return 0;
	
	auto hdc = GetDC(hwnd);

	SelectObject(hdc, hFont);
	wglUseFontOutlines( hdc, 0, 255, base, 0.0f, 0.2f, WGL_FONT_POLYGONS, gmf);   

	return base;
};

void PrintString(const char *str) {

	if((TextListBase == 0) || (str == NULL))
		return;

	glPushAttrib(GL_LIST_BIT);
		glListBase(TextListBase);
		glCallLists(strlen(str), GL_UNSIGNED_BYTE, str);
	glPopAttrib();
};

void ClearBitmapfont()
{
	glDeleteLists(TextListBase, 256); 
	//glDeleteLists(base, 256);	
	//DeleteObject(hFont);

};

float GetStringLength(const char *str)
{
	float Length = 0.0f;
	for(unsigned loop = 0; loop< (strlen(str)); loop++)
	{
		Length += gmf[str[loop]].gmfCellIncX;
	}
	return Length;
};