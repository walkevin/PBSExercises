#ifndef SPH_SOLVER_HPP
#define SPH_SOLVER_HPP

#include "SphBase.hpp"
#include "SphCell.hpp"

namespace sph
{
  class SphSolver
  {
    public:

      /*
      Executes a simulation step. So T_new = T_old + deltaT

      Arguments:
      1. deltaT
      */
      void simulationStep(entityValue);

      /*
      Function that returns the desired attribute at given positions

      Arguments:
      1. A vector of points where the desired attribute is computed
      2. The attribute that should be computed {example Attribute::density() }
      */
      std::vector<attributeValue> computeAttribute(std::vector<position>, Attribute) const;
      const SphCell& getNeighbour(coordinate, coordinate) const;
      const SphCell& getCell(coordinate) const;
      static const std::array<coordinate, 26>& getTransitions();
      
    private:
      static std::array<coordinate, 26> neighbourTransitions;
      std::vector<SphCell> cells;
      SphCell dummyCell;
      entityValue cutoff;
      entityValue gridSize;
      SmoothingKernel kernel;
    
      void initNeighbourTransitions();
  };
}

#endif
