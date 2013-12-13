/*
 * ViewerShader.h
 *
 *  Created on: Nov 10, 2013
 *      Author: kevin
 */

#ifndef LUSTIGISZENE_H_
#define LUSTIGISZENE_H_

#include "../GLUTFramework/src/GlutFramework.h"
#include "../GLUTFramework/src/ShaderLoader.h"
#include "../GLUTFramework/src/RotatingView.h"
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>


//typedef struct
//{
//	float XYZW[4];
//	float RGBA[4];
//} Vertex;


class LustigiSzene : public glutFramework::GlutFramework{
public:
	LustigiSzene();
	~LustigiSzene();

	void display(float dTime);
	void load();
	void close();
	void drawCar();
	void objToVBO();
	void drawVBOobj(); 
	void sf_loadObj(char *fname);
	void wiki_load_obj(const char* filename, std::vector<glm::vec3> &vertices, std::vector<glm::vec3> &normals, std::vector<GLushort> &elements);
private:
	static const int nVao = 2;
	static const int nBuffer = 7;
	static const int nIndexBuffer = 1;
    GLuint
     VaoId[nVao],
     BufferId[nBuffer],
     IndexBufferId[nIndexBuffer];
	std::map<std::string, GLint> locs;
	unsigned int numElements;

	ShaderLoader sh;
	RotatingView* rv;

	void createVBO();
	void destroyVBO();
	void specialKeyboardDown(int key, int x, int y );
};



#endif /* LUSTIGISZENE_H_ */
