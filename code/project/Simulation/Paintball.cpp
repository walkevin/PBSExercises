//EXAMPLE_SRC = main.cpp Paintball.cpp Ball.cpp
//EXAMPLE_OBJ = main.o Paintball.o Ball.o

#include "Paintball.h"
#include <iostream>

#include <chrono>
#include <thread>

//OpenGL Math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../GLUTFramework/src/RotatingView.h"
#include "../GLUTFramework/src/Ball.h"
#include "../GLUTFramework/src/Pyramid.h"
#include "../GLUTFramework/src/Cuboid.h"

using namespace sph;

	Paintball::Paintball(SphSolver* solver){
		this->solver = solver;
		nActiveParticles = solver->getParticleNumber();
		activeParticles = solver->getParticles();
//		nDeadParticles = solver->getDeadParticleNumber();
		nDeadParticles = 0;
		nTotalParticles = nActiveParticles + nDeadParticles;

	}
	Paintball::~Paintball(){}

	void Paintball::updatePositions(){
		nActiveParticles = solver->getParticleNumber();
		activeParticles.reserve(nActiveParticles);
		activeParticles = solver->getParticles();
////		nDeadParticles = solver->getDeadParticleNumber();
//		deadParticles.reserve(nDeadParticles);
//		deadParticles = solver->getDeadParticles();
//		nDeadParticles = deadParticles.size();
		nTotalParticles = nActiveParticles + nDeadParticles;
		for(int i = 0; i < 10; i++)
			solver->simulationStep(0.0001);
   	//std::chrono::milliseconds dura( 1000 );
    //std::this_thread::sleep_for( dura );
	}


	void Paintball::load(){
		sh.load("shaders/paintball.vert", "shaders/paintball.frag");
		sh.use();

		//Create RotatingView Object after shader have been loaded
		rv = new RotatingView(sh, "ProjectView_mat");
		//Send default projection view matrix to GPU
		rv->updateView();

		createVBO();
	}
	void Paintball::display(float dTime){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		updatePositions();

		glUseProgram(sh.getProgramId());
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glBindVertexArray(objInfo["Ball"].vaoId);

		glBindBuffer(GL_ARRAY_BUFFER, objInfo["Ball"].bufferId[3]);
		// Map the buffer
		glm::mat4* matrices = (glm::mat4 *)glMapBuffer(GL_ARRAY_BUFFER,GL_WRITE_ONLY);
		// Set model matrices for each instance

		for (int n = 0;	n < nActiveParticles; n++){
			const float a = activeParticles[n][0];
			const float b = activeParticles[n][1];
			const float c = activeParticles[n][2];
			matrices[n] = glm::translate(glm::mat4(1.0f), glm::vec3(a,b,c));
		}
//		int i = nActiveParticles;
//		for(int n = 0;	n < nDeadParticles; n++){
//			const float a = deadParticles[n][0];
//			const float b = deadParticles[n][1];
//			const float c = deadParticles[n][2];
//			matrices[i] = glm::translate(glm::mat4(1.0f), glm::vec3(a,b,c));
//			i++;
//		}
		// Done. Unmap the buffer.
		glUnmapBuffer(GL_ARRAY_BUFFER);
		objInfo["Ball"].numInstances = nTotalParticles;
		glDrawElementsInstanced(GL_TRIANGLES, objInfo["Ball"].numElements, GL_UNSIGNED_INT, NULL, objInfo["Ball"].numInstances);

//		glBindVertexArray(objInfo["Pyramid"].vaoId);
//		glDrawElementsInstanced(GL_TRIANGLES, objInfo["Pyramid"].numElements, GL_UNSIGNED_INT, NULL, objInfo["Pyramid"].numInstances);

		glBindVertexArray(objInfo["Cuboid"].vaoId);
		//glDrawElementsInstanced(GL_TRIANGLES, objInfo["Cuboid"].numElements, GL_UNSIGNED_INT, NULL, objInfo["Cuboid"].numInstances);

		glutSwapBuffers();
		glutPostRedisplay();
	}

	void Paintball::close(){
		std::cout << "Paintball::close" << std::endl;
		sh.destroy();
		destroyVBO();
	}
	void Paintball::createVBO(){
		//BEGIN: Create and preprocess Ball
		//Load ball
		const GLuint N = 5; //#vertices on longitude (without poles)
		const GLuint M = 5; //#vertices on latitude
		const float R = 0.05f; //Radius
		GeometricObject* bal = new Ball(N, M, R);

		//Prepare multiple instances of ball
		std::vector<glm::mat4> balTransforms;
		balTransforms.assign(nTotalParticles, glm::mat4(1.0));

		//Create objectInfo struct
		objectInfo balinfo(bal->getNumElements(), balTransforms.size());
		objInfo["Ball"] = balinfo;

		//Upload to GPU
		uploadGeometricObject(bal, balTransforms.size(), balTransforms, objInfo["Ball"]);
		//END: Create and preprocess Ball

//		//BEGIN: Create and preprocess pyramid
//		//Load pyramid
//		GeometricObject* pyr = new Pyramid(0.8, 1.2, 0.9);
//
//		//Prepare multiple instances of ball
//		std::vector<glm::mat4> pyrTransforms;
//		pyrTransforms.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(1.0,0.0,0.0)));
////		pyrTransforms.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(0.7,0.4,0.0)));
//		//glm::rotate(glm::mat4(1.0f), 40.0f, glm::vec3(1, 1, 1))
//
//		//Create objectInfo struct
//		objectInfo pyrinfo(pyr->getNumElements(), pyrTransforms.size());
//		objInfo["Pyramid"] = pyrinfo;
//
//		//Upload to GPU
//		uploadGeometricObject(pyr, pyrTransforms.size(), pyrTransforms, objInfo["Pyramid"]);
//
//		//Perform transformation of vertices and register in Collision Handler
//		for(int i = 0; i < pyrTransforms.size(); i++){
//			using namespace Eigen;
//			std::vector<geometry_type> pyrData = pyr->getVertices();
//			Map<MatrixXf> rawVertices(pyrData.data(), 4, pyrData.size() / 4);//rawVertices operates on the same data as pyrData
//			Map<Matrix<float, 4, 4> > transform(glm::value_ptr(pyrTransforms[i]));
//			rawVertices = transform * rawVertices;
//			solver->addObject(pyrData, 4, pyr->getIndices());
//		}
////		END: Create and preprocess pyramid

		//BEGIN: Create and preprocess cuboid
		//Load cuboid
		GeometricObject* cub = new Cuboid(1.0, 1.0, 1.3);

		//Prepare multiple instances of ball
		std::vector<glm::mat4> cubTransforms;
		cubTransforms.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(1.5,0.0,0.0)));

		//Create objectInfo struct
		objectInfo cubinfo(cub->getNumElements(), cubTransforms.size());
		objInfo["Cuboid"] = cubinfo;

		//Upload to GPU
		//uploadGeometricObject(cub, cubTransforms.size(), cubTransforms, objInfo["Cuboid"]);

		//Perform transformation of vertices and register in Collision Handler
		for(int i = 0; i < cubTransforms.size(); i++){
			using namespace Eigen;
			std::vector<geometry_type> cubData = cub->getVertices();
			Map<MatrixXf> rawVertices(cubData.data(), 4, cubData.size() / 4);//rawVertices operates on the same data as cubData
			Map<Matrix<float, 4, 4> > transform(glm::value_ptr(cubTransforms[i]));
			rawVertices = transform * rawVertices;
			//solver->addObject(cubData, 4, cub->getIndices());
		}

		//ENd: Create and preprocess cuboid
	}
	void Paintball::destroyVBO(){
		GLenum ErrorCheckValue = glGetError();

		for(std::map<std::string, GLint>::iterator it = locs.begin(); it != locs.end(); ++it){
			glDisableVertexAttribArray((*it).second);
		}

		for(std::map<std::string, objectInfo>::iterator it = objInfo.begin(); it != objInfo.end(); ++it){
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glDeleteBuffers(4, (*it).second.bufferId);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glDeleteBuffers(1, &((*it).second.indexBufferId));

			glBindVertexArray(0);
			glDeleteVertexArrays(1, &((*it).second.vaoId));
		}
		ErrorCheckValue = glGetError();
		if (ErrorCheckValue != GL_NO_ERROR){
			std::cerr << "ERROR: Could not destroy the VBO: " << gluErrorString(ErrorCheckValue) << "\n";
			exit(-1);
		}

	}

	void Paintball::specialKeyboardDown(int key, int x, int y ){
		rv->keyboardEvent(key);
	}

	void Paintball::uploadGeometricObject(GeometricObject* obj, int numObj, std::vector<glm::mat4> objTransforms, objectInfo objInfo){
		std::vector<float> vertices = obj->getVertices();
		std::vector<float> normals = obj->getNormals();
		std::vector<GLuint> indices = obj->getIndices();
		std::vector<float> colors = obj->getColors();

		GLenum ErrorCheckValue = glGetError();
		const size_t colorSize = 4 * sizeof(colors[0]);
		const size_t bufferSizeColors = colors.size() * colorSize / 4;
		const size_t vertexSize = 4 * sizeof(vertices[0]);
		const size_t bufferSizeVertices = vertices.size() * vertexSize / 4;
		const size_t normalSize = 3 * sizeof(normals[0]);
		const size_t bufferSizeNormals = normals.size() * normalSize / 3;

		//Bind VertexArrayObject
		glBindVertexArray(objInfo.vaoId);

		//Upload vertices
		glBindBuffer(GL_ARRAY_BUFFER, objInfo.bufferId[0]);
		glBufferData(GL_ARRAY_BUFFER, bufferSizeColors, colors.data(), GL_STATIC_DRAW);
		locs["color_body"] = glGetAttribLocation(sh.getProgramId(),"color_body");
		if(locs["color_body"] == -1){
			std::cerr << "ERROR: Could not find location of color_body" << std::endl;
			exit(-1);
		}
		glVertexAttribPointer(locs["color_body"], 4, GL_FLOAT, GL_FALSE, colorSize, 0);
		glEnableVertexAttribArray(locs["color_body"]);

		//Upload vertices
		glBindBuffer(GL_ARRAY_BUFFER, objInfo.bufferId[1]);
		glBufferData(GL_ARRAY_BUFFER, bufferSizeVertices, vertices.data(), GL_STATIC_DRAW);
		locs["position"] = glGetAttribLocation(sh.getProgramId(),"position");
		if(locs["position"] == -1){
			std::cerr << "ERROR: Could not find location of position" << std::endl;
			exit(-1);
		}
		glVertexAttribPointer(locs["position"], 4, GL_FLOAT, GL_FALSE, vertexSize, 0);
		glEnableVertexAttribArray(locs["position"]);

		//Upload normals
		glBindBuffer(GL_ARRAY_BUFFER, objInfo.bufferId[2]);
		glBufferData(GL_ARRAY_BUFFER, bufferSizeNormals, normals.data(), GL_STATIC_DRAW);
		locs["normal"] = glGetAttribLocation(sh.getProgramId(),"normal");
		if(locs["normal"] == -1){
			std::cerr << "ERROR: Could not find location of normal" << std::endl;
			exit(-1);
		}
		glVertexAttribPointer(locs["normal"], 3, GL_FLOAT, GL_FALSE, normalSize, 0);
		glEnableVertexAttribArray(locs["normal"]);

		//Upload indices of ball
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objInfo.indexBufferId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), indices.data(), GL_STATIC_DRAW);

		//Prepare model matrix
		//Model matrix contains information of position of particles.
		glBindBuffer(GL_ARRAY_BUFFER, objInfo.bufferId[3]);
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
