#ifndef SPH_BASE_HPP
#define SPH_BASE_HPP

#include <Eigen/Core>
#include <vector>
#include <array>

namespace sph
{
  class SphSolver;
  class SphCell;

  typedef Eigen::Vector3d velocity;
  typedef Eigen::Vector3d position;
  typedef Eigen::Vector3d force;
  typedef Eigen::Array<float, 4, 1> homogeneousPosition;
  typedef Eigen::Array<int, 3, 1> coordinate;
  typedef int discreteValue;
  typedef double attributeValue;
  typedef double entityValue;
  typedef std::array<attributeValue, 3> attributes;

  class Attribute
  {
    public:
      static Attribute restDensity() {return Attribute(1); };
      static Attribute mass() {return Attribute(2); };
      static Attribute volume() {return Attribute(3); };
      static Attribute viscosity() {return Attribute(4); };
      int getVal() {return attribute; };
    
    private:
      explicit Attribute(int attribute) {this->attribute = attribute; };
      int attribute;
      static std::size_t const numAttributes = 3;
  };

  class SmoothingKernel
  {
    public:
      SmoothingKernel(entityValue);
      virtual attributeValue operator()(position, position) = 0;
      virtual attributeValue grad(position, position) = 0;
      virtual attributeValue laplace(position, position) = 0;

    protected:
      entityValue h2;
      entityValue coeff;
  };

  class KernelPoly6 : public SmoothingKernel
  {
    public:
      KernelPoly6(entityValue);
      virtual attributeValue operator()(position, position);
      virtual attributeValue grad(position, position);
      virtual attributeValue laplace(position, position);
  };
}     

#endif
