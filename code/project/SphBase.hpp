#ifndef SPH_BASE_HPP
#define SPH_BASE_HPP

#include <Eigen/Core>
#include <vector>
#include <array>

namespace sph
{
  class Attribute
  {
    public:
      static Attribute density() {return Attribute(1); };
      static Attribute pressure() {return Attribute(2); };
      static Attribute volume() {return Attribute(3); };
      static int getNumAttributes() {return numAttributes; };
      int getVal() {return attribute; };
    
    private:
      explicit Attribute(int attribute) {this->attribute = attribute; };
      int attribute;
      static std::size_t const numAttributes = 3;
  };

  class SphSolver;
  class SphCell;

  typedef Eigen::Array<double, 3, 1> velocity;
  typedef Eigen::Array<double, 3, 1> position;
  typedef Eigen::Array<double, 3, 1> force;
  typedef Eigen::Array<int, 3, 1> coordinate;
  typedef double attributeValue;
  typedef double entityValue;
  typedef std::array<attributeValue, 3> attributes;
  typedef attributeValue (*SmoothingKernel)(position, position);

  class KernelPoly6
  {
    public:
      KernelPoly6(entityValue);
      attributeValue operator()(position, position);

    private:
      entityValue h2;
      entityValue coeff;
  };
}     

#endif
