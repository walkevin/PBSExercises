#include "RotatingCamera.h"
#include <iostream>
#include <cmath>
#include "vmath.h"
	RotatingCamera::RotatingCamera() : distance(3.), azimuth(M_PI *0.5), polar(M_PI * 0.5), angleChangeSpeed(M_PI * 0.1), zoomSpeed(0.3){}
	RotatingCamera::~RotatingCamera(){}

	vmath::mat4 RotatingCamera::getProjectionViewMatrix(double azimuth, double polar, double distance){
		float aspectRatio = glutGet( GLUT_WINDOW_WIDTH ) / glutGet( GLUT_WINDOW_HEIGHT );
		return vmath::perspective(60.0f, aspectRatio, 1.0f, 500.0f) *
				vmath::lookat(
						vmath::vec3(std::cos(azimuth) * std::sin(polar) * distance, std::cos(polar) * distance, std::sin(azimuth) * std::sin(polar) * distance),
						vmath::vec3(0.0f, 0.0f, 0.0f),
						vmath::vec3(0.0f, 1.0f, 0.0f)
						);
	}

	void RotatingCamera::load(){
		foreground_sh.load("shaders/rotatingCamera.glslv","shaders/rotatingCamera.glslf");
		foreground_sh.use();

		// Set model matrix
		GLfloat model_mat[16] = {
			 1.000000f,  0.000000f,  0.000000f, 0.000000f,
			 0.000000f,  1.000000f,  0.000000f, 0.000000f,
			 0.000000f,  0.000000f,  1.000000f, 0.000000f,
			 0.000000f,  0.000000f,  0.000000f, 1.0000000f
		};

		GLint location = glGetUniformLocation(foreground_sh.getProgramId(), "Model_mat");
		glUniformMatrix4fv(location, 1, false, model_mat);

		// Set projection*view Matrix
//		GLfloat projview_mat[16] = {
//			2.747478f, 0.000000f,  0.000000f,  0.000000f,
//			0.000000f, 2.457419f,  0.546594f,  0.447214f,
//			0.000000f, 1.228709f, -1.093189f, -0.894427f,
//			0.000000f, 0.000000f,  1.877236f,  3.354102f
//		};


		vmath::mat4 projview_mat = getProjectionViewMatrix(azimuth, polar, distance);
		location = glGetUniformLocation(foreground_sh.getProgramId(), "ProjectView_mat");
		glUniformMatrix4fv(location, 1, false, (float*)(projview_mat));

		createVBO();
	}
	void RotatingCamera::display(float dTime){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_BYTE, NULL);

		glutSwapBuffers();
		glutPostRedisplay();
	}

	void RotatingCamera::close(){
		std::cout << "RotatingCamera::close" << std::endl;
		foreground_sh.destroy();
		destroyVBO();
	}
	void RotatingCamera::createVBO(){
		Vertex Vertices[] =
		{
			//Back
			{ { -0.4f, -0.4f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // 0
			{ { -0.4f, 0.4f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } }, // 1
			{ { 0.4f, 0.4f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }, // 2
			{ { 0.4f, -0.4f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, //3

			// Front
			{ { -0.7f, -0.4f, 0.5f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // 4
			{ { -0.7f, 0.4f, 0.5f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } }, // 5
			{ { 0.7f, 0.4f, 0.5f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } }, // 6
			{ { 0.7f, -0.4f, 0.5f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // 7
		};

		GLubyte Indices[] = {
			// Back
			0, 1, 2,
			0, 2, 3,

			// Floor
			4, 3, 7,
			0, 3, 4,

			// Left wall
			0, 4, 5,
			0, 1, 5,

			// Right wall
			3, 6, 7,
			3, 6, 2
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
	void RotatingCamera::destroyVBO(){
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

	void RotatingCamera::specialKeyboardDown(int key, int x, int y )
	{
		// Subclass and override this method
		printf( "KeyboardDown: %c = %d\n", key, (int)key );
			if(key == 100)//Arrow left
				azimuth += angleChangeSpeed;
			else if(key == 102)//Arrow right
				azimuth -= angleChangeSpeed;
			else if(key == 101)//Arrow down
				polar -= angleChangeSpeed * 0.1;
			else if(key == 103)//Arrow up
				polar += angleChangeSpeed * 0.1;
			else if(key == 1)//F1
				distance += zoomSpeed;
			else if(key == 2)
				distance -= zoomSpeed;
			switch(key){
			case 27:
				glutLeaveMainLoop(); break;
			case 100:
			case 101:
			case 102:
			case 103:
			case 1:
			case 2:
				foreground_sh.use();
				vmath::mat4 projview_mat = getProjectionViewMatrix(azimuth, polar, distance);

				GLint location = glGetUniformLocation(foreground_sh.getProgramId(), "ProjectView_mat");
				glUniformMatrix4fv(location, 1, false, (float*)(projview_mat));
				glutPostRedisplay();
				break;
		}

		keyStates.keyDown( (int)key );
	}
