/*
 * ViewerShader.h
 *
 *  Created on: Nov 10, 2013
 *      Author: kevin
 */

#ifndef EXAMPLEBALL_H_
#define EXAMPLEBALL_H_

#include "../src/GlutFramework.h"
#include "../src/ShaderLoader.h"
#include "../src/RotatingView.h"

typedef struct
{
	float XYZW[4];
	float RGBA[4];
} Vertex;


class ExampleBall : public glutFramework::GlutFramework{
public:
	ExampleBall();
	~ExampleBall();

	void display(float dTime);
	void load();
	void close();
private:
	std::vector<GLuint> vaoId, bufferId, indexBufferId;
	std::vector<GLint> locs;
	unsigned int numElements;

	ShaderLoader sh;
	RotatingView* rv;
	void createVBO();
	void destroyVBO();
	void specialKeyboardDown(int key, int x, int y );

};



#endif /* VIEWERSHADER_H_ */
