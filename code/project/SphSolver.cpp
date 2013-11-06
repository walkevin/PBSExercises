#include "SphSolver.hpp"

namespace sph
{
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

  void SphSolver::initNeighbourTransitions()
  { 
    int counter = 0; 
    for(int i = -1; i < 2; i++)
    {
      for(int j = -1; j < 2; j++)
      {
        for(int k = -1; k < 2; k++)
        {
          if(counter == 26)
          {
            neighbourTransitions[13](0,0) = i;
            neighbourTransitions[13](1,0) = j;
            neighbourTransitions[13](2,0) = k;
          }
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

  const std::array<coordinate, 26>& SphSolver::getTransitions()
  {
    return neighbourTransitions;
  }
}
