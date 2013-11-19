////////////////////
//
// triangles.cpp
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

//---------------------------------
////
//// init
////

void init()
{
	glGenVertexArrays(NumVAOs, VAOs);
	glBindVertexArray(VAOs[Triangles]);

	GLfloat vertices[NumVertices][2] = {
		{ -0.90, -0.90 }, // Triangle 1
		{ 0.85, -0.90},
		{ -0.90, 0.85 },
		{ 0.90, -0.85 }, // Triangle 2
		{ 0.90, 0.90 },
		{ -0.85, 0.90 }
	};

	glGenBuffers(NumBuffers, Buffers);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
			vertices, GL_STATIC_DRAW);

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
