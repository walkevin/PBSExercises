//------------------------
//
// main
//
#include "common-header.h"
extern void init();
extern void display();

int main(int argc, char** argv)
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
