/*
 * ViewerShader.h
 *
 *  Created on: Nov 10, 2013
 *      Author: kevin
 */

#ifndef EXAMPLEANIM_H_
#define EXAMPLEANIM_H_

#include <Eigen/Core>
#include <array>

#include "../src/GlutFramework.h"
#include "../src/ShaderLoader.h"


typedef struct
{
	float XYZW[4];
	float RGBA[4];
} Vertex;


class ExampleAnim : public glutFramework::GlutFramework{
public:
	ExampleAnim();
	~ExampleAnim();

	void display(float dTime);
	void load();
	void close();
	void specialKeyboardDown(int key, int x, int y );
private:
	std::vector<Eigen::Array<float, 4, 1> > pos;
	static const int n_points = 20;
	GLuint
	   VaoId,
	   BufferId,
	   IndexBufferId;

	ShaderLoader sh;
	void createVBO();
	void destroyVBO();
	void updatePositions();

	double viewAzimuthAngle;//Note: Since y-coord is up, azimuth is angle between x and z
	double viewPolarAngle;//All angles in radians
	double viewDistance;
	double angleChangeSpeed;
};



#endif /* EXAMPLEANIM_H_ */
