#include "SphCell.hpp"
#include "SphSolver.hpp"

namespace sph
{ 
  attributeValue SphCell::computeAttribute(position point, Attribute attrib, SmoothingKernel kernel) const
  {
    attributeValue temp = 0;
    for(int i = 0; i < this->attr.size(); i++)
    {
      temp += kernel(point, pos[i])*(this->attr[i][attrib.getVal()]);
    }
    for(int i = 0; i < solver.getTransitions().size(); i++)
    {
      SphCell neighbour = solver.getNeighbour(coord, solver.getTransitions()[i]);
      for(int j = 0; j < this->attr.size(); j++)
      {
        temp += kernel(point, neighbour.pos[j])*neighbour.attr[j][attrib.getVal()];
      }
    }
  }

  void SphCell::update(entityValue deltaT)
  {
    updateVelocities(deltaT);
    updatePositions(deltaT);
    makeTransitions();
  }

  void SphCell::addParticle(position pos, velocity vel, attributes attr)
  {
    this->pos.push_back(pos);
    this->vel.push_back(vel);
    this->attr.push_back(attr);
  }

  void SphCell::makeTransitions() 
  {
    for(int i = 0; i < pos.size(); i++)
    {
      coordinate transition;
      for(int j = 0; j < 3; j++)
      {
        transition(j,0) = std::floor(pos[i](j,0)/cellSize) - coord(j,0);
      }
      if(transition(0,0) != 0 || transition(1,0) != 0 || transition(2,0) != 0)
      {
        SphCell neighbour = solver.getNeighbour(coord, transition);
        neighbour.addParticle(pos[i], vel[i], attr[i]);
        int last = pos.size() - 1;
        pos[i] = pos[last]; pos.pop_back();
        vel[i] = vel[last]; vel.pop_back();
        attr[i] = attr[last]; attr.pop_back();
      }  
    }
  }

  void SphCell::computeForces()
  {
  }

  void SphCell::updatePositions(entityValue deltaT)
  {
    for(int i = 0; i < pos.size(); i++)
    {
      pos[i] = pos[i] + vel[i] * deltaT;
    }
  }

  void SphCell::updateVelocities(entityValue deltaT)
  {
    for(int i = 0; i < vel.size(); i++)
    {
      vel[i] = vel[i] + attr[i][Attribute::density().getVal()] * attr[i][Attribute::volume().getVal()] * f[i];
    }
  }
}
