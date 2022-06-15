#pragma once
#include <gl/GL.h>
#include <gl/GLU.h>
//#include <gl/GLAux.h>
#include <cmath>

//#pragma comment(lib, "glew32.lib") 
#pragma comment(lib, "glu32.lib") 
#pragma comment(lib, "opengl32.lib") 
//#pragma comment(lib, "Glaux.lib") 

class StereoCamera
{
public:
    StereoCamera(  
        float Convergence,
        float EyeSeparation,
        float AspectRatio,
        float FOV,
        float NearClippingDistance,
        float FarClippingDistance
        )
    {
        mConvergence            = Convergence;
        mEyeSeparation          = EyeSeparation;
        mAspectRatio            = AspectRatio;
        mFOV                    = FOV * 3.1415 / 180.0f;
        mNearClippingDistance   = NearClippingDistance;
        mFarClippingDistance    = FarClippingDistance;

    }

    void ApplyLeftFrustum()
    {
        float top, bottom, left, right;
        top     = mNearClippingDistance * tan(mFOV/2);
        bottom  = -top;
        float a = mAspectRatio * tan(mFOV/2) * mConvergence;
        float b = a - mEyeSeparation/2;
        float c = a + mEyeSeparation/2;
        left    = -b * mNearClippingDistance/mConvergence;
        right   =  c * mNearClippingDistance/mConvergence;
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();  
        glFrustum(left, right, bottom, top,
        mNearClippingDistance, mFarClippingDistance);
        glMatrixMode(GL_MODELVIEW);                    
        glLoadIdentity();  
        glTranslatef(mEyeSeparation/2, 0.0f, 0.0f);
    }

    void ApplyRightFrustum()
    {
        float top, bottom, left, right;
        top     = mNearClippingDistance * tan(mFOV/2);
        bottom  = -top;
        float a = mAspectRatio * tan(mFOV/2) * mConvergence;
        float b = a - mEyeSeparation/2;
        float c = a + mEyeSeparation/2;
        left    =  -c * mNearClippingDistance/mConvergence;
        right   =   b * mNearClippingDistance/mConvergence;
        glMatrixMode(GL_PROJECTION);                       
        glLoadIdentity();  
        glFrustum(left, right, bottom, top, mNearClippingDistance, mFarClippingDistance);
        glMatrixMode(GL_MODELVIEW);                    
		glLoadIdentity();  
        glTranslatef(-mEyeSeparation/2, 0.0f, 0.0f);
    }

private:
    float mConvergence;
    float mEyeSeparation;
    float mAspectRatio;
    float mFOV;
    float mNearClippingDistance;
    float mFarClippingDistance;
};

void DrawCube()
{glPushMatrix();

  glBegin(GL_TRIANGLES);				//front
	glNormal3f(0.0f, 0.0f, 1.0f );
	
	glVertex3f( 150.0f, -150.0f, -150.0f );
	glVertex3f(-150.0f, -150.0f, -150.0f );
	glVertex3f( 150.0f,  150.0f, -150.0f );
	glVertex3f(-150.0f,  150.0f, -150.0f );
	glVertex3f( 150.0f,  150.0f, -150.0f );
	glVertex3f(-150.0f, -150.0f, -150.0f );
 glEnd();
 
 
 glBegin(GL_TRIANGLES);				//back
	glNormal3f(0.0f, 0.0f, -1.0f );
	glVertex3f(-150.0f, -150.0f, 150.0f );
	glVertex3f( 150.0f, -150.0f, 150.0f );
	glVertex3f( 150.0f,  150.0f, 150.0f );
	glVertex3f( 150.0f,  150.0f, 150.0f );
	glVertex3f(-150.0f,  150.0f, 150.0f );
	glVertex3f(-150.0f, -150.0f, 150.0f );
 glEnd();
			

 glBegin(GL_TRIANGLES);				//top
 
	glNormal3f(0.0f, -1.0f, 0.0f );
	glVertex3f(-150.0f,  150.0f, 150.0f );
	glVertex3f( 150.0f,  150.0f, 150.0f );
	glVertex3f( 150.0f,  150.0f,-150.0f );
	glVertex3f( 150.0f,  150.0f,-150.0f );
	glVertex3f(-150.0f,  150.0f,-150.0f );
	glVertex3f(-150.0f,  150.0f, 150.0f );
 glEnd();



  glBegin(GL_TRIANGLES);				//bottom
  
	glNormal3f(0.0f,  1.0f, 0.0f );
	glVertex3f( 150.0f, -150.0f, 150.0f );
	glVertex3f(-150.0f, -150.0f, 150.0f );
	glVertex3f( 150.0f, -150.0f,-150.0f );
	glVertex3f(-150.0f, -150.0f,-150.0f );
	glVertex3f( 150.0f, -150.0f,-150.0f );
	glVertex3f(-150.0f, -150.0f, 150.0f );
 glEnd();

 glBegin(GL_TRIANGLES);				//left
	glNormal3f(1.0f,  0.0f, 0.0f );
	glVertex3f(-150.0f, -150.0f, 150.0f );
	
	glVertex3f(-150.0f,  150.0f, 150.0f );
	glVertex3f(-150.0f, -150.0f,-150.0f );


	
	glVertex3f(-150.0f,  150.0f,-150.0f );
	glVertex3f(-150.0f, -150.0f,-150.0f );
	glVertex3f(-150.0f,  150.0f, 150.0f );
 glEnd();


  
 glBegin(GL_TRIANGLES);				//right

	glNormal3f(-1.0f,  0.0f, 0.0f );
	glVertex3f( 150.0f, -150.0f, 150.0f );
	
	glVertex3f( 150.0f, -150.0f,-150.0f );
	glVertex3f( 150.0f,  150.0f, 150.0f );
	glVertex3f( 150.0f, -150.0f,-150.0f );
	
	glVertex3f( 150.0f,  150.0f, -150.0f );
	glVertex3f( 150.0f,  150.0f, 150.0f );
 glEnd();
	glPopMatrix();

};

float angle=0;

void PlaceSceneElements()

{	

	//glLoadIdentity();

	glTranslatef(-200, 50, -1600);
	angle+=0.15f;
	if ( angle >= 360 ) angle = 0.0f;
	glRotatef(angle,0,1,0);
	glRotatef(-angle,1,0,0);
	glColor3f(1.0f,1.0f,1.0f);
	DrawCube();

	
	glLoadIdentity();

	glTranslatef(400, 0,-2500);
	//angle+=0.25f;
	//if ( angle >= 360 ) angle = 0.0f;
	glRotatef(45,0,1,0);
	glRotatef(angle,1,0,0);
	//glColor3f(0.1f,0.0f,1.0f);
	DrawCube();
	
	//glLoadIdentity();
	/*glTranslatef(40,80, 1000);
	glRotatef(-50,0,1,0);
	glRotatef(20,1,0,0);
	glColor3f(1.0f,1.0f,1.0f);
	DrawCube();*/
	/*

	//glLoadIdentity();
	glTranslatef(-50, 0, -200);
	glRotatef(-30,0,1,0);
	glRotatef(20,1,0,0);
	glColor3f(1.0f,1.0f,1.0f);
	DrawCube();*/

};

void DrawGLScene(StereoCamera cam)                                   
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 //glDisable(GL_LIGHTING);
    cam.ApplyLeftFrustum();
    
	 glColorMask(true, false, false, false);
	

    PlaceSceneElements();

	
	glClear(GL_DEPTH_BUFFER_BIT) ;
    cam.ApplyRightFrustum();
    glColorMask(false, true, true, false);
	//glColorMask(false, true, false, false);
   
	//glEnable(GL_LIGHTING);
    PlaceSceneElements();

    glColorMask(true, true, true, true);

};

