

/*
 * The MIT License
 *
 * Copyright (c) 2010 Paul Solt, PaulSolt@gmail.com 
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "Paintball.h"
#include "../GLUTFramework/src/GlutFramework.h"

#include "../SPH/SphSolver.hpp"

using namespace glutFramework;
using namespace sph;

int main(int argc, char *argv[])
{
  KernelSpiky kernel(0.1);
  SphSolver simulation(0.1, 20, kernel);
  
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

 /**
 * A sample program start that uses the base class GlutFramework to create a
 * graphics window that displays a teapot moving side to side.
 * Create a subclass of the GlutFramework and override the virtual methods.
 * 
 * @author Paul Solt 8-22-10
 */

	GlutFramework* framework = new Paintball(&simulation);
	framework->startFramework(argc, argv);
	// **Note** No code below startFramework() will get executed 
	
	return 0;
}
