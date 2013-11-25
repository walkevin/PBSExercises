#include "ExampleAnimInstanced.h"
#include <iostream>
	ExampleAnimInstanced::ExampleAnimInstanced(){
		pos.reserve(n_points);
		for(int i = 0; i < n_points; i++){
 			pos[i] << (float)i/n_points, (float)(i)/n_points, (float)(i)/n_points, 1.0f;
		}
	}
	ExampleAnimInstanced::~ExampleAnimInstanced(){}

	void ExampleAnimInstanced::updatePositions(){
		for(int i = 0; i < n_points; i++){
			pos[i][0] -= 0.01f;
		}
	}

	void ExampleAnimInstanced::load(){
		sh.load("shaders/ExampleAnimInstanced.glslv","shaders/ExampleAnimInstanced.glslf");
		sh.use();
		createVBO();
	}
	void ExampleAnimInstanced::display(float dTime){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		updatePositions();
		//VertexSize and BufferSize for std::vector<Eigen::Array<float,4> >
		const size_t VertexSize = sizeof(pos[0]);//ok for std::array (size known at compile time), otherwise 4 * sizeof(pos[0][0])
		const size_t BufferSize = n_points * VertexSize;
		void* dataPointer = pos.data();


		glBindVertexArray(VaoId);

		glBindBuffer(GL_ARRAY_BUFFER, BufferId);
		glBufferData(GL_ARRAY_BUFFER, BufferSize, dataPointer, GL_STATIC_DRAW);

		GLint posLoc = glGetAttribLocation(sh.getProgramId(), "in_Position");
		glVertexAttribPointer(posLoc, 4, GL_FLOAT, GL_FALSE, VertexSize, 0);
		glEnableVertexAttribArray(posLoc);

		GLenum ErrorCheckValue = glGetError();
		if (ErrorCheckValue != GL_NO_ERROR)
		{
			fprintf(
				stderr,
				"ERROR: Could not create a VBO: %s \n",
				gluErrorString(ErrorCheckValue)
			);

			//exit(-1);
		}


		glPointSize(5.0f);
		glDrawArrays(GL_POINTS, 0, n_points);
		glutSwapBuffers();
		glutPostRedisplay();
	}

	void ExampleAnimInstanced::close(){
		std::cout << "ExampleAnimInstanced::close" << std::endl;
		sh.destroy();
		destroyVBO();
	}
	void ExampleAnimInstanced::createVBO(){
		glGenVertexArrays(1, &VaoId);
		glGenBuffers(1, &BufferId);


		float coordsC[][4] = {
			{ -0.2f, 0.8f, 0.0f, 1.0f },  // 1
			{ 0.075, 0.85f, 0.05f, 1.0f },  // 2
			{ 0.0f, 0.8f, 0.0f, 1.0f },  //3
			{ 0.0f, 1.0f, 0.0f, 1.0f }
		};
//		const size_t VertexSize = sizeof(coordsC[0]);
//		const size_t BufferSize = 4 * VertexSize;
//		void* dataPointer = coordsC;

		std::vector<float> coordsStd;
		for(int i = 0; i < n_points; i++){
			coordsStd.push_back(float(i)/n_points);
			coordsStd.push_back(0.0f);
			coordsStd.push_back(0.0f);
			coordsStd.push_back(1.0f);
		}
//		const size_t VertexSize = 4 * sizeof(coordsStd[0]);
//		const size_t BufferSize = n_points*VertexSize;
//		void* dataPointer = coordsStd.data();
	}
	void ExampleAnimInstanced::destroyVBO(){
		GLenum ErrorCheckValue = glGetError();

		//TODO: Make locations of attributes, uniforms etc. class variables, in order to properly destroy them here.
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

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
