#include "../SPH/SphSolver.hpp"
#include <memory>
#include <iostream>

using namespace sph;

int main()
{
  KernelPoly6 kernel(0.5);
  SphSolver simulation(0.5, 5, kernel);
  
  std::vector<position> pos;
  std::vector<velocity> vel;

  for(int i = 0; i < 3; i++)
  {
    for(int j = 0; j < 3; j++)
    {
      for(int k = 0; k < 3; k++)
      {
        position tempPos;
        tempPos << 1 + i*0.1, 1 + j*0.1, 1 + k*0.1;
        pos.push_back(tempPos);
        velocity tempVel;
        tempVel << 0, 0, 0;
        vel.push_back(tempVel);
      }
    }
  }

  std::shared_ptr<SphWater> water = std::make_shared<SphWater>();
  simulation.insertParticles(pos, vel, water);

  for(int i = 0; i < 1000; i++)
  {
    simulation.simulationStep(0.01);
		std::vector<homogeneousPosition> particles = simulation.getParticles(); //here are stored the positions
  }
}
