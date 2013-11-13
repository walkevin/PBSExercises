#include "Example1.h"
#include <iostream>
	Example1::Example1(){}
	Example1::~Example1(){}

	void Example1::load(){
		sh.loadAndUse("shaders/example1.glslv","shaders/example1.glslf");
		createVBO();
	}
	void Example1::display(float dTime){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_BYTE, NULL);

		glutSwapBuffers();
		glutPostRedisplay();
	}

	void Example1::close(){
		std::cout << "Example1::close" << std::endl;
		sh.destroy();
		destroyVBO();
	}
	void Example1::createVBO(){
		Vertex Vertices[] =
		{
			{ { 0.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // 0

			// Top
			{ { -0.2f, 0.8f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } }, // 1
			{ { 0.2f, 0.8f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }, // 2
			{ { 0.0f, 0.8f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } }, //3
			{ { 0.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },	// 4

			// Bottom
			{ { -0.2f, -0.8f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }, // 5
			{ { 0.2f, -0.8f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } }, // 6
			{ { 0.0f, -0.8f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } }, //7
			{ { 0.0f, -1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },	// 8

			// Left
			{ { -0.8f, -0.2f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } }, // 9
			{ { -0.8f, 0.2f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }, // 10
			{ { -0.8f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } }, //11
			{ { -1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },	// 12

			// Right
			{ { 0.8f, -0.2f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }, // 13
			{ { 0.8f, 0.2f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } }, // 14
			{ { 0.8f, 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } }, //15
			{ { 1.0f, 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } }	// 16
		};

		GLubyte Indices[] = {
			// Top
			0, 1, 3,
			0, 3, 2,
			3, 1, 4,
			3, 4, 2,

			// Bottom
			0, 5, 7,
			0, 7, 6,
			7, 5, 8,
			7, 8, 6,

			// Left
			0, 9, 11,
			0, 11, 10,
			11, 9, 12,
			11, 12, 10,

			// Right
			0, 13, 15,
			0, 15, 14,
			15, 13, 16,
			15, 16, 14
		};

		GLenum ErrorCheckValue = glGetError();
		const size_t BufferSize = sizeof(Vertices);
		const size_t VertexSize = sizeof(Vertices[0]);
		const size_t RgbOffset = sizeof(Vertices[0].XYZW);

		glGenVertexArrays(1, &VaoId);
		glBindVertexArray(VaoId);

		glGenBuffers(1, &BufferId);
		glBindBuffer(GL_ARRAY_BUFFER, BufferId);
		glBufferData(GL_ARRAY_BUFFER, BufferSize, Vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, VertexSize, 0);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, VertexSize, (GLvoid*)RgbOffset);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glGenBuffers(1, &IndexBufferId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

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
	void Example1::destroyVBO(){
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
