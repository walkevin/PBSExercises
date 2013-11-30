/*
 * ViewerShader.h
 *
 *  Created on: Nov 10, 2013
 *      Author: kevin
 */

#ifndef EXAMPLEANIMINSTANCED_H_
#define EXAMPLEANIMINSTANCED_H_

#include <Eigen/Core>
#include <array>

#include "../src/GlutFramework.h"
#include "../src/ShaderLoader.h"
#include "../src/RotatingView.h"

#include "../../SPH/SphSolver.hpp"

typedef struct
{
	float XYZW[4];
	float RGBA[4];
} Vertex;


class ExampleAnimInstanced : public glutFramework::GlutFramework{
public:
	ExampleAnimInstanced(sph::SphSolver*);
	~ExampleAnimInstanced();

	void display(float dTime);
	void load();
	void close();
private:
	std::vector<Eigen::Array<float, 4, 1> > pos;
	static const int n_points = 125;

	sph::SphSolver* solver;

	std::vector<GLuint> vaoId, bufferId, indexBufferId;
	std::vector<GLint> locs;
	unsigned int numElements;

	ShaderLoader sh;
	RotatingView* rv;

	void createVBO();
	void destroyVBO();
	void updatePositions();
	void specialKeyboardDown(int key, int x, int y );

};



#endif /* EXAMPLEANIMINSTANCED_H_ */
