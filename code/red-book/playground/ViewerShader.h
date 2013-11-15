/*
 * ViewerShader.h
 *
 *  Created on: Nov 10, 2013
 *      Author: kevin
 */

#ifndef VIEWERSHADER_H_
#define VIEWERSHADER_H_

//Import glew before OpenGL
#include <GL/glew.h>
#include "GlutFramework.h"

typedef struct
{
	float XYZW[4];
	float RGBA[4];
} Vertex;


class ViewerShader : public glutFramework::GlutFramework{
public:
	ViewerShader();
	~ViewerShader();

	void display(float dTime);
	void load();
	void close();
private:
	GLuint
	   VertexShaderId,
	   FragmentShaderId,
	   ProgramId,
	   VaoId,
	   BufferId,
	   IndexBufferId;

	void createVBO();
	void destroyVBO();
	void createShaders();
	void destroyShaders();
};



#endif /* VIEWERSHADER_H_ */
