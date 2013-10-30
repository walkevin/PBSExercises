#include "gauss_seidel.h"
#include "fluid2d.h"

#define GETF(x,y) field[(x) + (y) * xRes]
#define GETB(x,y) b[(x) + (y) * xRes]
#define A(x,y) (x) + (y) * xRes

double interp(int i, int j, bool xStag, bool yStag, double ax, double ay, double* f, int res);

// Problem 1
void ExSolvePoisson(int xRes, int yRes, int _iterations, double _accuracy, double* field, double* b)
{
	// note that the boundaries are handles by the framework, so you iterations should be similar to:
	for (int i = 0; i < _iterations; i++)
	{
		//Stop condition using maximum norm
		double maxResidual = 0;
		for (int y = 1; y < yRes - 1; y++) {
			for (int x = 1; x < xRes - 1; x++)
			{
				GETF(x, y) = 0.25*(GETB(x, y) + GETF(x + 1, y) + GETF(x - 1, y) + GETF(x, y + 1) + GETF(x, y - 1));
				 //residual for each element if deltat = deltax and rho = 1
				double residual = std::abs(GETB(x, y) - (4 * GETF(x, y) - GETF(x + 1, y) - GETF(x - 1, y) - GETF(x, y + 1) - GETF(x, y - 1)));
				if (residual > maxResidual)
					maxResidual = residual;
			}
		}
		if (maxResidual < _accuracy){
			printf("Pressure solver: iter=%d , res=%f \n", i, maxResidual);
			break;
		}
		else if (i == _iterations)
			printf("Pressure solver: iter=%d , res=%f \n", i, maxResidual);
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
	int xRes = _xRes;//terribly ugly, but needed
	int yRes = _yRes;//debug

	double deltax = 1 / double(_xRes);
	double deltay = 1 / double(_yRes);

	for (int j = 0; j < _yRes - 1; j++)
	{
		for (int i = 0; i < _xRes - 1; i++)
		{
			_xVelocity[A(i + 1, j)] = _xVelocity[A(i + 1, j)] - _dt*(_pressure[A(i + 1, j)] - _pressure[A(i, j)]) * double(_xRes);
			_yVelocity[A(i, j + 1)] = _yVelocity[A(i, j + 1)] - _dt*(_pressure[A(i, j + 1)] - _pressure[A(i, j)]) * double(_yRes);
		}
	}
}

// Problem 3
void ExAdvectWithSemiLagrange(int xRes, int yRes, double dt,double* xVelocity, double* yVelocity, double *density, double *densityTemp, double *xVelocityTemp,double *yVelocityTemp)
{
	// note: velocity u_{i+1/2} is practically stored at i+1
	//Initialize to zero.
	for (int i = 0; i < xRes*yRes; i++)
	{
		xVelocityTemp[i] = 0;
		yVelocityTemp[i] = 0;
	}

	for (int i = 0; i < xRes - 1; i++)
	{
		for (int j = 0; j < yRes - 1; j++)
		{
			//Compute distance to departure point
			double advx = 0.5* (xVelocity[A(i,j)] + xVelocity[A(i+1,j)]);
			double advy = 0.5* (yVelocity[A(i,j)] + yVelocity[A(i,j+1)]);
			
			double ax = -dt * advx * static_cast<double>(xRes); //A negative ax => ax + 0.5 to account for staggering. Otherwise, ax > 0 => ax - 0.5
			double ay = -dt * advy * static_cast<double>(yRes);


			//Compute interpolation for u (=> staggered in x resp. i, unstaggered in y resp. j)
			double interp_u = interp(i, j, true, false, ax, ay, xVelocity, xRes);

			//Distribute value to neighboring cells. "undo initial interpolation"
			xVelocityTemp[A(i,j)]       += 0.5*interp_u;
			xVelocityTemp[A(i+1,j)] += 0.5*interp_u;

			//Compute interpolation for v (=> unstaggered in x resp. i, staggered in y resp. j)
			double interp_v = interp(i, j, false, true, ax, ay, yVelocity, xRes);

			//Distribute value to neighboring cells. "undo initial interpolation"
			yVelocityTemp[A(i,j)]   += 0.5*interp_v;
			yVelocityTemp[A(i,j+1)] += 0.5*interp_v;

			//Compute interpolation for rho (=> unstaggered in x resp. i, unstaggered in y resp. j)
			double interp_rho = interp(i, j, false, false, ax, ay, density, xRes);

			//Distribute value to neighboring cells. "undo initial interpolation"
			densityTemp[A(i,j)]   = interp_rho;
		}
	}

	//"Swap" buffers
	for (int i = 0; i < xRes*yRes; i++)
	{
		xVelocity[i] = xVelocityTemp[i];
		yVelocity[i] = yVelocityTemp[i];
		density[i] = densityTemp[i];
	}
}

double interp(int i, int j, bool xStag, bool yStag,double ax, double ay, double* f, int xRes){
	double xst = 0., yst = 0.;
	if (xStag)
		xst = 0.5;
	if (yStag)
		yst = 0.5;

	//Compute interpolation indices
	//i-direction
	int ip0 = i + std::floor(ax + xst);
	int ip1 = ip0 + 1;
	double iw1 = ax + xst - std::floor(ax + xst);
	double iw0 = 1 - iw1;

	//j-direction
	int jp0 = j + std::floor(ay + yst);
	int jp1 = jp0 + 1;
	double jw1 = ay + yst - std::floor(ay + yst);
	double jw0 = 1 - jw1;

	//Perform bilinear interpolation for v (at center of cell)
	return            iw0 * (jw0 * f[A(ip0,jp0)]
		                   + jw1 * f[A(ip0,jp1)])
		            + iw1 * (jw0 * f[A(ip1,jp0)]
		                   + jw1 * f[A(ip1,jp1)]);	
}

