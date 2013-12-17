/*
 * OBJModel.h
 *
 *  Created on: Nov 23, 2013
 *      Author: kevin
 */

#ifndef OBJMODEL_H_
#define OBJMODEL_H_

typedef float geometry_type;

#include <vector>
#include <Eigen/Core>
#include "GeometricObject.h"

class OBJModel : public GeometricObject {
public:
	OBJModel(const char* filename_, Eigen::Vector4f bodyColor_);
	virtual ~OBJModel();

	std::vector<geometry_type> getVertices();
	std::vector<geometry_type> getNormals();
	std::vector<unsigned int> getIndices();
	std::vector<geometry_type> getColors();

	unsigned int getNumElements();

private:
	void loadObj(const char* filename);

	Eigen::Vector4f bodyColor;
	std::vector<geometry_type> vertices;
	std::vector<geometry_type> normals;
	std::vector<unsigned int> indices;
	std::vector<geometry_type> colors;
	unsigned int numElements;
};

#endif /* OBJMODEL_H_ */
