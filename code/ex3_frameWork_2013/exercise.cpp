#include "gauss_seidel.h"
#include "fluid2d.h"

#define GETF(x,y) field[(x) + (y) * xRes]
#define GETB(x,y) b[(x) + (y) * xRes]

double interp(double x1, double y1, double x2, double y2, double xip);

// Problem 1
void ExSolvePoisson(int xRes, int yRes, int _iterations, double _accuracy, double* field, double* b)
{
	// note that the boundaries are handles by the framework, so you iterations should be similar to:
	for (int i = 0; i < _iterations; i++)
	{
		//Stop condition using maximum norm
		double maxResidual = 0.;
		for (int y = 1; y < yRes - 1; y++) {
			for (int x = 1; x < xRes - 1; x++)
			{
				GETF(x, y) = 0.25*(GETB(x, y) + GETF(x + 1, y) + GETF(x - 1, y) + GETF(x, y + 1) + GETF(x, y - 1));
				if (i == _iterations - 1) //residual for each element if deltat = deltax and rho = 1
				{
					double residual = GETB(x, y) - (4 * GETF(x, y) - GETF(x + 1, y) - GETF(x - 1, y) - GETF(x, y + 1) - GETF(x, y - 1));
					printf("Pressure solver: iter=%d , res=%f \n", i, residual);
					if (residual > maxResidual)
						maxResidual = residual;
				}
			}
		}
		if (maxResidual > _accuracy){
			printf("Pressure solver: iter=%d , res=%f \n", i, maxResidual);
			break;
		}
	}

	// for your debugging, and ours, please add these prints after every iteration
	/*if(iter==_iterations-1) 
		printf("Pressure solver: iter=%d , res=%f \n",iter, residual);
	if(residual<_accuracy) {
		printf("Pressure solver: iter=%d , converged \n",iter,residual);
		break; // optional
	} */

}

// Problem 2
void ExCorrectVelocities(int _xRes, int _yRes, double _dt, const double* _pressure, double* _xVelocity, double* _yVelocity)
{
	double deltax = 1 / double(_xRes);
	double deltay = 1 / double(_yRes);
	for (int j = 0; j < _xRes-1; j++)
	{
		for (int i = 0; i < _yRes-1; i++)
		{
			_xVelocity[i+1 + j*_xRes] = _xVelocity[i+1 + j*_xRes] - _dt*(_pressure[(i + 1) + j*_xRes] - _pressure[i + j*_xRes])/deltax;
			_yVelocity[i + (j+1)*_xRes] = _yVelocity[i + (j+1)*_xRes] - _dt*(_pressure[i + (j+1)*_xRes] - _pressure[i + j*_xRes])/deltay;
		}
	}	
}

// Problem 3
void ExAdvectWithSemiLagrange(int xRes, int yRes, double dt,double* xVelocity, double* yVelocity, double *density, double *densityTemp, double *xVelocityTemp,double *yVelocityTemp)
{
	// note: velocity u_{i+1/2} is practically stored at i+1
}

double interp(double x1, double y1, double x2, double y2, double xip)
{
	double deltax = std::abs(x2 - x1);
	double dis1 = std::abs(xip - x1);
	return dis1 / deltax*y1 + (1 - dis1) / deltax* y2;
}
