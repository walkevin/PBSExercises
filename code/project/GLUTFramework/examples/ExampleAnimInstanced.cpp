//EXAMPLE_SRC = main.cpp ExampleAnimInstanced.cpp Ball.cpp
//EXAMPLE_OBJ = main.o ExampleAnimInstanced.o Ball.o

#include "ExampleAnimInstanced.h"
#include <iostream>

//OpenGL Math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../src/RotatingView.h"
#include "../src/Ball.h"

	ExampleAnimInstanced::ExampleAnimInstanced(){
		pos.reserve(n_points);
		for(int i = 0; i < n_points; i++){
			pos[i] << (float)i/n_points, 0.0f, 0.0f, 1.0f;
// 			pos[i] << (float)i/n_points, (float)(i)/n_points, (float)(i)/n_points, 1.0f;
		}
	}
	ExampleAnimInstanced::~ExampleAnimInstanced(){}

	void ExampleAnimInstanced::updatePositions(){
		for(int i = 0; i < n_points; i++){
			//pos[i][0] -= 0.01f;
		}
	}


	void ExampleAnimInstanced::load(){
		sh.load("shaders/exampleAnimInstanced.vert","shaders/exampleAnimInstanced.frag");
		sh.use();

		//Create RotatingView Object AFTER shader have been loaded
		rv = new RotatingView(sh, "ProjectView_mat");
		//Send default projection view matrix to GPU
		rv->updateView();

		createVBO();
	}
	void ExampleAnimInstanced::display(float dTime){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		updatePositions();

		glBindVertexArray(vaoId[0]);
		//vertexSize and bufferSize for std::vector<Eigen::Array<float,4> >
		const size_t vertexSize = sizeof(pos[0]);//ok for std::array (size known at compile time), otherwise 4 * sizeof(pos[0][0])
		const size_t bufferSize = n_points * vertexSize;

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
//		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
		//One vertex array object for the ball
		glGenVertexArrays(nVao, vaoId);
		glGenBuffers(nBuffer, bufferId);
		glGenBuffers(nIndexBuffer, indexBufferId);

		//Upload Ball
		glBindVertexArray(vaoId[0]);

		GLint tmp;

		//Load ball
		const GLuint N = 10; //#vertices on longitude (without poles)
		const GLuint M = 10; //#vertices on latitude
		const float R = 0.05; //Radius

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
		locs["position"] = glGetAttribLocation(sh.getProgramId(),"position");
		if(locs["position"] == -1){
			std::cerr << "ERROR: Could not find location of position" << std::endl;
			exit(-1);
		}
		glVertexAttribPointer(locs["position"], 4, GL_FLOAT, GL_FALSE, vertexSize, 0);
		glEnableVertexAttribArray(locs["position"]);

		//Upload normals
		glBindBuffer(GL_ARRAY_BUFFER, bufferId[1]);
		glBufferData(GL_ARRAY_BUFFER, bufferSizeNormals, normals.data(), GL_STATIC_DRAW);
		locs["normal"] = glGetAttribLocation(sh.getProgramId(),"normal");
		if(locs["normal"] == -1){
			std::cerr << "ERROR: Could not find location of normal" << std::endl;
			exit(-1);
		}
		glVertexAttribPointer(locs["normal"], 3, GL_FLOAT, GL_FALSE, normalSize, 0);
		glEnableVertexAttribArray(locs["normal"]);

		//Upload indices of ball
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), indices.data(), GL_STATIC_DRAW);

		//Prepare model matrix
		//Model matrix contains information of position of particles.
		glBindBuffer(GL_ARRAY_BUFFER, bufferId[2]);
		glBufferData(GL_ARRAY_BUFFER, n_points * sizeof(glm::mat4), vertices.data(), GL_STATIC_DRAW);//Fill with any data
		locs["modelMat"] = glGetAttribLocation(sh.getProgramId(),"modelMat");
		// Loop over each column of the matrix...
		for (int i = 0; i < 4; i++)	{
			// Set up the vertex attribute
			glVertexAttribPointer(locs["modelMat"] + i,	4, GL_FLOAT, GL_FALSE,	sizeof(glm::mat4),(void *)(sizeof(glm::vec4) * i));
			// Enable it
			glEnableVertexAttribArray(locs["modelMat"] + i);
			// Make it instanced
			glVertexAttribDivisor(locs["modelMat"] + i, 1);
		}


	}
	void ExampleAnimInstanced::destroyVBO(){
		GLenum ErrorCheckValue = glGetError();

		for(std::map<std::string, GLint>::iterator it = locs.begin(); it != locs.end(); ++it){
			glDisableVertexAttribArray((*it).second);
		}
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(nBuffer, bufferId);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDeleteBuffers(nIndexBuffer, indexBufferId);

		glBindVertexArray(0);
		glDeleteVertexArrays(nVao, vaoId);

		ErrorCheckValue = glGetError();
		if (ErrorCheckValue != GL_NO_ERROR){
			std::cerr << "ERROR: Could not destroy the VBO: " << gluErrorString(ErrorCheckValue) << "\n";
			exit(-1);
		}
	}

	void ExampleAnimInstanced::specialKeyboardDown(int key, int x, int y ){
		rv->keyboardEvent(key);
	}
