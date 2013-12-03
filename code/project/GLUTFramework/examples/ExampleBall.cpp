#include "ExampleBall.h"
#include <iostream>

//OpenGL Math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../src/Ball.h"
#include "../src/RotatingView.h"

	ExampleBall::ExampleBall(){}
	ExampleBall::~ExampleBall(){}

	void ExampleBall::load(){
		sh.load("shaders/exampleBall.vert","shaders/exampleBall.frag");
		sh.use();

		double viewDistance = 3.;
		double viewAzimuthAngle = 0.;

		//Create RotatingView Object AFTER shader have been loaded
		rv = new RotatingView(sh, "ProjectView_mat");
		//Send default projection view matrix to GPU
		rv->updateView();

		createVBO();
	}
	void ExampleBall::display(float dTime){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, NULL);

//		glPointSize(3.0f);
//		glDrawArrays(GL_POINTS, 0, 1+15+1);

		glutSwapBuffers();
		glutPostRedisplay();
	}

	void ExampleBall::close(){
		std::cout << "ExampleBall::close" << std::endl;
		sh.destroy();
		destroyVBO();
	}
	void ExampleBall::createVBO(){
		//Initialize Buffers
		unsigned int numVao = 1;
		unsigned int numBuf = 2;
		unsigned int numIBuf = 1;
		vaoId.reserve(numVao);
		bufferId.reserve(numBuf);
		indexBufferId.reserve(numIBuf);

		//One vertex array object for all buffers
		glGenVertexArrays(numVao, vaoId.data());
		glBindVertexArray(vaoId[0]);

		//Generate buffers
		glGenBuffers(numBuf, bufferId.data());
		glGenBuffers(numIBuf, indexBufferId.data());
		GLint tmp;

		//Load ball
		const GLuint N = 36; //#vertices on longitude (without poles)
		const GLuint M = 24; //#vertices on latitude
		const float R = 1.; //Radius

		Ball b(N, M, R);
		std::vector<float> vertices = b.getVertices();
		std::vector<float> normals = b.getNormals();
		std::vector<GLuint> indices = b.getIndices();
		numElements = b.getNumElements();

		GLenum ErrorCheckValue = glGetError();
		const size_t vertexSize = 4 * sizeof(vertices[0]);
		const size_t bufferSizeVertices = vertices.size() * vertexSize;
		const size_t normalSize = 3 * sizeof(normals[0]);
		const size_t bufferSizeNormals = normals.size() * normalSize;

		//Upload vertices
		glBindBuffer(GL_ARRAY_BUFFER, bufferId[0]);
		glBufferData(GL_ARRAY_BUFFER, bufferSizeVertices, vertices.data(), GL_STATIC_DRAW);
		tmp = glGetAttribLocation(sh.getProgramId(),"position");
		if(tmp != -1)
			locs.push_back(tmp);
		else{
			std::cerr << "ERROR: Could not find location of position" << std::endl;
			exit(0);
		}
		glVertexAttribPointer(locs.back(), 4, GL_FLOAT, GL_FALSE, vertexSize, 0);
		glEnableVertexAttribArray(locs.back());

		//Upload normals
		glBindBuffer(GL_ARRAY_BUFFER, bufferId[1]);
		glBufferData(GL_ARRAY_BUFFER, bufferSizeNormals, normals.data(), GL_STATIC_DRAW);
		tmp = glGetAttribLocation(sh.getProgramId(),"normal");
		if(tmp != -1)
			locs.push_back(tmp);
		else{
			std::cerr << "ERROR: Could not find location of normal" << std::endl;
			exit(0);
		}
		glVertexAttribPointer(locs.back(), 3, GL_FLOAT, GL_FALSE, normalSize, 0);
		glEnableVertexAttribArray(locs.back());

		//Upload indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), indices.data(), GL_STATIC_DRAW);

		ErrorCheckValue = glGetError();
		if (ErrorCheckValue != GL_NO_ERROR)
		{
			fprintf(
				stderr,
				"ERROR: Could not create a VBO: %s \n",
				gluErrorString(ErrorCheckValue)
			);

			exit(-1);
		}
	}
	void ExampleBall::destroyVBO(){
		GLenum ErrorCheckValue = glGetError();

		for(std::vector<GLint>::iterator it = locs.begin(); it != locs.end(); ++it){
			glDisableVertexAttribArray(*it);
		}
		if(bufferId.size() > 0){
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glDeleteBuffers(bufferId.size(), bufferId.data());
		}
		if(indexBufferId.size() > 0){
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glDeleteBuffers(indexBufferId.size(), indexBufferId.data());
		}
		if(vaoId.size() > 0){
			glBindVertexArray(0);
			glDeleteVertexArrays(vaoId.size(), vaoId.data());
		}

		ErrorCheckValue = glGetError();
		if (ErrorCheckValue != GL_NO_ERROR)
		{
			fprintf(
				stderr,
				"ERROR: Could not destroy the VBO: %s \n",
				gluErrorString(ErrorCheckValue)
			);

			exit(-1);
		}
	}

	void ExampleBall::specialKeyboardDown(int key, int x, int y ){
		rv->keyboardEvent(key);
	}
