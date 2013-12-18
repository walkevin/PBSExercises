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
			float vx, vy, vz;
			s >> vx; s >> vy; s >> vz;
			vertices.push_back(vx);
			vertices.push_back(vy);
			vertices.push_back(vz);
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
			float nx, ny, nz;
			s >> nx; s >> ny; s >> nz;
			normals.push_back(nx);
			normals.push_back(ny);
			normals.push_back(nz);
			normals.push_back(1.0);
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
	if(normals.size() == 0){
		//return dummy normals if not specified
		assert(vertices.size() > 0);
		normals.assign(vertices.size(), 1.);
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
