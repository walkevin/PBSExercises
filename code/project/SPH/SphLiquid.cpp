#include "SphLiquid.hpp"

namespace sph
{
  attributeValue SphWater::getAttribute(Attribute attr)
  {
    switch(attr.getVal() )
    {
      case 1: 
        return restDensity;
      case 2:
        return mass;
      case 3:
        return volume;
      case 4:
        return viscosity;
    }
  }
}
