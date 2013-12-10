#include "SphCell.hpp"
#include "SphSolver.hpp"
#include <cmath>
#include <iostream>

namespace sph
{ 
  SphCell::SphCell(entityValue size, coordinate cellCoord, SphSolver& parent)
  : vel(0)
  , pos(0)
  , f(0)
  , liq(0)
  , density(0)
  , pressure(0)
	, bonds(0)
  , storedParticles(0)
  , cellSize(size)
  , coord(cellCoord)
  , solver(parent)
  {
  }

  attributeValue SphCell::computeAttribute(position point, Attribute attrib, SmoothingKernel& kernel) const
  {
    /*Attribute mass = Attribute::mass();
    Attribute 
    std::array<coordinate, 27> transitions = solver.getTransitions();
    double temp = 0;
    for(int i = 0; i < transitions.size(); i++)
    {
      SphCell& neighbour = solver.getNeighbour(coord, transitions[i]);
      for(int j = 0; j < neighbour.attr.size(); j++)
      {
        temp += neighbour.attr[j].getAttribute(volume)*kernel(point, neighbour.pos[j])*neighbour.attr[j][attrib.getVal()];
      }
    }
    return temp;*/
  }

  void SphCell::updatePressure()
  {
    if(pressure.size() != storedParticles)
      pressure.reserve(storedParticles);

    entityValue stiffness = solver.getStiffness();
    for(int i = 0; i < storedParticles; i++)
    {
      attributeValue restDensity = liq[i]->getAttribute(Attribute::restDensity());
      pressure[i] = stiffness * std::max(0.0, density[i] - restDensity);
    }
  }

	void SphCell::clear()
	{
		makeTransitions();
		for(int i = storedParticles; i > 0; i--)
		{
			pos.pop_back();
			vel.pop_back();
			liq.pop_back();
			f.pop_back();
			density.pop_back();
			pressure.pop_back();
			bonds.pop_back();
			storedParticles--;
			assert(storedParticles == pos.size());
			assert(storedParticles == vel.size());
			assert(storedParticles == liq.size());
			assert(storedParticles == density.size());
		}
	}

  void SphCell::updateDensity()
  {
    if(density.size() != storedParticles)
      density.reserve(storedParticles);

    for(int i = 0; i < storedParticles; i++)
    {
      density[i] = 0;
    }

    std::array<coordinate, 27> transitions = solver.getTransitions();
    for(int i = 0; i < transitions.size(); i++)
    {
      SmoothingKernel &kernel = solver.getKernel();
      SphCell& neighbour = solver.getNeighbour(coord, transitions[i]);
      for(int j = 0; j < neighbour.storedParticles; j++)
      {
        attributeValue massN = neighbour.liq[j]->getAttribute(Attribute::mass());
        position posN = neighbour.pos[j];
        for(int k = 0; k < storedParticles; k++)
        {
          density[k] += massN * kernel(posN, pos[k]);
        }
      }
    }
  }

  void SphCell::update(entityValue deltaT)
  {
    updateVelocities(deltaT);
    updatePositions(deltaT);
  }

  void SphCell::addParticle(position posIn, velocity velIn, std::shared_ptr<SphLiquid> liqIn, bond bondIn)
  {
    pos.push_back(posIn);
    vel.push_back(velIn);
    f.push_back(force());
    liq.push_back(liqIn);
    density.push_back(0);
    pressure.push_back(0);
		bonds.push_back(bondIn);
    storedParticles++;
		assert(storedParticles == pos.size());
		assert(storedParticles == vel.size());
		assert(storedParticles == liq.size());
		assert(storedParticles == density.size());
  }

	void SphCell::deleteParticle(int index)
	{
		assert(index >= 0 && index < storedParticles);
		int last = storedParticles -1;
		if(index != last)
		{
			pos[index] = pos[last];
			vel[index] = vel[last];
			liq[index] = liq[last];
			density[index] = density[last];
			bonds[index] = bonds[last];
		}
		pos.pop_back();
		vel.pop_back();
		liq.pop_back();
		density.pop_back();        
		f.pop_back();
		pressure.pop_back();
		bonds.pop_back();
		storedParticles--;
		assert(storedParticles == pos.size());
		assert(storedParticles == vel.size());
		assert(storedParticles == liq.size());
		assert(storedParticles == density.size());
	}

  void SphCell::makeTransitions() 
  {
    for(int i = storedParticles-1; i >= 0; i--)
    {
      coordinate transition;
      for(int j = 0; j < 3; j++)
      {
        transition(j,0) = std::floor(pos[i](j,0)/cellSize) - coord(j,0);
      }
      if(transition(0,0) != 0 || transition(1,0) != 0 || transition(2,0) != 0)
      {
        SphCell& neighbour = solver.getNeighbour(coord, transition);
        neighbour.addParticle(pos[i], vel[i], liq[i], bonds[i]);
        int last = pos.size() - 1;
        if(last != i)
        {
          pos[i] = pos[last];
          vel[i] = vel[last];
          liq[i] = liq[last];
          density[i] = density[last];
					bonds[i] = bonds[last];
        }
        pos.pop_back();
        vel.pop_back();
        liq.pop_back();
        density.pop_back();        
        f.pop_back();
        pressure.pop_back();
				bonds.pop_back();
        storedParticles--;
				assert(storedParticles == pos.size());
				assert(storedParticles == vel.size());
				assert(storedParticles == liq.size());
				assert(storedParticles == density.size());
      }  
    }
  }

  void SphCell::computeForces()
  {
    //update pressure values and set the force to zero
    updatePressure();
    for(int i = 0; i < f.size(); i++)
    {
      if(f.size() != storedParticles)
        f.reserve(storedParticles);
      f[i] << 0, 0, 0;
    }

    SmoothingKernel& kernel = solver.getKernel();
    std::array<coordinate, 27> transitions = solver.getTransitions();
    for(int i = 0; i < transitions.size(); i++)
    {
      SphCell& neighbour = solver.getNeighbour(coord, transitions[i]);
      for(int j = 0; j < neighbour.storedParticles; j++)
      {
        attributeValue massN = neighbour.liq[j]->getAttribute(Attribute::mass() );
        attributeValue densityN = neighbour.density[j];
        attributeValue pressureN = neighbour.pressure[j];
        velocity velocityN = neighbour.vel[j];
        position posN = neighbour.pos[j];
        attributeValue preFactor = massN/densityN;
				position nullVec;
				nullVec << 0,0,0;
        for(int k = 0; k < storedParticles; k++)
        {
          position dirVec = pos[k] - posN;
					if(dirVec == nullVec)
						continue;
					if(bonds[k])
						continue;
          dirVec.normalize();
					assert(!isnan(dirVec(0)));
          f[k] -= dirVec * preFactor * 0.5 * (pressureN + pressure[k]) * kernel.grad(posN, pos[k]);
					assert(!isnan(f[k](1)));
          f[k] += liq[k]->getAttribute(Attribute::viscosity()) * preFactor * (velocityN - vel[k]) * kernel.laplace(posN, pos[k]);
					assert(!isnan(f[k](2)));    
        }
      }
    }
  
    force gravity = solver.getGravity();
    for(int i = 0; i < storedParticles; i++)
    {
      f[i] += density[i]*gravity;
    }
  }

  void SphCell::updatePositions(entityValue deltaT)
  {
		std::shared_ptr<CollisionHandlerNS::CollisionHandler> handler = solver.getCollisionHandler();
    for(int i = 0; i < storedParticles; i++)
    {
			for(int k = 0; k < 3; k++)
			{
				assert(!isnan(pos[i](k)));
				assert(!isnan(vel[i](k)));
				assert(!isnan(f[i](k)));
			}
			position posOld = pos[i];
      pos[i] = pos[i] + vel[i] * deltaT;
			std::tuple<bool, CollisionHandlerNS::position_t, CollisionHandlerNS::velocity_t> res;
			res = handler->particleVsAllObjects(pos[i], posOld, vel[i]);
			if(std::get<0>(res))
			{
				pos[i] = std::get<1>(res);
				vel[i] = std::get<2>(res);
				bonds[i] = true;
			}
    }
  }

  void SphCell::updateVelocities(entityValue deltaT)
  {
    for(int i = 0; i < storedParticles; i++)
    {
      vel[i] = vel[i] + f[i] * liq[i]->getAttribute(Attribute::mass()) / density[i];
    }
  }

  const std::vector<position>& SphCell::getPositions() const
  {
    return pos;
  }

  discreteValue SphCell::getStoredParticles() const
  {
    return storedParticles;
  }
}
