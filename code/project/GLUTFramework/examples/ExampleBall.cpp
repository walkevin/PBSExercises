#include "ExampleBall.h"
#include <iostream>
#include <cmath>
#include "vmath.h"
#include "Ball.h"
	ExampleBall::ExampleBall(){}
	ExampleBall::~ExampleBall(){}

	void ExampleBall::load(){
		sh.load("shaders/exampleBall.glslv","shaders/exampleBall.glslf");
		sh.use();

		double viewDistance = 3.;
		double viewAzimuthAngle = 0.;

		// Set projection*view Matrix
		vmath::mat4 projview_mat = vmath::perspective(60.0f, float(WINDOW_WIDTH/WINDOW_HEIGHT), 1.0f, 500.0f) *
				                   vmath::lookat(vmath::vec3(std::cos(viewAzimuthAngle) * viewDistance, std::sin(viewAzimuthAngle) * viewDistance, 2.0f), vmath::vec3(0.0f, 0.0f, 0.0f), vmath::vec3(0.0f, 0.0f, 1.0f));


		GLint location = glGetUniformLocation(sh.getProgramId(), "ProjectView_mat");
		glUniformMatrix4fv(location, 1, false, (float*)(projview_mat));
		createVBO();
	}
	void ExampleBall::display(float dTime){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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

		const GLuint N = 36; //#vertices on longitude (without poles)
		const GLuint M = 24; //#vertices on latitude
		const float R = 1.; //Radius

		Ball b(N, M, R);
		std::vector<float> vertices = b.getVertices();
		std::vector<float> normals = b.getNormals();
		std::vector<GLuint> indices = b.getIndices();
		numElements = b.getNumElements();

		GLenum ErrorCheckValue = glGetError();
		const size_t vertexSize = 4* sizeof(vertices[0]);
		const size_t bufferSize = vertices.size() * vertexSize;

		glGenVertexArrays(1, &VaoId);
		glBindVertexArray(VaoId);

		glGenBuffers(1, &BufferId);
		glBindBuffer(GL_ARRAY_BUFFER, BufferId);
		glBufferData(GL_ARRAY_BUFFER, bufferSize, vertices.data(), GL_STATIC_DRAW);
		GLint posLoc = glGetAttribLocation(sh.getProgramId(),"position");
		glVertexAttribPointer(posLoc, 4, GL_FLOAT, GL_FALSE, vertexSize, 0);
		glEnableVertexAttribArray(posLoc);


//		//glGenBuffers(1, &BufferId);
//		glBindBuffer(GL_ARRAY_BUFFER, BufferId);
//		glBufferData(GL_ARRAY_BUFFER, bufferSize, normals.data(), GL_STATIC_DRAW);
//		GLint norLoc = glGetAttribLocation(sh.getProgramId(),"normal");
//		glVertexAttribPointer(norLoc, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);
//		glEnableVertexAttribArray(norLoc);


		glGenBuffers(1, &IndexBufferId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferId);
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

		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &BufferId);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &IndexBufferId);

		glBindVertexArray(0);
		glDeleteVertexArrays(1, &VaoId);

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
