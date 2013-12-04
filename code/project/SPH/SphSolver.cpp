#include "SphSolver.hpp"
#include <iostream>
#include <omp.h>

namespace sph
{
  SphSolver::SphSolver(entityValue cut, discreteValue size, SmoothingKernel& kern)
	: neighbourTransitions()
  , dummyCell(cut, coordinate::Constant(-1), *this)
  , cutoff(cut)
  , gridSize(size)
  , kernel(kern)
  , cells()
  , gravity()
	, stiffness(1000)
  {
    gravity << 0, 0, -9.81;
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
        temp << onePos(0), onePos(2), onePos(1), 2;
				temp = temp - 1.;
        positions[index] = temp;
        index++;
      }
    }
    return positions;
  }

  void SphSolver::insertParticles(std::vector<position> pos, std::vector<velocity> vel, std::shared_ptr<SphLiquid> liq)
  {
    for(int i = 0; i < pos.size(); i++)
    {
      cells[0].addParticle(pos[i], vel[i], liq);
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
}
