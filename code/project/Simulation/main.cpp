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
  
  std::vector<position> pos;
  std::vector<velocity> vel;

  for(int i = 0; i < 2; i++)
  {
    for(int j = 0; j < 2; j++)
    {
      for(int k = 0; k < 2; k++)
      {
        position tempPos;
        tempPos << 1.5 + i*0.05, 1 + j*0.05, 1 + k*0.05;
        pos.push_back(tempPos);
        velocity tempVel;
        tempVel << 100, 0, 0;
        vel.push_back(tempVel);
      }
    }
  }

  std::shared_ptr<SphWater> water = std::make_shared<SphWater>();
	std::shared_ptr<bond> bondTemp = std::make_shared<bond>();
	*bondTemp = false;
  simulation.insertParticles(pos, vel, water, bondTemp);

//	position center;
//	center << 0.4, 0.4, 1;
//
//	velocity vel;
//	vel << 100, 0, 80;
//
//	simulation.createSphere(0.2, center, vel);

	GlutFramework* framework = new Paintball(&simulation);
	framework->startFramework(argc, argv);
	// **Note** No code below startFramework() will get executed 
	
	return 0;
}
