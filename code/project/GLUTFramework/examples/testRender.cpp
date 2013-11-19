////////////////////
//
// testRender.cpp
//
////////////////////

#include <iostream>
using namespace std;

#include <GL/glew.h>
#include <GL/freeglut.h>
#include "../src/ShaderLoader.h"

#define BUFFER_OFFSET(x)  ((const void*) (x))

enum VAO_IDs { Triangles, NumVAOs};
enum Buffer_IDs { ArrayBuffer, NumBuffers};
enum Attrib_IDs { vPosition = 0 };

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

const GLuint NumVertices = 6;

float fTriangle[9]; // Data to render triangle (3 vertices, each has 3 floats)
float fQuad[12]; // Data to render quad using triangle strips (4 vertices, each has 3 floats)
GLuint uiVBO[2];

//---------------------------------
////
//// init
////


void init()
{
	glGenVertexArrays(NumVAOs, VAOs);
	glBindVertexArray(VAOs[Triangles]);

	glClearColor(0.0f, 0.5f, 1.0f, 1.0f);

	// Setup triangle vertices
	fTriangle[0] = -0.4f; fTriangle[1] = 0.1f; fTriangle[2] = 0.0f;
	fTriangle[3] = 0.4f; fTriangle[4] = 0.1f; fTriangle[5] = 0.0f;
	fTriangle[6] = 0.0f; fTriangle[7] = 0.7f; fTriangle[8] = 0.0f;
 
	// Setup quad vertices
 
	fQuad[0] = -0.2f; fQuad[1] = -0.1f; fQuad[2] = 0.0f;
	fQuad[3] = -0.2f; fQuad[4] = -0.6f; fQuad[5] = 0.0f;
	fQuad[6] = 0.2f; fQuad[7] = -0.1f; fQuad[8] = 0.0f;
	fQuad[9] = 0.2f; fQuad[10] = -0.6f; fQuad[11] = 0.0f;

//	GLfloat vertices[NumVertices][2] = {
//		{ -0.90, -0.90 }, // Triangle 1
//		{ 0.85, -0.90},
//		{ -0.90, 0.85 },
//		{ 0.90, -0.85 }, // Triangle 2
//		{ 0.90, 0.90 },
//		{ -0.85, 0.90 }
//	};

	glGenBuffers(2, uiVBO);
	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float),
			fTriangle, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, 12*sizeof(float),
			fTriangle, GL_STATIC_DRAW);

	ShaderLoader sh;
	sh.loadAndUse("shaders/triangle.glslv","shaders/triangle.glslf");


	glVertexAttribPointer(vPosition, 2, GL_FLOAT,
			GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(vPosition);
}

//---------------------------------
//
// display
//

void
display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(VAOs[Triangles]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);

	glFlush();
}

//------------------------
//
// main
//

int
main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(512, 512);
	glutInitContextVersion(3, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitContextProfile(GLUT_FORWARD_COMPATIBLE);
	//glutInitCreateWindow(argv[0]);
	glutCreateWindow("irgendöppis");

	if (glewInit()) {
		cerr << "Unable to initialize GLEW ... exiting" << endl;
		exit(EXIT_FAILURE);
	}

	init();

	glutDisplayFunc(display);

	glutMainLoop();
}
