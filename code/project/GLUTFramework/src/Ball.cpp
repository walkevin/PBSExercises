/*
 * Ball.cpp
 *
 *  Created on: Nov 23, 2013
 *      Author: kevin
 *
 *
 *  Example
 *
 *  	Ball b(N, M, R);
		std::vector<geometry_type> vertices = b.getVertices();
		std::vector<GLuint> indices = b.getIndices();
		numElements = b.getNumElements();

		Displayfunction

		glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, NULL);

 */
//TODO: Move computation of vertices and indices into constructor, create private variables vertices and indices

#include "Ball.h"
#include <vector>
#include <cmath>

Ball::Ball(int N_, int M_, geometry_type R_) : N(N_), M(M_), R(R_), numElements(N_ * M_ * 6){}

Ball::~Ball() {}

std::vector<geometry_type> Ball::getVertices(){
	//Specify vertices for sphere (displaced latitude-longitude discretization)
	std::vector<geometry_type> vertices;

	//North Pole
	vertices.push_back(0.0f); vertices.push_back(R); vertices.push_back(0.0f); vertices.push_back(1.0f);

	double theta = M_PI/(N+1), phi;
	for(int n = 0; n < N; n++){//loop over theta
		phi = 0.;
		for(int m = 0; m < M; m++){//loop over phi
			vertices.push_back(R * std::sin(theta) * std::cos(phi + n * (M_PI/M)) );
			vertices.push_back(R * std::cos(theta));
			vertices.push_back(R * std::sin(theta) * std::sin(phi + n * (M_PI/M)) );
			vertices.push_back(1.0f);

			phi += 2*M_PI/M;
		}
		theta += M_PI/(N+1);
	}
	//South pole
	vertices.push_back(0.0f); vertices.push_back(-R); vertices.push_back(0.0f); vertices.push_back(1.0f);

	return vertices;
}

std::vector<geometry_type> Ball::getNormals(){
	std::vector<geometry_type> normals;

	//North Pole
	normals.push_back(0.0f); normals.push_back(1.0f); normals.push_back(0.0f);

	double theta = M_PI/(N+1), phi;
	for(int n = 0; n < N; n++){//loop over theta
		phi = 0.;
		for(int m = 0; m < M; m++){//loop over phi
			normals.push_back(std::sin(theta) * std::cos(phi +  n * (M_PI/M) ));
			normals.push_back(std::cos(theta));
			normals.push_back(std::sin(theta) * std::sin(phi +  n * (M_PI/M) ));

			phi += 2*M_PI/M;
		}
		theta += M_PI/(N+1);
	}
	//South pole
	normals.push_back(0.0f); normals.push_back(-1.0f); normals.push_back(0.0f);

	return normals;
}


std::vector<unsigned int> Ball::getIndices(){
	//Specify indices
	std::vector<unsigned int> indices;
	//Specify triangles connecting with North-pole
	for(unsigned int m = 1; m < M; m++){//start at m=1 since North-pole is vertex 0.
		indices.push_back(0); indices.push_back(m);	indices.push_back(m+1);
	}
	indices.push_back(0); indices.push_back(M);	indices.push_back(1);//Handle last triangle specially

	//Specify all "normal" triangles. Per lat-lon "rectangle" there are two triangles
	for(unsigned int n = 0; n < N-1; n++){
		for(unsigned int m = 1; m < M; m++){
			indices.push_back(n*M + m); indices.push_back(n*M + m+1); indices.push_back((n+1)*M + m);
			indices.push_back(n*M + m+1); indices.push_back((n+1)*M + m); indices.push_back((n+1)*M + m+1);
		}
		//Again handle last "rectangle" separately
		indices.push_back(n*M + M); indices.push_back(n*M + 1); indices.push_back((n+1)*M + M);
		indices.push_back(n*M + 1); indices.push_back((n+1)*M + M); indices.push_back((n+1)*M + 1);
	}

	//Specify triangles connecting with South-pole
	for(unsigned int m = (N-1)*M+1; m < N*M; m++){//start at m=1 since North-pole is vertex 0.
		indices.push_back(N*M+1); indices.push_back(m);	indices.push_back(m+1);
	}
	indices.push_back(N*M+1); indices.push_back(N*M);	indices.push_back((N-1)*M+1);//Handle last triangle specially

	numElements = indices.size();

	return indices;
}

unsigned int Ball::getNumElements(){
	return numElements;
}
