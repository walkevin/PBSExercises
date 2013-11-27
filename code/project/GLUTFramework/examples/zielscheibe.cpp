#include "RotatingCamera.h"
#include "zielscheibe.h"
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
		//blakommentar
		foreground_sh.load("shaders/lustigiSzene.vert","shaders/lustigiSzene.frag");
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
		//glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_BYTE, NULL);
		glBindVertexArray(VaoId[0]);
		// 2D: triangle draw
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawArrays(GL_TRIANGLES, 0, 12);
		glBindVertexArray(VaoId[1]);
		// 3D: quad draw
		//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		glutSwapBuffers();
		glutPostRedisplay();
	}

	void RotatingCamera::close(){
		std::cout << "RotatingCamera::close" << std::endl;
		foreground_sh.destroy();
		destroyVBO();
	}
	void RotatingCamera::createVBO(){
////////////////////////////////////////////////////////////////////////////////////
// original Kevin camera example
////////////////////////////////////////////////////////////////////////////////////

//		Vertex Vertices[] =
//		{
//			//Back
//			{ { -0.4f, -0.4f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // 0
//			{ { -0.4f, 0.4f, 0.0f, 1.0f }, { 0.0f, 1.0f, 1.0f, 1.0f } }, // 1
//			{ { 0.4f, 0.4f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }, // 2
//			{ { 0.4f, -0.4f, 0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, //3
//
//			// Front
//			{ { -0.4f, -0.4f, 0.5f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // 4
//			{ { -0.4f, 0.4f, 0.5f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } }, // 5
//			{ { 0.4f, 0.4f, 0.5f, 1.0f }, { 1.0f, 1.0f, 0.0f, 1.0f } }, // 6
//			{ { 0.4f, -0.4f, 0.5f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f } }, // 7
//		};
//
//		GLubyte Indices[] = {
//			// Back
//			0, 1, 2,
//			0, 2, 3,
//
//			// Floor
//			4, 3, 7,
//			0, 3, 4,
//
//			// Left wall
//			0, 4, 5,
//			0, 1, 5,
//
//			// Right wall
//			3, 6, 7,
//			3, 6, 2
//		};
//
//		GLenum ErrorCheckValue = glGetError();
//		const size_t BufferSize = sizeof(Vertices);
//		const size_t VertexSize = sizeof(Vertices[0]);
//		const size_t RgbOffset = sizeof(Vertices[0].XYZW);
//
//		glGenVertexArrays(1, &VaoId);
//		glBindVertexArray(VaoId);
//
//		glGenBuffers(1, &BufferId);
//		glBindBuffer(GL_ARRAY_BUFFER, BufferId);
//		glBufferData(GL_ARRAY_BUFFER, BufferSize, Vertices, GL_STATIC_DRAW);
//
//		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, VertexSize, 0);
//		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, VertexSize, (GLvoid*)RgbOffset);
//
//		glEnableVertexAttribArray(0);
//		glEnableVertexAttribArray(1);
//
//		glGenBuffers(1, &IndexBufferId);
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferId);
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

////////////////////////////////////////////////////////////////////////////////////
// 2d Triangle and Quad
////////////////////////////////////////////////////////////////////////////////////
//
//	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//
//	float fTriangle[9]; // Data to render triangle (3 vertices, each has 3 floats)
//	float fQuad[12]; // Data to render quad using triangle strips (4 vertices, each has 3 floats)
//	float fTriangleColor[9];
//	float fQuadColor[12];
//
//
//	// Setup triangle vertices
//	fTriangle[0] = -0.4f; fTriangle[1] = 0.1f; fTriangle[2] = 0.0f;
//	fTriangle[3] = 0.4f; fTriangle[4] = 0.1f; fTriangle[5] = 0.0f;
//	fTriangle[6] = 0.0f; fTriangle[7] = 0.7f; fTriangle[8] = 0.0f;
//
//	// Setup triangle color
//
//	fTriangleColor[0] = 1.0f; fTriangleColor[1] = 0.0f; fTriangleColor[2] = 0.0f;
//	fTriangleColor[3] = 0.0f; fTriangleColor[4] = 1.0f; fTriangleColor[5] = 0.0f;
//	fTriangleColor[6] = 0.0f; fTriangleColor[7] = 0.0f; fTriangleColor[8] = 1.0f;
// 
//	// Setup quad vertices
// 
//	fQuad[0] = -0.2f; fQuad[1] = -0.1f; fQuad[2] = 0.0f;
//	fQuad[3] = -0.2f; fQuad[4] = -0.6f; fQuad[5] = 0.0f;
//	fQuad[6] = 0.2f; fQuad[7] = -0.1f; fQuad[8] = 0.0f;
//	fQuad[9] = 0.2f; fQuad[10] = -0.6f; fQuad[11] = 0.0f;
//
//	// Setup quad color
//
//	fQuadColor[0] = 1.0f; fQuadColor[1] = 0.0f; fQuadColor[2] = 0.0f;
//	fQuadColor[3] = 0.0f; fQuadColor[4] = 1.0f; fQuadColor[8] = 0.0f;
//	fQuadColor[6] = 0.0f; fQuadColor[7] = 0.0f; fQuadColor[5] = 1.0f;
//	fQuadColor[9] = 1.0f; fQuadColor[10] = 1.0f; fQuadColor[11] = 0.0f;
//
//	glGenVertexArrays(2, VaoId); // Generate two VAOs, one for triangle and one for quad
//	glGenBuffers(4, BufferId); // And four VBOs
//
//	// Setup whole triangle
//	glBindVertexArray(VaoId[0]);
//
//	glBindBuffer(GL_ARRAY_BUFFER, BufferId[0]);
//	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), fTriangle, GL_STATIC_DRAW);
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
//
//	glBindBuffer(GL_ARRAY_BUFFER, BufferId[1]);
//	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), fTriangleColor, GL_STATIC_DRAW);
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
//
//	// Setup whole quad
//	glBindVertexArray(VaoId[1]);
//
//	glBindBuffer(GL_ARRAY_BUFFER, BufferId[2]);
//	glBufferData(GL_ARRAY_BUFFER, 12*sizeof(float), fQuad, GL_STATIC_DRAW);
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
//
//	glBindBuffer(GL_ARRAY_BUFFER, BufferId[3]);
//	glBufferData(GL_ARRAY_BUFFER, 12*sizeof(float), fQuadColor, GL_STATIC_DRAW);
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
//
////////////////////////////////////////////////////////////////////////////////////
// 3d pyramid and Quad
////////////////////////////////////////////////////////////////////////////////////
	float fPyramid[36]; // Pyramid data - 4 triangles of 3 vertices of 3 floats
	float fPyramidColor[36]; // Same for color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Setup pyramid

	// Front face
	fPyramid[0] = 0.0f; fPyramid[1] = 5.0f; fPyramid[2] = 0.0f;
	fPyramid[3] = -3.0f; fPyramid[4] = 0.0f; fPyramid[5] = 3.0f;
	fPyramid[6] = 3.0f; fPyramid[7] = 0.0f; fPyramid[8] = 3.0f;

	// Back face
	fPyramid[9] = 0.0f; fPyramid[10] = 5.0f; fPyramid[11] = 0.0f;
	fPyramid[12] = 3.0f; fPyramid[13] = 0.0f; fPyramid[14] = -3.0f;
	fPyramid[15] = -3.0f; fPyramid[16] = 0.0f; fPyramid[17] = -3.0f;

	// Left face
	fPyramid[18] = 0.0f; fPyramid[19] = 5.0f; fPyramid[20] = 0.0f;
	fPyramid[21] = -3.0f; fPyramid[22] = 0.0f; fPyramid[23] = -3.0f;
	fPyramid[24] = -3.0f; fPyramid[25] = 0.0f; fPyramid[26] = 3.0f;

	// Right face
	fPyramid[27] = 0.0f; fPyramid[28] = 5.0f; fPyramid[29] = 0.0f;
	fPyramid[30] = 3.0f; fPyramid[31] = 0.0f; fPyramid[32] = 3.0f;
	fPyramid[33] = 3.0f; fPyramid[34] = 0.0f; fPyramid[35] = -3.0f;

	// Setup pyramid color

	for(int i=0;i<4;i++)
	{
		fPyramidColor[i*9] = 1.0f; fPyramidColor[i*9+1] = 0.0f; fPyramidColor[i*9+2] = 0.0f;
		if(i < 2)
		{
			fPyramidColor[i*9+1] = 0.0f; fPyramidColor[i*9+4] = 1.0f; fPyramidColor[i*9+5] = 0.0f;
			fPyramidColor[i*9+2] = 0.0f; fPyramidColor[i*9+7] = 0.0f; fPyramidColor[i*9+8] = 1.0f;
		}
		else
		{
			fPyramidColor[i*9+2] = 0.0f; fPyramidColor[i*9+7] = 1.0f; fPyramidColor[i*9+8] = 0.0f;
			fPyramidColor[i*9+1] = 0.0f; fPyramidColor[i*9+4] = 0.0f; fPyramidColor[i*9+5] = 1.0f;
		}
	}
 
	glGenVertexArrays(1, VaoId);
	glGenBuffers(2, BufferId);

	// Setup whole pyramid
	glBindVertexArray(VaoId[0]);

	glBindBuffer(GL_ARRAY_BUFFER, BufferId[0]);
	glBufferData(GL_ARRAY_BUFFER, 36*sizeof(float), fPyramid, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, BufferId[1]);
	glBufferData(GL_ARRAY_BUFFER, 36*sizeof(float), fPyramidColor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//warning, possible double declaration
	GLenum	ErrorCheckValue = glGetError();
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
		glDeleteBuffers(4, &BufferId[0]);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &IndexBufferId[0]);

		glBindVertexArray(0);
		glDeleteVertexArrays(2, &VaoId[0]);

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
