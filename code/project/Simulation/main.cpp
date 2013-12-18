#include "Paintball.h"
#include "../GLUTFramework/src/GlutFramework.h"

#include "../SPH/SphSolver.hpp"

using namespace glutFramework;
using namespace sph;

int main(int argc, char *argv[])
{
  KernelSpiky kernel(0.05);
	std::shared_ptr<CollisionHandlerNS::CollisionHandler> handler = std::make_shared<CollisionHandlerNS::CollisionHandler>();
  SphSolver simulation(0.05, 40, kernel, handler);
  
  /*std::vector<position> pos;
  std::vector<velocity> vel;

  for(int i = 0; i < 1; i++)
  {
    for(int j = 0; j < 1; j++)
    {
      for(int k = 0; k < 1; k++)
      {
        position tempPos;
        tempPos << 0 + i*0.5, 0 + j*0.5, 6 + k*0.5;
        pos.push_back(tempPos);
        velocity tempVel;
        tempVel << 0, 0, -100;
        vel.push_back(tempVel);
      }
    }
  }

  std::shared_ptr<SphWater> water = std::make_shared<SphWater>();
	std::shared_ptr<bond> bondTemp = std::make_shared<bond>();
	*bondTemp = true;
  simulation.insertParticles(pos, vel, water, bondTemp);*/

	/*position center;
	center << 1, 1.4, 6;

	velocity vel;
	vel << 0, 100, -200;

	simulation.createSphere(0.11, center, vel);*/

	GlutFramework* framework = new Paintball(&simulation);
	framework->startFramework(argc, argv);
	// **Note** No code below startFramework() will get executed 
	
	return 0;
}
