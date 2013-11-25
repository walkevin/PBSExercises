/*
 * 
 *
 *  Created on: Nov 10, 2013
 *      Author: kevin
 */

#ifndef EXAMPLE1_H_
#define EXAMPLE1_H_

#include "../GLUTFramework/src/GlutFramework.h"
#include "../GLUTFramework/src/ShaderLoader.h"

typedef struct
{
	float XYZW[4];
	float RGBA[4];
} Vertex;


class ShadersComing : public glutFramework::GlutFramework{
public:
	ShadersComing();
	~ShadersComing();

	void display(float dTime);
	void load();
	void close();
private:
	GLuint
	   VaoId,
	   BufferId,
	   IndexBufferId;

	ShaderLoader sh;
	void createVBO();
	void destroyVBO();
};



#endif /* VIEWERSHADER_H_ */
