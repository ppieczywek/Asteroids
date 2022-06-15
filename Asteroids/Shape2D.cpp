#pragma once
#include "stdafx.h"
/*#include "Shape2D.h"

#include <fstream> 
#include <istream> 
#include <iostream>
#include <conio.h>
#include <string>

Shape2D::Shape2D() {
	p_cBody = shared_ptr<cBody>(new cBody());						
	p_cRender = shared_ptr<cRender>(new cRender());
	p_cConstrained = shared_ptr<cConstrained>(new cConstrained());
};

Shape2D::~Shape2D() {
	p_cRender.reset();
	p_cBody.reset();
	p_cConstrained.reset();
};

const shared_ptr<Entity> LoadModel(const char* FileName) {

	/*shared_ptr<Entity> BodyPtr = nullptr;

	fstream ModelFile;
    ModelFile.open( FileName, ios::in );
    if( ModelFile.good() ) {

		BodyPtr = shared_ptr<Entity> (new Shape2D);
        string FileLine;
        unsigned int found;
        while( !ModelFile.eof() ) {

            getline( ModelFile, FileLine);
		
			if( (found=FileLine.find("VertexNumber")) != std::string::npos ) {
				int VertexNumber = 0;
				if(sscanf ( FileLine.c_str(),"VertexNumber\t%d", &VertexNumber)) {
					BodyPtr->p_cBody->VertexNumber = VertexNumber;
					BodyPtr->p_cBody->Vertex.assign(VertexNumber, ZeroVector);
					BodyPtr->p_cBody->VertexBuffer.assign(VertexNumber, ZeroVector);
					BodyPtr->p_cRender->VertexColor.assign(VertexNumber, Color(0.9f,0.9f,0.9f));
					BodyPtr->p_cRender->LineColor.assign(VertexNumber, Color());
				};
			};

			if( (found=FileLine.find("FaceNumber")) != std::string::npos ) {
				int FaceNumber = 0;
				if(sscanf ( FileLine.c_str(),"FaceNumber\t%d", &FaceNumber)) {
					BodyPtr->p_cBody->FaceNumber = FaceNumber;
					BodyPtr->p_cBody->FaceList.assign(FaceNumber, Face());
					BodyPtr->p_cBody->FaceFlag.assign(FaceNumber, FaceFlags());
					BodyPtr->p_cBody->FaceNormals.assign(FaceNumber, FaceVectors());
					BodyPtr->p_cBody->FaceEdges.assign(FaceNumber, FaceVectors());
				};
			};
			
			if( (found=FileLine.find("[VertexList]")) != std::string::npos ) {
				
				int index = 0;
				float x = 0.0f; 
				float y = 0.0f;

				for(auto ii=0; ii< BodyPtr->p_cBody->VertexNumber; ++ii) {
					getline( ModelFile, FileLine);
					if(sscanf ( FileLine.c_str(),"%d\t%f\t%f", &index, &x, &y)) {
						BodyPtr->p_cBody->Vertex[ii].x = x;
						BodyPtr->p_cBody->Vertex[ii].y = y;
					};
				};
			};

			if( (found=FileLine.find("[FaceList]")) != std::string::npos ) {
				int index = 0;
				int face1 = 0;
				int face2 = 0;
				int face3 = 0;
				
				for(auto ii=0; ii< BodyPtr->p_cBody->FaceNumber; ++ii) {
					getline( ModelFile, FileLine);
					if(sscanf ( FileLine.c_str(),"%d\t%d\t%d\t%d", &index, &face1, &face2, &face3)) {
						BodyPtr->p_cBody->FaceList[ii] = Face(face1, face2, face3);
						/*BodyPtr->p_cBody->FaceList[ii].index[0] = face1;
						BodyPtr->p_cBody->FaceList[ii].index[1] = face2;
						BodyPtr->p_cBody->FaceList[ii].index[2] = face3;*/
	/*				};
				};
			};

			if( (found=FileLine.find("[VertexColor]")) != std::string::npos ) {
				int index = 0;
				float col1 = 0;
				float col2 = 0;
				float col3 = 0;
				
				for(auto ii=0; ii< BodyPtr->p_cBody->VertexNumber; ++ii) {
					getline( ModelFile, FileLine);
					if(sscanf ( FileLine.c_str(),"%d\t%f\t%f\t%f", &index, &col1, &col2, &col3)) BodyPtr->p_cRender->VertexColor[ii] = Color(col1, col2, col3);
				};
			};

			if( (found=FileLine.find("[LineColor]")) != std::string::npos ) {
				int index = 0;
				float col1 = 0;
				float col2 = 0;
				float col3 = 0;
				
				for(auto ii=0; ii< BodyPtr->p_cBody->VertexNumber; ++ii) {
					getline( ModelFile, FileLine);
					if(sscanf ( FileLine.c_str(),"%d\t%f\t%f\t%f", &index, &col1, &col2, &col3)) BodyPtr->p_cRender->LineColor[ii] = Color(col1, col2, col3);
				};
			};
			
			if( (found=FileLine.find("LineWidth")) != std::string::npos ) {
				float Width = 0.0f;
				if(sscanf ( FileLine.c_str(),"LineWidth\t%f", &Width))
					BodyPtr->p_cRender->LineWidth = Width;
			};

			if( (found=FileLine.find("[FaceFlagList]")) != std::string::npos ) {
				int index = 0;
				int face1 = 0;
				int face2 = 0;
				int face3 = 0;
				
				for(auto ii=0; ii< BodyPtr->p_cBody->FaceNumber; ++ii) {
					getline( ModelFile, FileLine);
					if(sscanf ( FileLine.c_str(),"%d\t%d\t%d\t%d", &index, &face1, &face2, &face3)) {
						if(face1==0)
							BodyPtr->p_cBody->FaceFlag[ii].flag[0] = false;
						else
							BodyPtr->p_cBody->FaceFlag[ii].flag[0] = true;
						
						if(face2==0)
							BodyPtr->p_cBody->FaceFlag[ii].flag[1] = false;
						else
							BodyPtr->p_cBody->FaceFlag[ii].flag[1] = true;

						if(face3==0)
							BodyPtr->p_cBody->FaceFlag[ii].flag[2] = false;
						else
							BodyPtr->p_cBody->FaceFlag[ii].flag[2] = true;
					};
				};
			};

			if( (found=FileLine.find("BodyMass")) != std::string::npos ) {
				float Mass = 0.0f;
				if(sscanf ( FileLine.c_str(),"BodyMass\t%f", &Mass))
					BodyPtr->p_cBody->SetMass(Mass);
			};

			if( (found=FileLine.find("IsVisible")) != std::string::npos ) {
			int Visibility = 0;
				if(sscanf ( FileLine.c_str(),"IsVisible\t%d", &Visibility)) {
					if(Visibility==0)
						BodyPtr->p_cRender->SetVisible(false);
					else
						BodyPtr->p_cRender->SetVisible(true);
				};
			};

			if( (found=FileLine.find("IsCollidable")) != std::string::npos ) {
			int Visibility = 0;
				if(sscanf ( FileLine.c_str(),"IsCollidable\t%d", &Visibility)) {//?Visibility
					if(Visibility==0)
						BodyPtr->p_cCollide->SetCollidable(false);
					else
						BodyPtr->p_cCollide->SetCollidable(true);
				};
			};

			if( (found=FileLine.find("BodyType")) != std::string::npos ) {
				if( (found=FileLine.find("static2D")) != std::string::npos ) {
					BodyPtr->p_cBody->SetBodyType(static2D);
				}

				if( (found=FileLine.find("kinematic2D")) != std::string::npos ) {
					BodyPtr->p_cBody->SetBodyType(kinematic2D);
				}

				if( (found=FileLine.find("dynamic2D")) != std::string::npos ) {
					BodyPtr->p_cBody->SetBodyType(dynamic2D);
				}
			};
		
        }
        ModelFile.close();

		return BodyPtr;
    } else {*/
		
		return nullptr;    	
	//}
};*/