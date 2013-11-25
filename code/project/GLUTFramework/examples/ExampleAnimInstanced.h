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


typedef struct
{
	float XYZW[4];
	float RGBA[4];
} Vertex;


class ExampleAnimInstanced : public glutFramework::GlutFramework{
public:
	ExampleAnimInstanced();
	~ExampleAnimInstanced();

	void display(float dTime);
	void load();
	void close();
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
};



#endif /* EXAMPLEANIMINSTANCED_H_ */
