#ifndef SPH_CELL_HPP
#define SPH_CELL_HPP

#include "SphBase.hpp"

namespace sph
{
  class SphCell
  {
    public:

      /*
      Function that returns the desired attribute at given positions

      Arguments:
      1. A point where the desired attribute is computed
      2. The attribute that should be computed {example Attribute::density() }
      3. A kernel that specifies the contribution of the particles around the desired position
      */
      attributeValue computeAttribute(position, Attribute, SmoothingKernel) const;

      /*
      Function that makes an update step for each particle in the cell

      Arguments:
      1. Timestep deltaT
      */
      void computeForces();
      void update(entityValue);
      
    private:
      std::vector<velocity> vel;
      std::vector<position> pos;
      std::vector<force> f;
      std::vector<attributes> attr;
      entityValue cellSize;
      coordinate coord;
      SphSolver &solver;

      void addParticle(position, velocity, attributes);
      void makeTransitions();
      void updatePositions(entityValue);
      void updateVelocities(entityValue); 
  };
}

#endif
