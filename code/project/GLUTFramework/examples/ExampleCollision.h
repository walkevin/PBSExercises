/*
 * ViewerShader.h
 *
 *  Created on: Nov 10, 2013
 *      Author: kevin
 */

#ifndef EXAMPLECOLLISION_H_
#define EXAMPLECOLLISION_H_

#include <Eigen/Core>
#include <array>
#include <map>
#include <string>

#include "../src/GlutFramework.h"

//Forward declarations?
#include "../src/ShaderLoader.h"
#include "../src/RotatingView.h"
#include "../../Collision/CollisionHandler.h"
#include "../src/GeometricObject.h"

struct objectInfo{
	objectInfo(int numElements_, int numInstances_) : numElements(numElements_), numInstances(numInstances_){}
	int numElements;
	int numInstances;
};

class ExampleCollision : public glutFramework::GlutFramework{
public:
	ExampleCollision();
	~ExampleCollision();

	void display(float dTime);
	void load();
	void close();
private:
	std::vector<Eigen::Matrix<float, 4, 1> > pos;
	std::vector<Eigen::Matrix<float, 4, 1> > vel;

	int n_points = 20;

	int nVao;
	int nBuffer;
	int nIndexBuffer;

	GLuint *vaoId, *bufferId, *indexBufferId;
	std::map<std::string, GLint> locs;

	std::vector<objectInfo> objInfo;

	ShaderLoader sh;
	RotatingView* rv;
	CollisionHandlerNS::CollisionHandler ch;

	void createVBO();
	void destroyVBO();
	void updatePositions();
	void specialKeyboardDown(int key, int x, int y );

	void uploadGeometricObject(GeometricObject* obj, int numObj, std::vector<glm::mat4> objTransforms, int bufferStride);

};



#endif /* EXAMPLECOLLISION_H_ */
