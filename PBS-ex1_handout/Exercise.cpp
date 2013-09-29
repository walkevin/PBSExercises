#include <iostream>
using namespace std;
#include "Vec2.h"
#include "LinSys.h"
#include "Scene.h"

// gravitational acceleration (9.81)
static const double g = 9.81f;


// Exercise 1
// hanging mass point
void AdvanceTimeStep1(double k, double m, double d, double L, double dt, int method, double p1, double v1, double& p2, double& v2)
{
	//switch (method){
	//case scene::method::euler:

	//}
	p2 = p2 + dt*v2;
	v2 = v2 + dt*(-k*(p2 - L) - d*v2 + m*g ) / m;
}


// Exercise 3
// falling triangle
void AdvanceTimeStep3(double k, double m, double d, double L, double dt,
                      Vec2& p1, Vec2& v1, Vec2& p2, Vec2& v2, Vec2& p3, Vec2& v3)
{
   p1 += Vec2(1,1);
}
