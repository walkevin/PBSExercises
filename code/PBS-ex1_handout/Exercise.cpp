#include <iostream>
using namespace std;
#include "Vec2.h"
#include "LinSys.h"
#include "Scene.h"
#include <cassert>

// gravitational acceleration (9.81)
static const double g = 9.8066;


inline double f(double p, double v, double k, double d, double L, double m){
	//k*(-p - L) == -k*(p - (-L)) == k*(abs(p) -L) Last equality only holds for p < 0.
	//In fact abs(p) would be nice, since it denotes a length, and by definition of a spring, it can only be elongated in one direction.
	//However, abs(p) is not so nice if we want to solve for p in the implicit euler and we just want to be consistent with this RHS.
	//assert(p < 0);
	return (k*(-p - L) - d*v) / m -g;
}
// Exercise 1
// hanging mass point
void AdvanceTimeStep1(double k, double m, double d, double L, double dt, int method, double p1, double v1, double& p2, double& v2)
{
	//assert(p2 < p1);
	switch (method){
	case Scene::Method::EULER:
		{
			double tmp = p2;
			p2 = p2 + dt*v2;
			v2 = v2 + dt*f(tmp, v2, k, d, L, m);
		}
		break;
	case Scene::Method::LEAP_FROG:
		{
			v2 = v2 + dt*f(p2, v2, k, d, L, m);
			p2 = p2 + dt*v2;
		}
		break;
	case Scene::Method::MIDPOINT:
		{
			double k1_v = v2 + 0.5*dt*f(p2, v2, k, d, L, m);
			double k1_p = p2 + 0.5*dt*v2;

			p2 = p2 + dt * k1_v; 
			v2 = v2 + dt * f(k1_p, k1_v, k, d, L, m);
		}
		break;
	case Scene::Method::BACK_EULER:
		{
			v2 = (v2 - dt*(k/m * (p2 + L) - g)) / (1 + dt/m * (k*dt + d) );
			p2 = p2 + dt * v2;
		}
		break;
	case Scene::Method::ANALYTIC:
		{
			double const alpha = -0.5*d / m;
			double const beta = 0.5*sqrt(4 * k*m - d*d) / m;

			double const c1 = m*g/k;
			double const c2 = m*g/(beta*k) * (1-alpha) + L / beta;

			p2 = c1*exp(alpha*dt)*cos(beta*dt) + c2*exp(alpha*dt)*sin(beta*dt) - L - m*g / k;
		
			double const tmp = exp(alpha*dt);
			v2 = c1*tmp*(alpha*cos(beta*dt) - beta*sin(beta*dt)) + c2*tmp*(alpha*sin(beta*dt) + beta*cos(beta*dt));
		}
		break;

	
	}

}


// Exercise 3
// falling triangle
void AdvanceTimeStep3(double k, double m, double d, double L, double dt,
                      Vec2& p1, Vec2& v1, Vec2& p2, Vec2& v2, Vec2& p3, Vec2& v3)
{
	//Update velocities
	//Define springs sprXY is a spring from node X to node Y
	Vec2 spr12 = p2 - p1;
	Vec2 spr23 = p3 - p2;
	Vec2 spr13 = p3 - p1;

	double lenSpr12 = spr12.length();
	double lenSpr23 = spr23.length();
	double lenSpr13 = spr13.length();

	//Define force vectors. forceXY points from node X to node Y
	Vec2 force12 = (lenSpr12 - L) / lenSpr12 * spr12;
	Vec2 force13 = (lenSpr13 - L) / lenSpr13 * spr13;
	Vec2 force23 = (lenSpr23 - L) / lenSpr23 * spr23;

	//Be careful about direction of force vectors!
	v1 += dt/m *(-k*(force12 + force13) - d*v1) - dt*Vec2(0., g);
	v2 += dt/m *(k*(force12 - force23) - d*v2) - dt*Vec2(0., g);
	v3 += dt/m *(k*(force13 + force23) - d*v3) - dt*Vec2(0., g);

	//Update positions
	p1 += dt * v1;
	p2 += dt * v2;
	p3 += dt * v3;

	//Check for ground collision and add penalty force if needed
	double const kG = 200.;
	double const yG = -1.;
	Vec2 normalG = Vec2(0., 1.);
	if (p1.y <= yG)
		v1 += dt*kG / m*(yG - p1.y)*normalG;
	if (p2.y <= yG)
		v2 += dt*kG / m*(yG - p2.y)*normalG;
	if (p3.y <= yG)
		v3 += dt*kG / m*(yG - p3.y)*normalG;
}
