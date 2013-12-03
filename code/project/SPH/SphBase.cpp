#include "SphBase.hpp"
#include <cmath>

namespace sph
{
  SmoothingKernel::SmoothingKernel(entityValue h)
  {
    this->h = h;
  }

  KernelPoly6::KernelPoly6(entityValue h)
  : SmoothingKernel(h)
  {
		coeff = 315/(64*M_PI*std::pow(h,9));
		h2 = h*h;
  }

  attributeValue KernelPoly6::operator()(position vec1, position vec2)
  {
    position diff = vec1 - vec2;
    diff = diff.cwiseProduct(diff);
    double distance2 = diff[0]+diff[1]+diff[2];
    if(distance2 > h2)
      return 0;
    double term = h2 - distance2;
    return coeff*term*term*term;
  }

  attributeValue KernelPoly6::grad(position vec1, position vec2)
  {
    position diff = vec1 - vec2;
    diff = diff.cwiseProduct(diff);
    double distance2 = diff[0]+diff[1]+diff[2];
    if(distance2 > h2)
      return 0;
    double term = h2 - distance2;
    double distance = std::sqrt(distance2);
    return coeff*(-6)*distance*term*term;
  }

  attributeValue KernelPoly6::laplace(position vec1, position vec2)
  {
    position diff = vec1 - vec2;
    diff = diff.cwiseProduct(diff);
    double distance2 = diff[0]+diff[1]+diff[2];
    if(distance2 > h2)
      return 0;
    return -6*coeff*(h2*h2 - 6*h2*distance2 + 5*distance2*distance2);
  }

	KernelSpiky::KernelSpiky(entityValue h)
  : SmoothingKernel(h)
  {
		coeff = 15/(M_PI*std::pow(h,6));
  }

	attributeValue KernelSpiky::operator()(position vec1, position vec2)
  {
    position diff = vec1 - vec2;
		double norm = diff.norm();
		if(norm > h)
			return 0;
    double term = h - norm;
    return coeff*term*term*term;
  }

	attributeValue KernelSpiky::grad(position vec1, position vec2)
  {
    return -3*operator()(vec1, vec2);
  }

	attributeValue KernelSpiky::laplace(position vec1, position vec2)
  {
    return 6*operator()(vec1, vec2);
  }
}
