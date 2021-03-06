#ifndef SPH_CELL_HPP
#define SPH_CELL_HPP

#include "SphBase.hpp"
#include "SphLiquid.hpp"
#include <memory>

namespace sph
{
  class SphCell
  {
    public:

      /*
      Constructor

      Arguments:
      1. Size of 1 dimension (all 3 are the same)
      2. Coordinate of the Cell in the Solver
      3. Reference to the solver it belongs
      */
      SphCell(entityValue, coordinate, SphSolver&);

      /*
      Function that returns the desired attribute at given positions

      Arguments:
      1. A point where the desired attribute is computed
      2. The attribute that should be computed {example Attribute::density() }
      3. A kernel that specifies the contribution of the particles around the desired position
      */
      attributeValue computeAttribute(position, Attribute, SmoothingKernel&) const;

      /*
      Function that makes an update step for each particle in the cell

      Arguments:
      1. Timestep deltaT
      */
      void update(entityValue);

      /*
      Function that computes the forces for each particle. Has to be called before an update
      */
      void computeForces();

      /*
      Computes the new densities and pressure of the particles
      */
      void updatePressure();

      /*
      Computes the new densities of the particles
      */
      void updateDensity();

      /*
      Returns the particle positions
      */
      const std::vector<position>& getPositions() const;

      /*
      Returns the number of stored particles
      */
      inline discreteValue getStoredParticles() const {return storedParticles;};

      /*
      Adds a particle to the cell
      
      Arguments:
      1. Position of the particle
      2. Velocity of the particle
      3. Liquid of the particle
			4. Bool value for the bond
      */
      void addParticle(position, velocity, std::shared_ptr<SphLiquid>, std::shared_ptr<bond>);

      /*
      Checks if all particles are still inside the boundaries and else adds them to the right cell
      */
      void makeTransitions();

			/*
			Deletes all the particles belonging to the given cell (if the particles fit into another cell, they're not deleted)
			*/
			void clear();

			/*
			Deletes the particle at the given index
			
			Arguments:
			1. Index of the particle to be deleted
			*/
			void deleteParticle(int);

			/*
			Sets all bonds true that are new added with push_back
			*/
			void setBonds();

			/*
			Checks if the particle moves into the computing domain and if yes adds it
			*/
			void checkDomain();

			/*
			Computes only gravity
			*/
			void computeOnlyGravity();
 
    private:
      std::vector<velocity> vel;
      std::vector<position> pos;
      std::vector<force> f;
      std::vector<std::shared_ptr<SphLiquid>> liq;
      std::vector<attributeValue> density;
      std::vector<attributeValue> pressure;
			std::vector<std::shared_ptr<bond>> bonds;
      discreteValue storedParticles;
      entityValue cellSize;
      coordinate coord;
      SphSolver &solver;

      void updatePositions(entityValue);
      void updateVelocities(entityValue); 
  };
}

#endif
