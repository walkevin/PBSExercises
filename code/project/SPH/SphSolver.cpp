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
  {
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
		dummyCell.clear();
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
		double linTrans = 0.5 * gridSize * cutoff;
		CollisionHandlerNS::collision_t temp;
		for(int i = 0; i < vertices.size(); i+=4)
		{
			for(int j = 0; j < 4; j++)
			{
				if(j == 3)
					continue;
				vertices[i + j] = linTrans * (vertices[i + j] + 1);
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
		return size;
	}

  std::vector<homogeneousPosition> SphSolver::getParticles() const
  {
    int vectorSize = getParticleNumber();
    int index = 0;
		double linTransFac = 2./(cutoff*gridSize);
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
        temp << onePos(0), onePos(1), onePos(2), 2;
				temp = temp - 1.;
        positions[index] = temp;
        index++;
      }
    }
    return positions;
  }

	std::vector<homogeneousPosition> SphSolver::getDeadParticles() 
	{
		int vectorSize = getDeadParticleNumber();
		double linTransFac = 2./(cutoff*gridSize);
		std::vector<homogeneousPosition> positions(vectorSize);
		const std::vector<position>& tempPos = trash.getPositions();
		homogeneousPosition temp;
		for(int i = 0; i < vectorSize; i++)
		{
			position onePos = tempPos[i];
			onePos = onePos*linTransFac;
      temp << onePos(0), onePos(1), onePos(2), 2;
			temp = temp - 1.;
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
      cells[0].addParticle(pos[i], vel[i], liq, bondIn);
    }
  
    cells[0].makeTransitions();
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

	Eigen::Matrix<float, 4, 4> SphSolver::getBackTransform()
	{
		Eigen::Matrix<float, 4, 4> B;
		double linTransFac = 2./(cutoff*gridSize);
		B << linTransFac, 0, 0, -1,
			 0, linTransFac, 0, -1,
			 0, 0, linTransFac, -1,
			 0, 0, 0, 1;
		return B;
	}
}
