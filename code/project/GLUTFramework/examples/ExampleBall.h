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
	GLuint
	   VaoId,
	   BufferId,
	   IndexBufferId;

	ShaderLoader sh;
	void createVBO();
	void destroyVBO();
	unsigned int numElements;
};



#endif /* VIEWERSHADER_H_ */
