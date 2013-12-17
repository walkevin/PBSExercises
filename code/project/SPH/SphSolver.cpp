#include "SphSolver.hpp"
#include <iostream>
#include <omp.h>

namespace sph
{
  SphSolver::SphSolver(entityValue cut, discreteValue size, SmoothingKernel& kern, std::shared_ptr<CollisionHandlerNS::CollisionHandler> handler)
	: neighbourTransitions()
  , dummyCell(cut, coordinate::Constant(-1), *this)
	, trash(cut, coordinate::Constant(-2), *this)
  , cutoff(cut)
  , gridSize(size)
  , kernel(kern)
  , cells()
  , gravity()
	, stiffness(1000)
	, collisionHandler(handler)
	, linTransFac(2./(2*cut*size))
	, linTransConst()
  {
		linTransConst[0] = 0.5;
		linTransConst[1] = 0.5;
		linTransConst[2] = -0.5;
    gravity << 0, -9.81, 0;
    initNeighbourTransitions();
    for(int k = 0; k < size; k++)
    {
      for(int j = 0; j < size; j++)
      {
        for(int i = 0; i < size; i++)
        {
          coordinate cellCoord;
          cellCoord << i, j, k;
          cells.push_back(SphCell(cut, cellCoord, *this));
        }
      }
    }
  }

  void SphSolver::simulationStep(entityValue deltaT)
  {
		lastTimestep = deltaT;
		#pragma omp parallel
		{
			#pragma omp for schedule(dynamic)
			for(int i = 0; i < cells.size(); i++)
			{
				cells[i].updateDensity();
			}
			#pragma omp barrier
			#pragma omp for schedule(dynamic)
		  for(int i = 0; i < cells.size(); i++)
		  {
		    cells[i].computeForces();
		  }
			#pragma omp barrier
			#pragma omp for schedule(dynamic)
		  for(int i = 0; i < cells.size(); i++)
		  {
		    cells[i].update(deltaT);
		  }
			#pragma omp barrier
			#pragma omp for schedule(dynamic)
			for(int i = 0; i < cells.size(); i++)
			{
				cells[i].makeTransitions();
			}
		}
		dummyCell.computeOnlyGravity();
		dummyCell.update(deltaT);
		dummyCell.checkDomain();
  }

	void SphSolver::createSphere(entityValue radius, position center, velocity vel)
	{
		double dist = cutoff/2;
		double dist2 = dist*dist;
		double radius2 = radius*radius;
		int range = std::floor(radius/dist);
		std::vector<position> positions(0);
		std::vector<velocity> velocities(0);
		for(int i = -range; i <= range; i++)
		{
			for(int j = -range; j <= range; j++)
			{
				for(int k = -range; k <= range; k++)
				{
					if((i*i + j*j + k*k)*dist2 > radius2)
						continue;
					position tempPos;
        	tempPos << center(0) + i*dist, center(1) + j*dist, center(2) + k*dist;
        	positions.push_back(tempPos);
       		velocities.push_back(vel);
				}
			}
		}
		std::shared_ptr<SphWater> water = std::make_shared<SphWater>();
		std::shared_ptr<bond> trueBond = std::make_shared<bond>();
		*trueBond = true;
  	this->insertParticles(positions, velocities, water, trueBond);
	}

	void SphSolver::addObject(std::vector<CollisionHandlerNS::collision_t> vertices, int stride, std::vector<unsigned int> indices)
	{
		CollisionHandlerNS::collision_t temp;
		double linTransFacInv = 1./linTransFac;
		for(int i = 0; i < vertices.size(); i+=4)
		{
			for(int j = 0; j < 4; j++)
			{
				if(j == 3)
					continue;
				vertices[i + j] = linTransFacInv * (vertices[i + j] + linTransConst[j]);
			}
		}
		collisionHandler->addObject(vertices, stride, indices);
	}
		
  std::vector<attributeValue> SphSolver::computeAttribute(std::vector<position> points, Attribute attr)
  {
    std::vector<attributeValue> returnValues(points.size());
    for(int i = 0; i < points.size(); i++)
    {
      coordinate coords;
      for(int j = 0; j < 3; j++)
      {
        coords(j,0) = std::floor(points[i](j,0)/cutoff);
      }
      returnValues[i] = getCell(coords).computeAttribute(points[i], attr, kernel);
    }
    return returnValues;
  }

	int SphSolver::getParticleNumber() const
	{
		int size = 0;
		for(int i = 0; i < cells.size(); i++)
    {
      size += cells[i].getStoredParticles();
    }
		size += dummyCell.getStoredParticles();
		return size;
	}

  std::vector<homogeneousPosition> SphSolver::getParticles() const
  {
    int vectorSize = getParticleNumber();
    int index = 0;
    homogeneousPosition temp;
    std::vector<homogeneousPosition> positions(vectorSize);
    for(int i = 0; i < cells.size(); i++)
    {
      int particles = cells[i].getStoredParticles();
      const std::vector<position>& tempPos = cells[i].getPositions();
      for(int j = 0; j < particles; j++)
      {
				position onePos = tempPos[j];
				onePos = onePos*linTransFac;
        temp << onePos(0) - linTransConst[0], onePos(1) - linTransConst[1], onePos(2) - linTransConst[2], 1;
        positions[index] = temp;
        index++;
      }
    }
		for(int i = 0; i < dummyCell.getStoredParticles(); i++)
		{
			const std::vector<position>& tempPos = dummyCell.getPositions();
			position onePos = tempPos[i];
			onePos = onePos*linTransFac;
			temp << onePos(0) - linTransConst[0], onePos(1) - linTransConst[1], onePos(2) - linTransConst[2], 1;
			positions[index] = temp;
			index++;
		}
    return positions;
  }

	std::vector<homogeneousPosition> SphSolver::getDeadParticles() 
	{
		int vectorSize = getDeadParticleNumber();
		std::vector<homogeneousPosition> positions(vectorSize);
		const std::vector<position>& tempPos = trash.getPositions();
		homogeneousPosition temp;
		for(int i = 0; i < vectorSize; i++)
		{
			position onePos = tempPos[i];
			onePos = onePos*linTransFac;
      temp << onePos(0) - linTransConst[0], onePos(1) - linTransConst[1], onePos(2) - linTransConst[2], 1;
      positions[i] = temp;
    }
		trash.clear();
		return positions;
	}

	SphCell& SphSolver::getTrashCell()
	{
		return trash;
	}

	int SphSolver::getDeadParticleNumber() const
	{
		return trash.getStoredParticles();
	}

  void SphSolver::insertParticles(std::vector<position> pos, std::vector<velocity> vel, std::shared_ptr<SphLiquid> liq, std::shared_ptr<bond> bondIn)
  {
    for(int i = 0; i < pos.size(); i++)
    {
      dummyCell.addParticle(pos[i], vel[i], liq, bondIn);
    }
  
    dummyCell.checkDomain();
  }

	void SphSolver::addParticle(position posIn, velocity velIn, std::shared_ptr<SphLiquid> liqIn, std::shared_ptr<bond> bondIn)
	{
		cells[0].addParticle(posIn, velIn, liqIn, bondIn);
	}

  SmoothingKernel& SphSolver::getKernel() const
  {
    return kernel;
  }

  void SphSolver::initNeighbourTransitions()
  { 
    int counter = 0;
    for(int i = -1; i < 2; i++)
    {
      for(int j = -1; j < 2; j++)
      {
        for(int k = -1; k < 2; k++)
        {
          neighbourTransitions[counter](0,0) = i;
          neighbourTransitions[counter](1,0) = j;
          neighbourTransitions[counter](2,0) = k;
          counter++;
        }
      }
    }
  }

  SphCell& SphSolver::getNeighbour(coordinate center, coordinate translation)
  {
    for(int i = 0; i < 3; i++)
    {
      if(center(i,0) + translation(i,0) >= gridSize || center(i,0) + translation(i,0) < 0)
        return dummyCell;
    }

    int index = (center(0,0)+translation(0,0)) + (center(1,0)+translation(1,0))*gridSize + (center(2,0)+translation(2,0))*gridSize*gridSize;

    return cells[index];
  }

  SphCell& SphSolver::getCell(coordinate center)
  {
    for(int i = 0; i < 3; i++)
    {
      if(center(i,0) > gridSize || center(i,0) < 0)
        assert(false);
    }

    int index = center(0,0) + center(1,0)*gridSize + center(2,0)*gridSize*gridSize;
    return cells[index];
  }

  const std::array<coordinate, 27>& SphSolver::getTransitions()
  {
    return neighbourTransitions;
  }

  force SphSolver::getGravity() const
  {
    return gravity;
  }

  entityValue SphSolver::getStiffness() const
  {
    return stiffness;
  }

	discreteValue SphSolver::getGridSize() const
	{
		return gridSize;
	}

	entityValue SphSolver::getLastTimestep() const
	{
		return lastTimestep;
	}

	std::shared_ptr<CollisionHandlerNS::CollisionHandler> SphSolver::getCollisionHandler()
	{
		return collisionHandler;
	}

	void SphSolver::clearObjects()
	{
		collisionHandler->clearObjects();
	}

	std::vector<CollisionHandlerNS::position_t> SphSolver::getCollisionPositions()
	{
		std::vector<CollisionHandlerNS::position_t> vec = collisionHandler->getCollisionPositions();
    for(int i = 0; i < vec.size(); i++)
    {
      for(int j = 0; j < 3; j++)
      {
				vec[i](j) = vec[i](j)*linTransFac - linTransConst[j];
      }
    }
		collisionHandler->clearCollisionPositions();
    return vec;
	}

	std::vector<CollisionHandlerNS::velocity_t> SphSolver::getCollisionVelocities()
	{
		std::vector<CollisionHandlerNS::velocity_t> vec = collisionHandler->getCollisionVelocities();
    for(int i = 0; i < vec.size(); i++)
    {
      vec[i] = vec[i]*linTransFac;
    }
		collisionHandler->clearCollisionVelocities();
    return vec;
	}

	std::vector<CollisionHandlerNS::velocity_t> SphSolver::getCollisionVelocitiesOrthogonal()
	{
		std::vector<CollisionHandlerNS::velocity_t> vec = collisionHandler->getCollisionVelocitiesOrthogonal();
    for(int i = 0; i < vec.size(); i++)
    {
      vec[i] = vec[i]*linTransFac;
    }
		collisionHandler->clearCollisionVelocitiesOrthogonal();
    return vec;
	}

	void SphSolver::rotateObjects(double angle)
	{
		collisionHandler->rotateObjects(angle);
	}

	void SphSolver::makeTransitions()
	{
		cells[0].makeTransitions();
	}
}
