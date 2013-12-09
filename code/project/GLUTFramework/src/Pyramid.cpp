/*
 * Pyramid.cpp
 *
 *  Created on: Dec 8, 2013
 *      Author: kevin
 *
 *
 *  Example
 *
 *  	Pyramid p(a, b, h);
		std::vector<geometry_type> vertices = p.getVertices();
		std::vector<GLuint> indices = p.getIndices();
		numElements = p.getNumElements();

		Displayfunction

		glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, NULL);

 */

#include "Pyramid.h"
#include <vector>
#include <cmath>

Pyramid::Pyramid(geometry_type a_, geometry_type b_, geometry_type h_) : a(a_), b(b_), h(h_), numElements(3 * 6){}

Pyramid::~Pyramid() {}

/* Vertex numbering of pyramid
 *      4(summit)
 *    1     2
 *
 * 0     3
 *
 */


std::vector<geometry_type> Pyramid::getVertices(){
	//Specify vertices for sphere (displaced latitude-longitude discretization)
	std::vector<geometry_type> vertices;

	//Base
	vertices.push_back(-0.5 * a); vertices.push_back(-0.5 * h); vertices.push_back(-0.5 * b); vertices.push_back(1.0f);
	vertices.push_back( 0.5 * a); vertices.push_back(-0.5 * h); vertices.push_back(-0.5 * b); vertices.push_back(1.0f);
	vertices.push_back( 0.5 * a); vertices.push_back(-0.5 * h); vertices.push_back( 0.5 * b); vertices.push_back(1.0f);
	vertices.push_back(-0.5 * a); vertices.push_back(-0.5 * h); vertices.push_back( 0.5 * b); vertices.push_back(1.0f);

	//top
	vertices.push_back(0.0); vertices.push_back(0.5 * h); vertices.push_back(0.0); vertices.push_back(1.0f);

	return vertices;
}

std::vector<geometry_type> Pyramid::getNormals(){
	std::vector<geometry_type> normals;
	geometry_type tmpdist;
	tmpdist =distance(-0.5 * a, -0.5 * h, -0.5 * b);
	normals.push_back(-0.5 * a / tmpdist); normals.push_back(-0.5 * h / tmpdist); normals.push_back(-0.5 * b / tmpdist);
	tmpdist =distance( 0.5 * a, -0.5 * h, -0.5 * b);
	normals.push_back( 0.5 * a / tmpdist); normals.push_back(-0.5 * h / tmpdist); normals.push_back(-0.5 * b / tmpdist);
	tmpdist =distance( 0.5 * a, -0.5 * h,  0.5 * b);
	normals.push_back( 0.5 * a / tmpdist); normals.push_back(-0.5 * h / tmpdist); normals.push_back( 0.5 * b / tmpdist);
	tmpdist =distance(-0.5 * a, -0.5 * h,  0.5 * b);
	normals.push_back(-0.5 * a / tmpdist); normals.push_back(-0.5 * h / tmpdist); normals.push_back( 0.5 * b / tmpdist);
	//Summit
	normals.push_back(0.0f); normals.push_back(1.0f); normals.push_back(0.0f);
	return normals;
}


std::vector<unsigned int> Pyramid::getIndices(){
	//Specify indices
	std::vector<unsigned int> indices;

	//Base
	indices.push_back(0); indices.push_back(1); indices.push_back(2);
	indices.push_back(0); indices.push_back(2); indices.push_back(3);
	//Side areas
	indices.push_back(0); indices.push_back(1); indices.push_back(4);
	indices.push_back(1); indices.push_back(2); indices.push_back(4);
	indices.push_back(2); indices.push_back(3); indices.push_back(4);
	indices.push_back(3); indices.push_back(0); indices.push_back(4);

	numElements = indices.size();

	return indices;
}

unsigned int Pyramid::getNumElements(){
	return numElements;
}

inline geometry_type Pyramid::distance(geometry_type a1, geometry_type a2, geometry_type a3){//length of a vector (a1, a2, a3)
	return std::sqrt(a1*a1 + a2*a2 + a3*a3);
}

