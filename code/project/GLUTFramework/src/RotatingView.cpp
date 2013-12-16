/*
 * RotatingView.cpp
 *
 *  Created on: Nov 25, 2013
 *      Author: kevin
 */

#include "RotatingView.h"


RotatingView::RotatingView(ShaderLoader sh_, std::string matrixName_)
: sh(sh_)
, PVMatrixName(matrixName_)
, distance(6.)
, azimuth(M_PI * 0.5)
, polar(M_PI * 0.5)
, angleChangeSpeed(M_PI * 0.1)
, zoomSpeed(0.3)
{}


RotatingView::~RotatingView() {}

glm::mat4 RotatingView::getProjectionViewMatrix(value_t azimuth_, value_t polar_, value_t distance_){
	float aspectRatio = glutGet( GLUT_WINDOW_WIDTH ) / glutGet( GLUT_WINDOW_HEIGHT );
	return glm::perspective(60.0f, aspectRatio, 1.0f, 500.0f) *
			glm::lookAt(
					glm::vec3(std::cos(azimuth_) * std::sin(polar_) * distance_, std::cos(polar_) * distance_, std::sin(azimuth_) * std::sin(polar_) * distance_),
					glm::vec3(0.0f, 0.0f, 0.0f),
					glm::vec3(0.0f, 1.0f, 0.0f)
					);
}

void RotatingView::setAzimuth(value_t azimuth_){
	azimuth = azimuth_;
}
void RotatingView::setPolar(value_t polar_){
	polar = polar_;
}
void RotatingView::setDistance(value_t distance_){
	distance = distance_;
}

void RotatingView::updateView(){
	glm::mat4 projview_mat = getProjectionViewMatrix(azimuth, polar, distance);
	GLint location = glGetUniformLocation(sh.getProgramId(), PVMatrixName.c_str());
	sh.use();
	glUniformMatrix4fv(location, 1, false, glm::value_ptr(projview_mat));

}

void RotatingView::keyboardEvent(int key)
{
	// Subclass and override this method
	//printf( "KeyboardDown: %c = %d\n", key, (int)key );
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
			updateView();
			glutPostRedisplay();
			break;
	}
}
