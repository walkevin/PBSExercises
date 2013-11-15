
#include "ViewerTeapot.h"
	void ViewerTeapot::display(float dTime){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		static int frame = 0;
		std::cout << "GlutFramework Display: Frame: " << frame << ", dt(sec): " << dTime << ", Position: " << position << std::endl;
		++frame;

		// DEMO: Create a teapot and move it back and forth on the x-axis
		glTranslatef(position, 0.0f, 0.0f);
		glutWireTeapot(2.5);
		if(position > 4 && direction > 0) {
			direction = -1.0 / FRAME_TIME;
		} else if(position < -4 && direction < 0) {
			direction = 1.0 / FRAME_TIME;
		}
		position += direction;

		glutSwapBuffers();
		glutPostRedisplay();
	}
