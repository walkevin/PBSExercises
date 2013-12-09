/*
 * Pyramid.h
 *
 *  Created on: Nov 23, 2013
 *      Author: kevin
 */

#ifndef PYRAMID_H_
#define PYRAMID_H_

typedef float geometry_type;

#include <vector>
#include "GeometricObject.h"

class Pyramid : public GeometricObject {
public:
	Pyramid(geometry_type a_, geometry_type b_, geometry_type h_);
	virtual ~Pyramid();

	std::vector<geometry_type> getVertices();
	std::vector<geometry_type> getNormals();
	std::vector<unsigned int> getIndices();

	unsigned int getNumElements();

private:
	geometry_type a;// side length of base
	geometry_type b;// side length of base
	geometry_type h;// height of pyramid
	unsigned int numElements;

	inline geometry_type distance(geometry_type a1, geometry_type a2, geometry_type a3);//length of a vector (a1, a2, a3)
};

#endif /* PYRAMID_H_ */
