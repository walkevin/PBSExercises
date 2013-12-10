#ifndef PAINTBALL_H_
#define PAINTBALL_H_

#include <Eigen/Core>
#include <array>
#include <map>
#include <string>

#include "../GLUTFramework/src/GlutFramework.h"
#include "../GLUTFramework/src/ShaderLoader.h"
#include "../GLUTFramework/src/RotatingView.h"
#include "../GLUTFramework/src/GeometricObject.h"

#include "../SPH/SphSolver.hpp"
#include "../Collision/CollisionHandler.h"


struct objectInfo{
	objectInfo(int numElements_, int numInstances_) : numElements(numElements_), numInstances(numInstances_){}
	int numElements;
	int numInstances;
};


class Paintball : public glutFramework::GlutFramework{
public:
	Paintball(sph::SphSolver*);
	~Paintball();

	void display(float dTime);
	void load();
	void close();
private:
	std::vector<Eigen::Array<float, 4, 1> > activeParticles;
//	std::vector<Eigen::Array<float, 4, 1> > deadParticles;

	unsigned int nActiveParticles;
//	unsigned int nDeadParticles;
	unsigned int nTotalParticles;

	sph::SphSolver* solver;

	int nVao;
	int nBuffer;
	int nIndexBuffer;

	GLuint *vaoId, *bufferId, *indexBufferId;
	std::map<std::string, GLint> locs;
	std::vector<objectInfo> objInfo;

	ShaderLoader sh;
	RotatingView* rv;
	CollisionHandlerNS::CollisionHandler* ch;

	void createVBO();
	void destroyVBO();
	void updatePositions();
	void specialKeyboardDown(int key, int x, int y );

	void uploadGeometricObject(GeometricObject* obj, int numObj, std::vector<glm::mat4> objTransforms, int bufferStride);

};



#endif /* PAINTBALL_H_ */
