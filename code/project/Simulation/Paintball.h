#ifndef PAINTBALL_H_
#define PAINTBALL_H_

#include <Eigen/Core>
#include <array>

#include "../GLUTFramework/src/GlutFramework.h"
#include "../GLUTFramework/src/ShaderLoader.h"
#include "../GLUTFramework/src/RotatingView.h"

#include "../SPH/SphSolver.hpp"

typedef struct
{
	float XYZW[4];
	float RGBA[4];
} Vertex;


class Paintball : public glutFramework::GlutFramework{
public:
	Paintball(sph::SphSolver*);
	~Paintball();

	void display(float dTime);
	void load();
	void close();
private:
	std::vector<Eigen::Array<float, 4, 1> > pos;
	unsigned int n_points;

	sph::SphSolver* solver;

	std::vector<GLuint> vaoId, bufferId, indexBufferId;
	std::vector<GLint> locs;
	unsigned int numElements;

	ShaderLoader sh;
	RotatingView* rv;

	void createVBO();
	void destroyVBO();
	void updatePositions();
	void specialKeyboardDown(int key, int x, int y );

};



#endif /* PAINTBALL_H_ */
