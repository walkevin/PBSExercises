//EXAMPLE_SRC = main.cpp LustigiSzene.cpp Ball.cpp
//EXAMPLE_OBJ = main.o LustigiSzene.o Ball.o

#include "lustigiSzene.h"
#include <iostream>

//OpenGL Math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../GLUTFramework/src/RotatingView.h"
#include "../GLUTFramework/src/Ball.h"

	LustigiSzene::LustigiSzene(){}
	LustigiSzene::~LustigiSzene(){}

	void LustigiSzene::load(){
		sh.load("shaders/lustigiSzene.vert","shaders/lustigiSzene.frag");
		sh.use();

		//Create RotatingView Object AFTER shader have been loaded
		rv = new RotatingView(sh, "ProjectView_mat");
		//Send default projection view matrix to GPU
		rv->updateView();

		createVBO();
	}
	void LustigiSzene::display(float dTime){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_BYTE, NULL);
		glBindVertexArray(VaoId[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(VaoId[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glutSwapBuffers();
		glutPostRedisplay();
	}

	void LustigiSzene::close(){
		std::cout << "LustigiSzene::close" << std::endl;
		sh.destroy();
		destroyVBO();
	}
	void LustigiSzene::createVBO(){
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		float fTriangle[9]; // Data to render triangle (3 vertices, each has 3 floats)
		float fQuad[12]; // Data to render quad using triangle strips (4 vertices, each has 3 floats)
		float fTriangleColor[9];
		float fQuadColor[12];


		// Setup triangle vertices
		fTriangle[0] = -0.4f; fTriangle[1] = 0.1f; fTriangle[2] = 0.0f;
		fTriangle[3] = 0.4f; fTriangle[4] = 0.1f; fTriangle[5] = 0.0f;
		fTriangle[6] = 0.0f; fTriangle[7] = 0.7f; fTriangle[8] = 0.0f;

		// Setup triangle color

		fTriangleColor[0] = 1.0f; fTriangleColor[1] = 0.0f; fTriangleColor[2] = 0.0f;
		fTriangleColor[3] = 0.0f; fTriangleColor[4] = 1.0f; fTriangleColor[5] = 0.0f;
		fTriangleColor[6] = 0.0f; fTriangleColor[7] = 0.0f; fTriangleColor[8] = 1.0f;

		// Setup quad vertices

		fQuad[0] = -0.2f; fQuad[1] = -0.1f; fQuad[2] = 0.0f;
		fQuad[3] = -0.2f; fQuad[4] = -0.6f; fQuad[5] = 0.0f;
		fQuad[6] = 0.2f; fQuad[7] = -0.1f; fQuad[8] = 0.0f;
		fQuad[9] = 0.2f; fQuad[10] = -0.6f; fQuad[11] = 0.0f;

		// Setup quad color

		fQuadColor[0] = 1.0f; fQuadColor[1] = 0.0f; fQuadColor[2] = 0.0f;
		fQuadColor[3] = 0.0f; fQuadColor[4] = 1.0f; fQuadColor[8] = 0.0f;
		fQuadColor[6] = 0.0f; fQuadColor[7] = 0.0f; fQuadColor[5] = 1.0f;
		fQuadColor[9] = 1.0f; fQuadColor[10] = 1.0f; fQuadColor[11] = 0.0f;

		glGenVertexArrays(nVao, VaoId); // Generate two VAOs, one for triangle and one for quad
		glGenBuffers(nBuffer, BufferId); // And four VBOs

		// Setup whole triangle
		glBindVertexArray(VaoId[0]);

		glBindBuffer(GL_ARRAY_BUFFER, BufferId[0]);
		glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), fTriangle, GL_STATIC_DRAW);
		locs["inPosition"] = glGetAttribLocation(sh.getProgramId(),"inPosition");
		glVertexAttribPointer(locs["inPosition"], 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(locs["inPosition"]);

		glBindBuffer(GL_ARRAY_BUFFER, BufferId[1]);
		glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), fTriangleColor, GL_STATIC_DRAW);
		locs["inColor"] = glGetAttribLocation(sh.getProgramId(),"inColor");
		glVertexAttribPointer(locs["inColor"], 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(locs["inColor"]);


		// Setup whole quad
		glBindVertexArray(VaoId[1]);

		glBindBuffer(GL_ARRAY_BUFFER, BufferId[2]);
		glBufferData(GL_ARRAY_BUFFER, 12*sizeof(float), fQuad, GL_STATIC_DRAW);
		glVertexAttribPointer(locs["inPosition"], 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(locs["inPosition"]);

		glBindBuffer(GL_ARRAY_BUFFER, BufferId[3]);
		glBufferData(GL_ARRAY_BUFFER, 12*sizeof(float), fQuadColor, GL_STATIC_DRAW);
		glVertexAttribPointer(locs["inColor"], 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(locs["inColor"]);

		//warning, possible double declaration
		GLenum	ErrorCheckValue = glGetError();
			if (ErrorCheckValue != GL_NO_ERROR){
				std::cerr << "ERROR: Could not create a VBO: " << gluErrorString(ErrorCheckValue) << "\n";
				exit(-1);
			}

	}
	void LustigiSzene::destroyVBO(){
		GLenum ErrorCheckValue = glGetError();

		std::map<std::string, GLint>::iterator it;
		for(it = locs.begin(); it != locs.end(); ++it){
			glDisableVertexAttribArray((*it).second);
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(nBuffer, &BufferId[0]);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDeleteBuffers(nIndexBuffer, &IndexBufferId[0]);

		glBindVertexArray(0);
		glDeleteVertexArrays(nVao, &VaoId[0]);

		ErrorCheckValue = glGetError();
		if (ErrorCheckValue != GL_NO_ERROR){
			std::cerr << "ERROR: Could not destroy the VBO: " << gluErrorString(ErrorCheckValue) << "\n";
			exit(-1);
		}
	}

	void LustigiSzene::specialKeyboardDown(int key, int x, int y ){
		rv->keyboardEvent(key);
	}
