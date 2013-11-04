#include <eigen/core>
#include <vector>

namespace sph
{
  typedef Eigen::Array<double, 3, 1> velocity;
  typedef Eigen::Array<double, 3, 1> position;
  typedef Eigen::Array<int, 3, 1> coordinate;
  typedef double attributeValue;
  typedef double entityValue;
  typedef attributeValue (*SmoothingKernel)(position, position);

  class Attribute
  {
    public:
      static Attribute density() {return Attribute(1); };
      static Attribute pressure() {return Attribute(2); };
      static int getNumAttributes() {return numAttributes; };
    
    private:
      explicit Attribute(int attribute) {this.attribute = attribute; };
      int attribute;
      static int const numAttributes = 2;
  }

  class SphSolver
  {
    public:
      sphSolver();
      ~sphSolver();

      /*
      Executes a simulation step. So T_new = T_old + deltaT

      Arguments:
      1. deltaT
      */
      void simulationStep(entiyValue);

      /*
      Function that returns the desired attribute at given positions

      Arguments:
      1. A vector of points where the desired attribute is computed
      2. The attribute that should be computed {example Attribute::density() }
      */
      std::vector<attributeValue> computeAttribute(std::vector<positions>, Attribute);
      
    private:
      std::vector<SphCell> cells;
      entityValue cutoff;
      SmoothingKernel kernel;

      SphCell getNeighbour(coordinate, coordinate);
  }

  class SphCell
  {
    public:
      sphCell();
      ~sphCell();

      /*
      Function that returns the desired attribute at given positions

      Arguments:
      1. A vector of points where the desired attribute is computed
      2. The attribute that should be computed {example Attribute::density() }
      3. A kernel that specifies the contribution of the particles around the desired position
      */
      std::vector<attributeValue> computeAttribute(std::vector<position>, Attribute, SmoothingKernel) const;

      /*
      Function that makes an update step for each particle in the cell

      Arguments:
      1. Timestep deltaT
      */
      void update(entityValue);
      
    private:
      std::vector<velocity> vel;
      std::vector<position> pos;
      std::vector< std::array<attributeValue, Attribute::getNumAttributes()> > attributes;
      entityValue boxSize;

      void makeTransitions();
      void computeForces();
      void updatePositions();
      void updateVelocities(); 
  }
}     
