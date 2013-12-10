#include "Paintball.h"
#include "../GLUTFramework/src/GlutFramework.h"

#include "../SPH/SphSolver.hpp"

using namespace glutFramework;
using namespace sph;

int main(int argc, char *argv[])
{
  KernelSpiky kernel(0.1);
	std::shared_ptr<CollisionHandlerNS::CollisionHandler> handler = std::make_shared<CollisionHandlerNS::CollisionHandler>();
  SphSolver simulation(0.1, 20, kernel, handler);
  
  /*std::vector<position> pos;
  std::vector<velocity> vel;

  for(int i = 0; i < 19; i++)
  {
    for(int j = 0; j < 19; j++)
    {
      for(int k = 0; k < 15; k++)
      {
        position tempPos;
        tempPos << 0.05 + i*0.05, 0.05 + j*0.05, 0.2 + k*0.05;
        pos.push_back(tempPos);
        velocity tempVel;
        tempVel << 0, 0, 0;
        vel.push_back(tempVel);
      }
    }
  }

  std::shared_ptr<SphWater> water = std::make_shared<SphWater>();
  simulation.insertParticles(pos, vel, water, false);*/

	position center;
	center << 0.4, 0.4, 1;

	velocity vel;
	vel << 100, 0, 80;

	simulation.createSphere(0.2, center, vel);

	GlutFramework* framework = new Paintball(&simulation);
	framework->startFramework(argc, argv);
	// **Note** No code below startFramework() will get executed 
	
	return 0;
}
