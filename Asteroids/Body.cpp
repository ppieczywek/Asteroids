#pragma once
#include "stdafx.h"
#include "Body.h"
#include <vector>

using namespace std;


Circle2D::Circle2D(float r, float mass) : radius(r) {
	 
	 MassCenter.mass = mass;
	 MassCenter.freeze = false;
	 diameter = 2*r;

	 BBox.min.x -= radius;
	 BBox.max.x += radius;

	 BBox.min.y -= radius;
	 BBox.max.y += radius;

};
	

void Circle2D::Draw() {
		glPointSize(diameter-0.5);	
		glBegin(GL_POINTS);
				glColor3f(0.0f,0.0f,0.0f);
				glVertex3f( MassCenter.Position.x,  MassCenter.Position.y, 0.0f );
		glEnd();

		glPointSize(diameter-4.5);	
		glBegin(GL_POINTS);
				
				//if(contact)
				//	glColor3f(1.0f,0.0f,0.0f);
				//else
					glColor3f(1.0f,1.0f,1.0f);

			//	if(rest)  glColor3f(0.0f,1.0f,0.0f);

				glVertex3f( MassCenter.Position.x,  MassCenter.Position.y, 0.0f );
		glEnd();

		
		glLineWidth(1.0f);
		glBegin(GL_LINES);
				glColor3f(0.0f,0.0f,0.0f);
				glVertex3f( MassCenter.Position.x,  MassCenter.Position.y, 0.0f );
				glVertex3f( (MassCenter.Position.x+MassCenter.Velocity.x),  (MassCenter.Position.y+MassCenter.Velocity.y), 0.0f );
				/*glVertex3f( BBox.min.x,   BBox.min.y, 0.0f );
				glVertex3f( BBox.min.x,   BBox.max.y, 0.0f );

				glVertex3f( BBox.min.x,   BBox.max.y, 0.0f );
				glVertex3f( BBox.max.x,   BBox.max.y, 0.0f );

				glVertex3f( BBox.max.x,   BBox.max.y, 0.0f );
				glVertex3f( BBox.max.x,   BBox.min.y, 0.0f );

				glVertex3f( BBox.max.x,   BBox.min.y, 0.0f );
				glVertex3f( BBox.min.x,   BBox.min.y, 0.0f );*/
		glEnd();
	};

inline void Circle2D::AppLinearAcc(const Vector2D& Acc) {
		MassCenter.Acceleration+=Acc;
};

inline void Circle2D::AppAngularAcc(const Vector2D& Acc) {
	
};

inline void Circle2D::AppLinearVel(const Vector2D& Velocity)  {
		MassCenter.Velocity+=Velocity;
	};

inline void Circle2D::AppAngularVel(const Vector2D& Velocity)  {
		
	};

void Circle2D::Integrate(const float dt) {

			
			MassCenter.Velocity += 0.5*(MassCenter.Acceleration)*dt;
			MassCenter.Position += MassCenter.Velocity*dt;// + 0.5*(MassCenter.Acceleration)*dt*dt;
			//wyznaczenie nowych przyspieszeñ
			MassCenter.Velocity += 0.5*(MassCenter.Acceleration)*dt;
			
			BBox.min.x = MassCenter.Position.x - radius;
			BBox.max.x = MassCenter.Position.x + radius;
			BBox.min.y = MassCenter.Position.y - radius;
			BBox.max.y = MassCenter.Position.y + radius;
		//rest = false;
	
	
	MassCenter.Acceleration = 0;
};

void Circle2D::BorderCollision(const Borders& Brd) {
		
	float dst = 0;
	Vector2D PlaneNormal;
	float VelocityNormal;

		for( auto ii=0; ii<4; ii++) {
			
			dst = (MassCenter.Position.x * Brd.planes[ii][0] + MassCenter.Position.y * Brd.planes[ii][1] + Brd.planes[ii][2]) - (radius-1.1);
			
			if(dst < 0) {
				
				PlaneNormal.x = Brd.planes[ii][0];
				PlaneNormal.y = Brd.planes[ii][1];
				VelocityNormal = MassCenter.Velocity * PlaneNormal;

				Translate(-(dst * (PlaneNormal)));			

				if(VelocityNormal < 0){
					auto bias = (dst/0.1)*0.2;
					MassCenter.Velocity -= 0.57*2*PlaneNormal*(VelocityNormal+bias);
				}
							
				contact = true;
				
				BBox.min.x = MassCenter.Position.x - radius;
				BBox.max.x = MassCenter.Position.x + radius;
				BBox.min.y = MassCenter.Position.y - radius;
				BBox.max.y = MassCenter.Position.y + radius;
				
			};
		};

};

void Circle2D::SetContact(bool state) {
	
	contact = state;
	
};

void Circle2D::Collide(shared_ptr<Body2D> Body){

	if (shared_ptr<Circle2D> Circle = dynamic_pointer_cast<Circle2D>(Body)) {
	
		auto N = MassCenter.Position - Circle->MassCenter.Position;
		//auto lng = N.length();
			//if(lng !=0) {
		
				auto penetration = N.length() - (radius + Circle->radius-1.1);
		
				if (penetration < 0) {
												
					N.normalize();
																	
					auto RelVel = (MassCenter.Velocity - Circle->MassCenter.Velocity)*N;

					auto correction = N*penetration*0.5;
						
						Translate(-correction);
						Circle->Translate(correction);
															
					if (RelVel < 0)
					{					
						auto bias = (penetration/0.1)*0.2;
						auto Impulse = N*((RelVel+bias))/( (MassCenter.mass) + (Circle->MassCenter.mass));
	
						MassCenter.Velocity -=0.57*2*Impulse*(MassCenter.mass); 
						contact = true;
				
						Circle->MassCenter.Velocity +=0.57*2*Impulse*(Circle->MassCenter.mass);
						Circle->contact = true;
					};

					BBox.min.x = MassCenter.Position.x - radius;
					BBox.max.x = MassCenter.Position.x + radius;
					BBox.min.y = MassCenter.Position.y - radius;
					BBox.max.y = MassCenter.Position.y + radius;
			};
		//};
	};
};

void Circle2D::Translate(const Vector2D& vector) {
	
	MassCenter.Position += vector;	
	BBox.min.x = MassCenter.Position.x - radius;
	BBox.max.x = MassCenter.Position.x + radius;
	BBox.min.y = MassCenter.Position.y - radius;
	BBox.max.y = MassCenter.Position.y + radius;
	
};

void Circle2D::Rotate(const float angle) {};


