/*
 * OBJModel.cpp
 *
 *  Created on: Dec 8, 2013
 *      Author: kevin
 *
 *
 *  Example
 *
 *  	OBJModel p(a, b, h);
		std::vector<geometry_type> vertices = p.getVertices();
		std::vector<GLuint> indices = p.getIndices();
		numElements = p.getNumElements();

		Displayfunction

		glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, NULL);

 */
//TODO: Move computation of vertices and indices into constructor, create private variables vertices and indices
#include "OBJModel.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <Eigen/Geometry>
#include <cmath>


OBJModel::OBJModel(const char* filename_, Eigen::Vector4f bodyColor_) : bodyColor(bodyColor_){
	loadObj(filename_);
}

OBJModel::~OBJModel() {}

void OBJModel::loadObj(const char* filename){
	  std::ifstream in(filename, std::ios::in);
	  if (!in) { std::cerr << "Cannot open " << filename << std::endl; exit(1); }

	  std::string line;
	  while (getline(in, line)) {

	    //Read vertices
	    if (line.substr(0,2) == "v ") {
			std::istringstream s(line.substr(2));
			geometry_type a, b, c;
			s >> a; s >> b; s >> c;
			vertices.push_back(a);
			vertices.push_back(b);
			vertices.push_back(c);
			vertices.push_back(1.0);
	    }

	    //Read Faces
	    else if (line.substr(0,2) == "f ") {
			std::istringstream s(line.substr(2));
			short temp[4];
			int numFaces = 0;
			bool eofReached = false;
			for(int i = 0; i < 4; i++){
				if(s.peek() == '/'){
					//Trashing '/'
					char trash = s.get();
					while(true){
						//Trashing subsequent chars until next space character
						trash = s.get();
						if(trash == ' ')
						  break;
						if(s.eof()){
						  eofReached = true;
						  break;
						}
					}
				}
				if(eofReached)
				  break;

				//Read into temporary buffer only if there is no reading error (e.g. end of line)
				if(s >> temp[i]){
				 temp[i] -= 1;
				 numFaces++;
				}
			}//End for

			indices.push_back(temp[0]); indices.push_back(temp[1]); indices.push_back(temp[2]);
			//Only triangles are supported, manually create two triangles out of a quadrilateral
			if(numFaces == 4){
				indices.push_back(temp[0]); indices.push_back(temp[2]); indices.push_back(temp[3]);
			}
	    }

	    //Read Normals
	    else if (line.substr(0,3) == "vn ") {
			std::istringstream s(line.substr(3));
			geometry_type nx, ny, nz;
			s >> nx; s >> ny; s >> nz;
			normals.push_back(nx);
			normals.push_back(ny);
			normals.push_back(nz);
	    }

	    else if (line[0] == '#') { /* ignoring this line */ }
	    else { /* ignoring this line */ }
	  }

	  numElements = indices.size();
}


std::vector<geometry_type> OBJModel::getVertices(){
	return vertices;
}

std::vector<geometry_type> OBJModel::getNormals(){
//	if(normals.size() == 0){
//		const int vertexStride = 4;
//
//		assert(vertices.size() > 0);
//		normals.reserve(vertices.size() / 4 * 3);
//		normals.assign(vertices.size() / 4 * 3, 0);
//
//		std::vector<int> counts(vertices.size() / 4, 0);
//		using namespace Eigen;
//
//
//
//		for(std::vector<unsigned int>::iterator it = indices.begin(); it != indices.end(); ++it){
//			unsigned int vInd[3];
//			vInd[0] = *it; vInd[1] = *(it+1); vInd[2] = *(it+2);
//			Vector3f v0(vertices[vertexStride * vInd[0]], vertices[vertexStride * vInd[0] + 1], vertices[vertexStride * vInd[0] + 2]);
//			Vector3f v1(vertices[vertexStride * vInd[1]], vertices[vertexStride * vInd[1] + 1], vertices[vertexStride * vInd[1] + 2]);
//			Vector3f v2(vertices[vertexStride * vInd[2]], vertices[vertexStride * vInd[2] + 1], vertices[vertexStride * vInd[2] + 2]);
//
//			//Compute (not normalized) triangle normal by vector product
//			Vector3f triangleNormal = ((v1-v0).cross(v2-v0)).normalized(); //note: normal always points away from the plane
//
//			if(!std::isnan(triangleNormal[0])){
//				//Loop over the three vertices
//				for(int i = 0; i < 3; i++){
//					float inFactor = counts[vInd[i]] / (counts[vInd[i]] + 1);
//					float outFactor = 1 / (counts[vInd[i]] + 1);
//
//					Eigen::Vector3f ntmp(normals[3*vInd[i]], normals[3*vInd[i] + 1], normals[3*vInd[i] + 2]);
//					Eigen::Vector3f res = inFactor * ntmp + outFactor * triangleNormal;
//
//					normals[3 * vInd[i]] = res[0];
//					normals[3 * vInd[i] + 1] = res[1];
//					normals[3 * vInd[i] + 2] = res[2];
//					counts[vInd[i]] = counts[vInd[i]] + 1;
//				}
//			}
//		}
//	}

	if(normals.size() == 0){
		normals.assign(vertices.size() / 4 * 3, 1.0);
	}
	return normals;
}

std::vector<unsigned int> OBJModel::getIndices(){
	return indices;
}

std::vector<geometry_type> OBJModel::getColors(){
	assert(vertices.size() > 0);
	for(int i = 0; i < vertices.size(); i+=4){
		colors.push_back(bodyColor[0]);
		colors.push_back(bodyColor[1]);
		colors.push_back(bodyColor[2]);
		colors.push_back(bodyColor[3]);
	}
	return colors;
}

unsigned int OBJModel::getNumElements(){
	return numElements;
}
