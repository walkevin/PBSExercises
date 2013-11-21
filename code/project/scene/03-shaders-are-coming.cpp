#include "03-shaders-are-coming.h"
#include <iostream>
	ShadersComing::ShadersComing(){}
	ShadersComing::~ShadersComing(){}

	void ShadersComing::load(){
		sh.loadAndUse("shaders/03-shader.vert","shaders/03-shader.frag");
		createVBO();
	}
	void ShadersComing::display(float dTime){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, 48, GL_UNSIGNED_BYTE, NULL);

		glutSwapBuffers();
		glutPostRedisplay();
	}

	void ShadersComing::close(){
		std::cout << "ShadersComing::close" << std::endl;
		sh.destroy();
		destroyVBO();
	}
	void ShadersComing::createVBO(){
			float fTriangle[9]; // Data to render triangle (3 vertices, each has 3 floats)
			float fQuad[12]; // Data to render quad using triangle strips (4 vertices, each has 3 floats)
			float fTriangleColor[9];
			float fQuadColor[12];

			GLuint uiVBO[4];
			GLuint uiVAO[2];

			fTriangle[0] = -0.4f; fTriangle[1] = 0.1f; fTriangle[2] = 0.0f;
			fTriangle[3] = 0.4f; fTriangle[4] = 0.1f; fTriangle[5] = 0.0f;
			fTriangle[6] = 0.0f; fTriangle[7] = 0.7f; fTriangle[8] = 0.0f;

			// Setup triangle color

			fTriangleColor[0] = 1.0f; fTriangleColor[1] = 0.0f; fTriangleColor[2] = 0.0f;
			fTriangleColor[3] = 0.0f; fTriangleColor[4] = 1.0f; fTriangleColor[5] = 0.0f;
			fTriangleColor[6] = 0.0f; fTriangleColor[7] = 0.0f; fTriangleColor[8] = 1.0f;
		 
			// Setup quad vertices
		 
			fQuad[0] = -0.2f; fQuad[1] = -0.1f; fQuad[2] = 0.0f;
			fQuad[3] = -0.2f; fQuad[4] = -0.6f; fQuad[5] = 0.0f;
			fQuad[6] = 0.2f; fQuad[7] = -0.1f; fQuad[8] = 0.0f;
			fQuad[9] = 0.2f; fQuad[10] = -0.6f; fQuad[11] = 0.0f;

			// Setup quad color

			fQuadColor[0] = 1.0f; fQuadColor[1] = 0.0f; fQuadColor[2] = 0.0f;
			fQuadColor[3] = 0.0f; fQuadColor[4] = 1.0f; fQuadColor[8] = 0.0f;
			fQuadColor[6] = 0.0f; fQuadColor[7] = 0.0f; fQuadColor[5] = 1.0f;
			fQuadColor[9] = 1.0f; fQuadColor[10] = 1.0f; fQuadColor[11] = 0.0f;


			GLenum ErrorCheckValue = glGetError();
			const size_t BufferSize = sizeof(fTriangle)+sizeof(fTriangleColor)+sizeof(fQuad)+sizeof(fQuadColor);
			//const size_t VertexSize = sizeof(fTriangle[0]);
			//const size_t RgbOffset = sizeof(fTriangle[0].XYZW);

	glGenVertexArrays(2, uiVAO); // Generate two VAOs, one for triangle and one for quad
	glGenBuffers(4, uiVBO); // And four VBOs

	// Setup whole triangle
	glBindVertexArray(uiVAO[0]);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), fTriangle, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), fTriangleColor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Setup whole quad
	glBindVertexArray(uiVAO[1]);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[2]);
	glBufferData(GL_ARRAY_BUFFER, 12*sizeof(float), fQuad, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[3]);
	glBufferData(GL_ARRAY_BUFFER, 12*sizeof(float), fQuadColor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

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
	void ShadersComing::destroyVBO(){
		GLuint uiVBO[4];
		GLuint uiVAO[2];
		GLenum ErrorCheckValue = glGetError();

		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(4, uiVBO);

		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(uiVAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(uiVAO[1]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		//glDeleteBuffers(1, &IndexBufferId);

		glBindVertexArray(0);
		glDeleteVertexArrays(2, uiVAO);

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
