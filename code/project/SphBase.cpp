#include "SphBase.hpp"
#include <cmath>

namespace sph
{
  KernelPoly6::KernelPoly6(entityValue h)
  {
    this->h2 = h*h;
    coeff = 315/(64*3.14*std::pow(h,9));
  }

  attributeValue KernelPoly6::operator()(position vec1, position vec2)
  {
    position diff = vec1 - vec2;
    diff = diff*diff;
    double distance2 = diff[0]+diff[1]+diff[2];
    if(distance2 > h2)
      return 0;
    double term = h2 - distance2;
    return coeff*term*term*term;
  }
}
