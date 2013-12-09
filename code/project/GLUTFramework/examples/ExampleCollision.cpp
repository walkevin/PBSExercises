//EXAMPLE_SRC = main.cpp ExampleCollision.cpp Ball.cpp
//EXAMPLE_OBJ = main.o ExampleCollision.o Ball.o

#include "ExampleCollision.h"
#include <iostream>
#include <tuple>

//OpenGL Math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../src/RotatingView.h"
#include "../src/Ball.h"
#include "../src/Pyramid.h"
#include "../src/Cuboid.h"

	ExampleCollision::ExampleCollision(){
		n_points = 1;
		pos.reserve(n_points);
		for(int i = 0; i < n_points; i++){
			pos[i] << (float)i/n_points, 0.0f, 0.0f, 1.0f;
		}
		vel.reserve(n_points);
		for(int i = 0; i < n_points; i++){
			vel[i] << 0.01f, 0.0f, 0.0f, 0.0f;
		}

		//Init vertex array objects, buffers
		nVao = 3;
		nBuffer = 3 * nVao;
		nIndexBuffer = nVao;

		vaoId = new GLuint[nVao];
		bufferId = new GLuint[nBuffer];
		indexBufferId = new GLuint[nIndexBuffer];
	}
	ExampleCollision::~ExampleCollision(){}

	void ExampleCollision::updatePositions(){
		using namespace CollisionHandlerNS;
		for(int i = 0; i < n_points; i++){
			position_t oldPos(pos[i].head<3>());
			pos[i] += vel[i];
			position_t newPos(pos[i].head<3>());
			std::tuple<bool, position_t, velocity_t> collisionRes = ch.particleVsAllObjects(newPos, oldPos, vel[i].head<3>());
			if(std::get<0>(collisionRes)){
				std::cout << "Collision occured" << std::endl;
				std::cout << "New Position: " << std::get<1>(collisionRes) << std::endl;
				std::cout << "New Velocity: " << std::get<2>(collisionRes) << std::endl;
				pos[i].head<3>() = std::get<1>(collisionRes);
				vel[i].head<3>() = std::get<2>(collisionRes);
			}
		}

	}


	void ExampleCollision::load(){
		sh.load("shaders/exampleAnimInstanced.vert","shaders/exampleAnimInstanced.frag");
		sh.use();

		//Create RotatingView Object AFTER shader have been loaded
		rv = new RotatingView(sh, "ProjectView_mat");
		//Send default projection view matrix to GPU
		rv->updateView();

		createVBO();
	}
	void ExampleCollision::display(float dTime){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		updatePositions();

		glUseProgram(sh.getProgramId());
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glBindVertexArray(vaoId[0]);

		glBindBuffer(GL_ARRAY_BUFFER, bufferId[2]);
		// Map the buffer
		glm::mat4* matrices = (glm::mat4 *)glMapBuffer(GL_ARRAY_BUFFER,GL_WRITE_ONLY);
		// Set model matrices for each instance

		for (int n = 0;	n < objInfo[0].numInstances; n++){
			const float a = pos[n][0];
			const float b = pos[n][1];
			const float c = pos[n][2];
			matrices[n] = glm::translate(glm::mat4(1.0f), glm::vec3(a,b,c));
		}

		// Done. Unmap the buffer.
		glUnmapBuffer(GL_ARRAY_BUFFER);

		glDrawElementsInstanced(GL_TRIANGLES, objInfo[0].numElements, GL_UNSIGNED_INT, NULL, objInfo[0].numInstances);

		glBindVertexArray(vaoId[1]);
		glDrawElementsInstanced(GL_TRIANGLES, objInfo[1].numElements, GL_UNSIGNED_INT, NULL, objInfo[1].numInstances);

		glBindVertexArray(vaoId[2]);
		glDrawElementsInstanced(GL_TRIANGLES, objInfo[2].numElements, GL_UNSIGNED_INT, NULL, objInfo[2].numInstances);

//		glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, NULL);
		glutSwapBuffers();
		glutPostRedisplay();
	}

	void ExampleCollision::close(){
		std::cout << "ExampleCollision::close" << std::endl;
		sh.destroy();
		destroyVBO();
	}
	void ExampleCollision::createVBO(){
		//One vertex array object for the ball
		glGenVertexArrays(nVao, vaoId);
		glGenBuffers(nBuffer, bufferId);
		glGenBuffers(nIndexBuffer, indexBufferId);

		//BEGIN: Upload Ball
		//Load ball
		const GLuint N = 10; //#vertices on longitude (without poles)
		const GLuint M = 10; //#vertices on latitude
		const float R = 0.05; //Radius
		GeometricObject* bal = new Ball(N, M, R);

		glBindVertexArray(vaoId[0]);
		std::vector<glm::mat4> balTransforms;
		balTransforms.assign(n_points, glm::mat4(1.0));
		uploadGeometricObject(bal, balTransforms.size(), balTransforms, 0);

		objectInfo balinfo(bal->getNumElements(), balTransforms.size());
		objInfo.push_back(balinfo);

		//ENd: Upload ball

		//BEGIN: Upload Pyramid
		//Load pyramid
		GeometricObject* pyr = new Pyramid(0.8, 1.2, 0.9);

		glBindVertexArray(vaoId[1]);
		std::vector<glm::mat4> pyrTransforms;
		pyrTransforms.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(0.0,0.0,0.0)));
//		pyrTransforms.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(1.0,0.0,0.0)));
		//glm::rotate(glm::mat4(1.0f), 40.0f, glm::vec3(1, 1, 1))
		uploadGeometricObject(pyr, pyrTransforms.size(), pyrTransforms, 1);
		objectInfo pyrinfo(pyr->getNumElements(), pyrTransforms.size());
		objInfo.push_back(pyrinfo);

		ch.addObject(pyr->getVertices(), 4, pyr->getIndices());
//		ENd: Upload pyramid


		//BEGIN: Upload cuboid
		//Load cuboid
		GeometricObject* cub = new Cuboid(1.0, 1.0, 1.3);

		glBindVertexArray(vaoId[2]);
		std::vector<glm::mat4> cubTransforms;
		cubTransforms.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(0.0,0.0,0.0)));
		uploadGeometricObject(cub, cubTransforms.size(), cubTransforms, 2);
		objectInfo cubinfo(cub->getNumElements(), cubTransforms.size());
		objInfo.push_back(cubinfo);
		//TODO: Store cub->getVertices, loop over cubTransforms.size and compute cubTransforms * vertices and then addObject to collisionHandler
		ch.addObject(cub->getVertices(), 4, cub->getIndices());

		//ENd: Upload cuboid

	}
	void ExampleCollision::destroyVBO(){
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

	void ExampleCollision::specialKeyboardDown(int key, int x, int y ){
		rv->keyboardEvent(key);
	}

	void ExampleCollision::uploadGeometricObject(GeometricObject* obj, int numObj, std::vector<glm::mat4> objTransforms, int bufferStride){
		std::cout << "Uploading geometric object" << std::endl;
		std::vector<float> vertices = obj->getVertices();
		std::vector<float> normals = obj->getNormals();
		std::vector<GLuint> indices = obj->getIndices();

		GLenum ErrorCheckValue = glGetError();
		const size_t vertexSize = 4 * sizeof(vertices[0]);
		const size_t bufferSizeVertices = vertices.size() * vertexSize;
		const size_t normalSize = 3 * sizeof(normals[0]);
		const size_t bufferSizeNormals = normals.size() * normalSize;

		//Upload vertices
		glBindBuffer(GL_ARRAY_BUFFER, bufferId[bufferStride*3 + 0]);
		glBufferData(GL_ARRAY_BUFFER, bufferSizeVertices, vertices.data(), GL_STATIC_DRAW);
		locs["position"] = glGetAttribLocation(sh.getProgramId(),"position");
		if(locs["position"] == -1){
			std::cerr << "ERROR: Could not find location of position" << std::endl;
			exit(-1);
		}
		glVertexAttribPointer(locs["position"], 4, GL_FLOAT, GL_FALSE, vertexSize, 0);
		glEnableVertexAttribArray(locs["position"]);

		//Upload normals
		glBindBuffer(GL_ARRAY_BUFFER, bufferId[bufferStride*3 + 1]);
		glBufferData(GL_ARRAY_BUFFER, bufferSizeNormals, normals.data(), GL_STATIC_DRAW);
		locs["normal"] = glGetAttribLocation(sh.getProgramId(),"normal");
		if(locs["normal"] == -1){
			std::cerr << "ERROR: Could not find location of normal" << std::endl;
			exit(-1);
		}
		glVertexAttribPointer(locs["normal"], 3, GL_FLOAT, GL_FALSE, normalSize, 0);
		glEnableVertexAttribArray(locs["normal"]);

		//Upload indices of ball
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId[bufferStride*3 + 0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), indices.data(), GL_STATIC_DRAW);

		//Prepare model matrix
		//Model matrix contains information of position of particles.
		glBindBuffer(GL_ARRAY_BUFFER, bufferId[bufferStride*3 + 2]);
		glBufferData(GL_ARRAY_BUFFER, numObj * sizeof(objTransforms[0]), objTransforms.data(), GL_DYNAMIC_DRAW);
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
