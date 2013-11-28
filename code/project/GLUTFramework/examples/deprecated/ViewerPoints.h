/*
 * ViewerPoints.h
 *
 *  Created on: Nov 10, 2013
 *      Author: kevin
 */

#ifndef VIEWERPOINTS_H_
#define VIEWERPOINTS_H_

#include "../src/GlutFramework.h"
#include <Eigen/Core>

typedef struct
{
	float XYZW[4];
	float RGBA[4];
} Vertex;


class ViewerPoints : public glutFramework::GlutFramework{
public:
	ViewerPoints();
	~ViewerPoints();

	void display(float dTime);
	void load();
	void close();
private:
	std::vector<Eigen::Array<double, 4, 1> > pos;
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

	GLuint createShaderObject(const std::string &vertex_file, const std::string &fragment_file);
};



#endif /* VIEWERPOINTS_H_*/
