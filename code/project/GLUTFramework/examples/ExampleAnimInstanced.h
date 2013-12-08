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
#include <map>
#include <string>

#include "../src/GlutFramework.h"
#include "../src/ShaderLoader.h"
#include "../src/RotatingView.h"

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

	static const int nVao = 1;
	static const int nBuffer = 3;
	static const int nIndexBuffer = 1;

	GLuint vaoId[nVao], bufferId[nBuffer], indexBufferId[nIndexBuffer];
	std::map<std::string, GLint> locs;
	unsigned int numElements;

	ShaderLoader sh;
	RotatingView* rv;

	void createVBO();
	void destroyVBO();
	void updatePositions();
	void specialKeyboardDown(int key, int x, int y );

};



#endif /* EXAMPLEANIMINSTANCED_H_ */
