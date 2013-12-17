/*
 * Cuboid.h
 *
 *  Created on: Dec 8, 2013
 *      Author: kevin
 */

#ifndef CUBOID_H_
#define CUBOID_H_

typedef float geometry_type;

#include <vector>
#include "GeometricObject.h"

class Cuboid : public GeometricObject {
public:
	Cuboid(geometry_type a_, geometry_type b_, geometry_type c_);
	virtual ~Cuboid();

	std::vector<geometry_type> getVertices();
	std::vector<geometry_type> getNormals();
	std::vector<unsigned int> getIndices();
	std::vector<geometry_type> getColors();

	unsigned int getNumElements();

private:
	geometry_type a;// side length in x direction
	geometry_type b;// side length in y direction
	geometry_type c;// side length in z direction
	unsigned int numElements;

	inline geometry_type distance(geometry_type a1, geometry_type a2, geometry_type a3);//length of a vector (a1, a2, a3)

};

#endif /* CUBOID_H_ */
