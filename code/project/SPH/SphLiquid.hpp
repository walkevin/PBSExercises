#ifndef SPH_LIQUID_HPP
#define SPH_LIQUID_HPP

#include "SphBase.hpp"

namespace sph
{
  class SphLiquid
  {
    public:
      virtual attributeValue getAttribute(Attribute) = 0;
  };

  class SphWater : public SphLiquid
  {
    public:
      virtual attributeValue getAttribute(Attribute);

    private:
      static constexpr attributeValue restDensity = 1000;
      static constexpr attributeValue mass = 1;
      static constexpr attributeValue volume = 1000;
      static constexpr attributeValue viscosity = 0.05;
  };
} 

#endif
