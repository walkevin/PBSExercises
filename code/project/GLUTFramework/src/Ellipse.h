/*
 * Ellipse.h
 *
 *  Created on: Dec 8, 2013
 *      Author: kevin
 */

#ifndef ELLIPSE_H_
#define ELLIPSE_H_


#include <vector>
#include <Eigen/Core>
#include "GeometricObject.h"

typedef Eigen::Matrix<geometry_type, 3, 1> vec3;

class Ellipse : public GeometricObject {
public:
	/* Ellipse
	 * Returns vertices for a (2D) ellipse in 3D space.
	 * A: outermost position on long axis
	 * B: outermost position on short axis
	 * center: center of the ellipse
	 * t: Number of vertices on the boundary => total number of vertices = t + 1
	 *
	 */
	Ellipse(vec3 A_, vec3 B_, vec3 center_, int t_);
	virtual ~Ellipse();

	std::vector<geometry_type> getVertices();
	std::vector<geometry_type> getNormals();
	std::vector<unsigned int> getIndices();
	std::vector<geometry_type> getColors();

	unsigned int getNumElements();

private:
	void computeVertices();
	void computeNormals();
	void computeIndices();
	void computeColors();

	vec3 A;// side length in x direction
	vec3 B;// side length in y direction
	vec3 center;// side length in z direction
	int t; //number of vertices on the boundary
	unsigned int numElements;

	std::vector<geometry_type> vertices;
	std::vector<geometry_type> normals;
	std::vector<unsigned int> indices;
	std::vector<geometry_type> colors;
};

#endif /* Ellipse_H_ */
