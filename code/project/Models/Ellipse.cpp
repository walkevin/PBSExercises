/*
 * Ellipse.cpp
 *
 *  Created on: Dec 8, 2013
 *      Author: kevin
 *
 *
 *  Example
 *
 *  	Ellipse cub(a, b, c);
		std::vector<geometry_type> vertices = c.getVertices();
		std::vector<GLuint> indices = c.getIndices();
		numElements = c.getNumElements();

		Displayfunction

		glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, NULL);

 */
//TODO: Move computation of vertices and indices into constructor, create private variables vertices and indices

#include "Ellipse.h"
#include <vector>
#include <cmath>
#include <cassert>
#include <Eigen/Geometry>



Ellipse::Ellipse(vec3 A_, vec3 B_, vec3 center_, int t_) : A(A_), B(B_), center(center_), numElements(3 * t_), t(t_), numEllipses(1){
	computeVertices();
	computeNormals();
	computeIndices();
	computeColors();
}

Ellipse::Ellipse(std::vector<vec3> A_, std::vector<vec3> B_, std::vector<vec3> center_, int t_) : t(t_), numEllipses(A_.size()){
	for(int i = 0; i < A_.size(); i++){
		A = A_[i];
		B = B_[i];
		center = center_[i];

		computeVertices();
		computeNormals();
		computeIndices();
		computeColors();
	}

}
Ellipse::~Ellipse() {}

/* Vertex numbering of Ellipse
 *    3     2
 *
 * 4     0     1
 *
 *    5     6
 *
 */


void Ellipse::computeVertices(){
	//Specify vertices for sphere (displaced latitude-longitude discretization)

	vertices.push_back(center[0]); vertices.push_back(center[1]); vertices.push_back(center[2]); vertices.push_back(1.0f);

	for(int i = 0; i < t; i++){
		float angle = i * 2 * M_PI / t;
		vertices.push_back(A[0] * std::cos(angle) + B[0] * std::sin(angle) + center[0]);
		vertices.push_back(A[1] * std::cos(angle) + B[1] * std::sin(angle) + center[1]);
		vertices.push_back(A[2] * std::cos(angle) + B[2] * std::sin(angle) + center[2]);
		vertices.push_back(1.0f);
	}

}
void Ellipse::computeNormals(){
	assert((vertices.size() / 4) % (t + 1) == 0); //Make sure that vertices have already been computed

	vec3 normal = (A-center).cross((vec3(vertices[3], vertices[4], vertices[5])) - center).normalized();
	for(int i = 0; i < t+1; i++){
		normals.push_back(normal[0]); normals.push_back(normal[1]); normals.push_back(normal[2]); normals.push_back(1.0);
	}
}


void Ellipse::computeIndices(){
	//Specify indices
	assert((vertices.size() / 4) % (t + 1) == 0); //Make sure that vertices have already been computed
	int offset = indices.size() / (3*t) * (t+1);
	for(int i = 1; i < t; i++){
		indices.push_back(offset + 0); indices.push_back(offset + i); indices.push_back(offset + i+1);
	}
	indices.push_back(offset + 0); indices.push_back(offset + t); indices.push_back(offset + 1);

	numElements = indices.size();

}
void Ellipse::computeColors(){
	assert((vertices.size() / 4) % (t + 1) == 0); //Make sure that vertices have already been computed
	for(int i = 0; i < t+1; i++){
		colors.push_back(0.4); colors.push_back(0.5); colors.push_back(0.0); colors.push_back(1.0);
	}
}

std::vector<geometry_type> Ellipse::getVertices(){
	return vertices;
}

std::vector<geometry_type> Ellipse::getNormals(){
	return normals;
}

std::vector<unsigned int> Ellipse::getIndices(){
	return indices;
}

std::vector<geometry_type> Ellipse::getColors(){
	return colors;
}

unsigned int Ellipse::getNumElements(){
	return numElements;
}
