#include "SphSolver.hpp"

namespace sph
{
  SphSolver::SphSolver(entityValue cut, discreteValue size, SmoothingKernel& kern)
  : dummyCell(cut, coordinate::Constant(-1), *this)
  , cutoff(cut)
  , gridSize(size)
  , kernel(kern)
  , cells()
  , gravity()
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
    for(int i = 0; i < cells.size(); i++)
    {
      cells[i].computeForces();
    }
    for(int i = 0; i < cells.size(); i++)
    {
      cells[i].update(deltaT);
    }
  }

  std::vector<attributeValue> SphSolver::computeAttribute(std::vector<position> points, Attribute attr) const
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

  std::vector<homogeneousPosition> SphSolver::getParticles() const
  {
    int vectorSize = 0;
    for(int i = 0; i < cells.size(); i++)
    {
      vectorSize += cells[i].getStoredParticles();
    }

    int index = 0;
    homogeneousPosition temp;
    std::vector<homogeneousPosition> positions(vectorSize);
    for(int i = 0; i < cells.size(); i++)
    {
      int particles = cells[i].getStoredParticles();
      const std::vector<position>& tempPos = cells[i].getPositions();
      for(int j = 0; j < particles; j++)
      {
        temp << tempPos[i], 1;
        positions[index] = temp;
        index++;
      }
    }
  }

  void SphSolver::insertParticles(std::vector<position> pos, std::vector<velocity> vel, SphLiquid* liq)
  {
    for(int i = 0; i < pos.size(); i++)
    {
      dummyCell.addParticle(pos[i], vel[i], liq);
    }
  
    dummyCell.makeTransitions();
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
  const SphCell& SphSolver::getNeighbour(coordinate center, coordinate translation) const
  {
    for(int i = 0; i < 3; i++)
    {
      if(center(i,0) + translation(i,0) > gridSize)
        return dummyCell;
    }
    int index = (center(0,0)+translation(0,0)) + (center(1,0)+translation(1,0))*gridSize + (center(2,0)+translation(2,0))*gridSize*gridSize;
    return cells[index];
  }

  const SphCell& SphSolver::getCell(coordinate center) const
  {
    for(int i = 0; i < 3; i++)
    {
      if(center(i,0) > gridSize)
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
}
