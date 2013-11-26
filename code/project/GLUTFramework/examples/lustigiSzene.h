/*
 * ViewerShader.h
 *
 *  Created on: Nov 10, 2013
 *      Author: kevin
 *      		//blakommentar
 *
 */

#ifndef ROTATINGCAMERA_H_
#define ROTATINGCAMERA_H_

#include "../src/GlutFramework.h"
#include "../src/ShaderLoader.h"
#include "vmath.h"

typedef struct
{
	float XYZW[4];
	float RGBA[4];
} Vertex;


class RotatingCamera : public glutFramework::GlutFramework{
public:
	RotatingCamera();
	~RotatingCamera();

	void display(float dTime);
	void load();
	void close();
	void specialKeyboardDown( int key, int x, int y);

private:
	GLuint
	   VaoId[2],
	   BufferId[4],
	   IndexBufferId[1];

	ShaderLoader foreground_sh;
	void createVBO();
	void destroyVBO();

	vmath::mat4 getProjectionViewMatrix(double azimuth, double polar, double distance);
	double azimuth;//Note: Since y-coord is up, azimuth is angle between x and z
	double polar;//All angles in radians
	double distance;
	double zoomSpeed;
	double angleChangeSpeed;
};



#endif /* ROTATINGCAMERA_H_ */
