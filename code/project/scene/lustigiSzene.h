/*
 * ViewerShader.h
 *
 *  Created on: Nov 10, 2013
 *      Author: kevin
 */

#ifndef LUSTIGISZENE_H_
#define LUSTIGISZENE_H_

#include "../src/GlutFramework.h"
#include "../src/ShaderLoader.h"
#include "../src/RotatingView.h"
#include <string>
#include <map>

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
private:
	static const int nVao = 2;
	static const int nBuffer = 4;
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
