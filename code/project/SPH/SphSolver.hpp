#ifndef SPH_SOLVER_HPP
#define SPH_SOLVER_HPP

#include "SphBase.hpp"
#include "SphCell.hpp"
#include "SphLiquid.hpp"
#include "../Collision/CollisionHandler.h"

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
			4. CollisionHandler
      */
      SphSolver(entityValue, discreteValue, SmoothingKernel&, std::shared_ptr<CollisionHandlerNS::CollisionHandler>);

      /*
      Executes a simulation step. So T_new = T_old + deltaT

      Arguments:
      1. deltaT
      */
      void simulationStep(entityValue);

			/*
			Inserts Particles in form of a sphere
			
			Arguments:
			1. Radius of the sphere
			*/
			void createSphere(entityValue, position, velocity);

			/*
			Inserts static objects into the collision handler
			
			Arguments:
			1. vertices, list of vertices
			2. vertex stride (entweder 3 oder 4)
	 		3. indices, list of indices (must be divisble by three) 
			*/
			void addObject(std::vector<CollisionHandlerNS::collision_t>, int, std::vector<unsigned int>);

      /*
      Function that returns the desired attribute at given positions

      Arguments:
      1. A vector of points where the desired attribute is computed
      2. The attribute that should be computed {example Attribute::density() }
      */
      std::vector<attributeValue> computeAttribute(std::vector<position>, Attribute);

      /*
      Function that returns the Position of all particles in homogenoues Positions ( x,y,z,1 )
      */
      std::vector<homogeneousPosition> getParticles() const;

			/*
			Returns the current amount of stored Particles
			*/
			int getParticleNumber() const;

      /*
      Inserts Particles of a given liquid, where positions and velocities are specified

      Arguments:
      1. Vector with the positions of the particles
      2. Vector with the velocities of the particles
      3. Liquid of the Particles
      */
      void insertParticles(std::vector<position>, std::vector<velocity>, std::shared_ptr<SphLiquid>, std::shared_ptr<bond>);

			/*
			Returns the number of currently dead Particles
			*/
			int getDeadParticleNumber() const;

			/*
			A function to read out dead particles (no motion and on a surface)
			*/
			std::vector<homogeneousPosition> getDeadParticles();

			/*
			A function that returns a reference to the trash cell
			*/
			SphCell& getTrashCell();

      /* 
      Returns a displacement based on a current position
      
      Arguments:
      1. Position from which the relative displacement is computed
      2. relative displacement
      */
      SphCell& getNeighbour(coordinate, coordinate);

      /*
      Returns the cell at the given position

      Argument:
      1. Requested position
      */
      SphCell& getCell(coordinate);

      /*
      Returns an array with the displacements for the 26 neighbours + a zero displacement (self)
      */
      const std::array<coordinate, 27>& getTransitions();

      /*
      Returns gravity
      */
      force getGravity() const;

      /*
      Returns stiffnes Parameter k
      */
      entityValue getStiffness() const;
      
			/*
			Returns gridSize
			*/
			discreteValue getGridSize() const;

			/*
			Returns the last timestep
			*/
			entityValue getLastTimestep() const;

			/*
			Returns 4x4 Backtransformation matrix from the Simulation coordinates to coordinates from [-1, 1]^3
			
			*/
			Eigen::Matrix<float, 4, 4> getBackTransform();

			/*
			Returns collision Handler
			*/
			std::shared_ptr<CollisionHandlerNS::CollisionHandler> getCollisionHandler();

			/*
			Returns the Positions from the Collision Handler where Collisions occured
			*/
			std::vector<CollisionHandlerNS::position_t> getCollisionPositions();

			/*
			Returns the Velocities from the Collision Handler where Collisions occured
			*/
			std::vector<CollisionHandlerNS::velocity_t> getCollisionVelocities();

			/*
			Returns the Orthogonal Velocities from the Collision Handler where Collisions occured
			*/
			std::vector<CollisionHandlerNS::velocity_t> getCollisionVelocitiesOrthogonal();

		private:

			/*
      Returns the kernel
      */
      SmoothingKernel& getKernel() const;

      std::array<coordinate, 27> neighbourTransitions;
      SphCell dummyCell;
			SphCell trash;
      entityValue cutoff;
      discreteValue gridSize;
      SmoothingKernel& kernel;
      std::vector<SphCell> cells;
      force gravity;
      entityValue stiffness;
			entityValue lastTimestep;
			std::shared_ptr<CollisionHandlerNS::CollisionHandler> collisionHandler;
			double linTransFac; 

      void initNeighbourTransitions();

			friend SphCell;
  };
}

#endif
