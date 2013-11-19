#ifndef SPH_SOLVER_HPP
#define SPH_SOLVER_HPP

#include "SphBase.hpp"
#include "SphCell.hpp"
#include "SphLiquid.hpp"

namespace sph
{
  class SphSolver
  {
    public:

      /*
      Constructor

      Arguments:
      1. Cutoff (Determines size of cells)
      2. GridSize in 1D (#Cells GridSize^3 and Domain is [0, Cutoff*GridSize]^3 )
      3. Kernel to compute the Attributes (Poly6 is an option)
      */
      SphSolver(entityValue, entityValue, SmoothingKernel&);

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

      /* 
      Returns a displacement based on a current position
      
      Arguments:
      1. Position from which the relative displacement is computed
      2. relative displacement
      */
      const SphCell& getNeighbour(coordinate, coordinate) const;

      /*
      Returns the cell at the given position

      Argument:
      1. Requested position
      */
      const SphCell& getCell(coordinate) const;

      /*
      Returns an array with the displacements for the 26 neighbours + a zero displacement (self)
      */
      static const std::array<coordinate, 27>& getTransitions();

      /*
      Returns the kernel
      */
      SmoothingKernel& getKernel() const;

      /*
      Returns gravity
      */
      force getGravity() const;

      /*
      Returns stiffnes Parameter k
      */
      entityValue getStiffness() const;
      
    private:
      static std::array<coordinate, 27> neighbourTransitions;
      SphCell dummyCell;
      entityValue cutoff;
      entityValue gridSize;
      SmoothingKernel& kernel;
      std::vector<SphCell> cells;
      force gravity;
      entityValue stiffness;    

      static void initNeighbourTransitions();
  };
}

#endif
