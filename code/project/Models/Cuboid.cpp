/*
 * Cuboid.cpp
 *
 *  Created on: Dec 8, 2013
 *      Author: kevin
 *
 *
 *  Example
 *
 *  	Cuboid cub(a, b, c);
		std::vector<geometry_type> vertices = c.getVertices();
		std::vector<GLuint> indices = c.getIndices();
		numElements = c.getNumElements();

		Displayfunction

		glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, NULL);

 */
//TODO: Move computation of vertices and indices into constructor, create private variables vertices and indices

#include "Cuboid.h"
#include <vector>
#include <cmath>

Cuboid::Cuboid(geometry_type a_, geometry_type b_, geometry_type c_, Eigen::Vector4f bodyColor_)
: a(a_), b(b_), c(c_), numElements(3 * 12), bodyColor(bodyColor_){}

Cuboid::~Cuboid() {}

/* Vertex numbering of cuboid
 * Top:
 *    5     6
 *
 * 4     7
 * Bottom:
 *    1     2
 *
 * 0     3
 *
 */


std::vector<geometry_type> Cuboid::getVertices(){
	//Specify vertices for sphere (displaced latitude-longitude discretization)
	std::vector<geometry_type> vertices;

	//Bottom
	vertices.push_back(-0.5 * a); vertices.push_back(-0.5 * b); vertices.push_back(-0.5 * c); vertices.push_back(1.0f);
	vertices.push_back( 0.5 * a); vertices.push_back(-0.5 * b); vertices.push_back(-0.5 * c); vertices.push_back(1.0f);
	vertices.push_back( 0.5 * a); vertices.push_back(-0.5 * b); vertices.push_back( 0.5 * c); vertices.push_back(1.0f);
	vertices.push_back(-0.5 * a); vertices.push_back(-0.5 * b); vertices.push_back( 0.5 * c); vertices.push_back(1.0f);

	//Top
	vertices.push_back(-0.5 * a); vertices.push_back( 0.5 * b); vertices.push_back(-0.5 * c); vertices.push_back(1.0f);
	vertices.push_back( 0.5 * a); vertices.push_back( 0.5 * b); vertices.push_back(-0.5 * c); vertices.push_back(1.0f);
	vertices.push_back( 0.5 * a); vertices.push_back( 0.5 * b); vertices.push_back( 0.5 * c); vertices.push_back(1.0f);
	vertices.push_back(-0.5 * a); vertices.push_back( 0.5 * b); vertices.push_back( 0.5 * c); vertices.push_back(1.0f);

	return vertices;
}

std::vector<geometry_type> Cuboid::getNormals(){
	std::vector<geometry_type> normals;
	geometry_type tmpdist;

	//Bottom
	tmpdist =distance(-0.5 * a, -0.5 * b, -0.5 * c);
	normals.push_back(-0.5 * a / tmpdist); normals.push_back(-0.5 * b / tmpdist); normals.push_back(-0.5 * c / tmpdist);
	tmpdist =distance( 0.5 * a, -0.5 * b, -0.5 * c);
	normals.push_back( 0.5 * a / tmpdist); normals.push_back(-0.5 * b / tmpdist); normals.push_back(-0.5 * c / tmpdist);
	tmpdist =distance( 0.5 * a, -0.5 * b,  0.5 * c);
	normals.push_back( 0.5 * a / tmpdist); normals.push_back(-0.5 * b / tmpdist); normals.push_back( 0.5 * c / tmpdist);
	tmpdist =distance(-0.5 * a, -0.5 * b,  0.5 * c);
	normals.push_back(-0.5 * a / tmpdist); normals.push_back(-0.5 * b / tmpdist); normals.push_back( 0.5 * c / tmpdist);

	//Bottom
	tmpdist =distance(-0.5 * a,  0.5 * b, -0.5 * c);
	normals.push_back(-0.5 * a / tmpdist); normals.push_back( 0.5 * b / tmpdist); normals.push_back(-0.5 * c / tmpdist);
	tmpdist =distance( 0.5 * a,  0.5 * b, -0.5 * c);
	normals.push_back( 0.5 * a / tmpdist); normals.push_back( 0.5 * b / tmpdist); normals.push_back(-0.5 * c / tmpdist);
	tmpdist =distance( 0.5 * a,  0.5 * b,  0.5 * c);
	normals.push_back( 0.5 * a / tmpdist); normals.push_back( 0.5 * b / tmpdist); normals.push_back( 0.5 * c / tmpdist);
	tmpdist =distance(-0.5 * a,  0.5 * b,  0.5 * c);
	normals.push_back(-0.5 * a / tmpdist); normals.push_back( 0.5 * b / tmpdist); normals.push_back( 0.5 * c / tmpdist);
	return normals;
}


std::vector<unsigned int> Cuboid::getIndices(){
	//Specify indices
	std::vector<unsigned int> indices;

	//Bottom plane (y = -0.5 * c)
	indices.push_back(0); indices.push_back(2); indices.push_back(1);
	indices.push_back(0); indices.push_back(3); indices.push_back(2);

	//Top plane (y = 0.5 * c)
	indices.push_back(4); indices.push_back(6); indices.push_back(5);
	indices.push_back(4); indices.push_back(7); indices.push_back(6);

	//Right plane (x = 0.5 * a)
	indices.push_back(3); indices.push_back(6); indices.push_back(7);
	indices.push_back(3); indices.push_back(2); indices.push_back(6);

	//Left plane (x = -0.5 * a)
	indices.push_back(0); indices.push_back(1); indices.push_back(5);
	indices.push_back(0); indices.push_back(5); indices.push_back(4);

	//Front plane (z = 0.5 * b)
	indices.push_back(0); indices.push_back(3); indices.push_back(7);
	indices.push_back(0); indices.push_back(7); indices.push_back(4);

	//Back plane (z = -0.5 * b)
	indices.push_back(1); indices.push_back(2); indices.push_back(6);
	indices.push_back(1); indices.push_back(6); indices.push_back(5);

	numElements = indices.size();

	return indices;
}

std::vector<geometry_type> Cuboid::getColors(){
	std::vector<geometry_type> colors;

	for(int i = 0; i < 8; i++){
		colors.push_back(bodyColor[0]);
		colors.push_back(bodyColor[1]);
		colors.push_back(bodyColor[2]);
		colors.push_back(bodyColor[3]);
	}
//	colors.push_back(0.0); colors.push_back(0.1); colors.push_back(0.2); colors.push_back(1.0);
	return colors;
}


unsigned int Cuboid::getNumElements(){
	return numElements;
}

inline geometry_type Cuboid::distance(geometry_type a1, geometry_type a2, geometry_type a3){//length of a vector (a1, a2, a3)
	return std::sqrt(a1*a1 + a2*a2 + a3*a3);
}

