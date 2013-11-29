//EXAMPLE_SRC = main.cpp ExampleAnimInstanced.cpp Ball.cpp
//EXAMPLE_OBJ = main.o ExampleAnimInstanced.o Ball.o

#ifdef SIMULATION

#define PATH_VERT "../GLUTFramework/examples/shaders/exampleAnimInstanced.vert"
#define PATH_FRAG "../GLUTFramework/examples/shaders/exampleAnimInstanced.frag"

#else

#define PATH_VERT "shaders/exampleAnimInstanced.vert"
#define PATH_FRAG "shaders/exampleAnimInstanced.frag"

#endif

#include "ExampleAnimInstanced.h"
#include <iostream>

//OpenGL Math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../src/RotatingView.h"
#include "Ball.h"

using namespace sph;

	ExampleAnimInstanced::ExampleAnimInstanced(SphSolver* solver){
		this->solver = solver;
		pos.reserve(n_points);
		for(int i = 0; i < n_points; i++){
			pos[i] << (float)i/n_points, 0.0f, 0.0f, 1.0f;
// 			pos[i] << (float)i/n_points, (float)(i)/n_points, (float)(i)/n_points, 1.0f;
		}
	}
	ExampleAnimInstanced::~ExampleAnimInstanced(){}

	void ExampleAnimInstanced::updatePositions(){
		pos = solver->getParticles();
		solver->simulationStep(0.001);
	}


	void ExampleAnimInstanced::load(){
		sh.load(PATH_VERT, PATH_FRAG);
		sh.use();

		//Create RotatingView Object after shader have been loaded
		rv = new RotatingView(sh, "ProjectView_mat");
		//Send default projection view matrix to GPU
		rv->updateView();

		createVBO();
	}
	void ExampleAnimInstanced::display(float dTime){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		updatePositions();
		//vertexSize and bufferSize for std::vector<Eigen::Array<float,4> >
		const size_t vertexSize = sizeof(pos[0]);//ok for std::array (size known at compile time), otherwise 4 * sizeof(pos[0][0])
		const size_t bufferSize = n_points * vertexSize;
		void* dataPointer = pos.data();

		// Map the buffer
		glm::mat4* matrices = (glm::mat4 *)glMapBuffer(GL_ARRAY_BUFFER,GL_WRITE_ONLY);
		// Set model matrices for each instance

		for (int n = 0;	n < n_points; n++){
			const float a = pos[n][0];
			const float b = pos[n][1];
			const float c = pos[n][2];
			matrices[n] = glm::translate(glm::mat4(1.0f), glm::vec3(a,b,c));
		}

		// Done. Unmap the buffer.
		glUnmapBuffer(GL_ARRAY_BUFFER);

		// Activate instancing progr
		glUseProgram(sh.getProgramId());
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//Draw n_points instances of balls.
		glDrawElementsInstanced(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, NULL, n_points);

//		glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, NULL);
		glutSwapBuffers();
		glutPostRedisplay();
	}

	void ExampleAnimInstanced::close(){
		std::cout << "ExampleAnimInstanced::close" << std::endl;
		sh.destroy();
		destroyVBO();
	}
	void ExampleAnimInstanced::createVBO(){
		//Initialize Buffers
		unsigned int numVao = 1;
		unsigned int numBuf = 2;
		unsigned int numIBuf = 1;
		vaoId.reserve(numVao);
		bufferId.reserve(numBuf);
		indexBufferId.reserve(numIBuf);
		glGenVertexArrays(numVao, vaoId.data());
		glGenBuffers(numBuf, bufferId.data());
		glGenBuffers(numIBuf, indexBufferId.data());

		//Load ball
		const GLuint N = 10; //#vertices on longitude (without poles)
		const GLuint M = 10; //#vertices on latitude
		const float R = 0.05f; //Radius

		Ball b(N, M, R);
		std::vector<float> vertices = b.getVertices();
		std::vector<float> normals = b.getNormals();
		std::vector<GLuint> indices = b.getIndices();
		//numElements is needed in display-function
		numElements = b.getNumElements();

		GLenum ErrorCheckValue = glGetError();
		const size_t vertexSize = 4* sizeof(vertices[0]);
		const size_t bufferSize = vertices.size() * vertexSize;

		glBindVertexArray(vaoId[0]);
		//Upload vertices of ball
		glBindBuffer(GL_ARRAY_BUFFER, bufferId[0]);
		glBufferData(GL_ARRAY_BUFFER, bufferSize, vertices.data(), GL_STATIC_DRAW);
		locs.push_back(glGetAttribLocation(sh.getProgramId(),"position"));
		glVertexAttribPointer(locs.back(), 4, GL_FLOAT, GL_FALSE, vertexSize, 0);
		glEnableVertexAttribArray(locs.back());

//		//Upload normals of ball
//		glBindBuffer(GL_ARRAY_BUFFER, bufferId[0]);
//		glBufferData(GL_ARRAY_BUFFER, bufferSize, normals.data(), GL_STATIC_DRAW);
//		locs.push_back(glGetAttribLocation(sh.getProgramId(),"normal"));
//		glVertexAttribPointer(locs.back(), 4, GL_FLOAT, GL_FALSE, vertexSize, 0);
//		glEnableVertexAttribArray(locs.back());

		//Upload indices of ball
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), indices.data(), GL_STATIC_DRAW);

		//Prepare model matrix
		//Model matrix contains information of position of particles.
		glBindBuffer(GL_ARRAY_BUFFER, bufferId[1]);
		glBufferData(GL_ARRAY_BUFFER, n_points * sizeof(glm::mat4), vertices.data(), GL_STATIC_DRAW);
		locs.push_back(glGetAttribLocation(sh.getProgramId(),"modelMat"));
		// Loop over each column of the matrix...
		for (int i = 0; i < 4; i++)	{
			// Set up the vertex attribute
			glVertexAttribPointer(locs.back() + i,	4, GL_FLOAT, GL_FALSE,	sizeof(glm::mat4),(void *)(sizeof(glm::vec4) * i));
			// Enable it
			glEnableVertexAttribArray(locs.back() + i);
			// Make it instanced
			glVertexAttribDivisor(locs.back() + i, 1);
		}

	}
	void ExampleAnimInstanced::destroyVBO(){
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

	void ExampleAnimInstanced::specialKeyboardDown(int key, int x, int y ){
		rv->keyboardEvent(key);
	}
