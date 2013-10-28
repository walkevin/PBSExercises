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
		if (maxResidual < _accuracy){
			printf("Pressure solver: iter=%d , res=%f \n", i+1, maxResidual);
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
	for (int i = 0; i < _xRes-1; i++)
	{
		for (int j = 0; j < _yRes-1; j++)
		{
			_xVelocity[(i+1)*_xRes + j] = _xVelocity[(i+1)*_xRes + j] - _dt*(_pressure[(i + 1)*_xRes + j] - _pressure[i*_xRes + j])/deltax;
			_yVelocity[i*_xRes + j + 1] = _yVelocity[i*_xRes + j + 1] - _dt*(_pressure[i*_xRes + j + 1] - _pressure[i*_xRes + j]) / deltay;
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
			double advx = 0.5*xVelocity[i*xRes + j] + 0.5*xVelocity[(i + 1)*xRes + j];
			double advy = 0.5*yVelocity[i*xRes + j] + 0.5*yVelocity[i*xRes + (j + 1)];
			
			double ax = -dt * advx * static_cast<double>(xRes); 
			double ay = -dt * advy * static_cast<double>(yRes);


			//Compute interpolation indices for u (=> staggered in x resp. i, unstaggered in y resp. j)
			//i-direction, staggered
			int ip0_u = i + std::floor(ax - 0.5);
			int ip1_u = ip0_u + 1;
			double iw1_u = ax - 0.5 - std::floor(ax - 0.5);
			double iw0_u = 1 - iw1_u;

			//j-direction, unstaggered
			int jp0_u = j + std::floor(ax);
			int jp1_u = jp0_u + 1;
			double jw1_u = ax - std::floor(ax);
			double jw0_u = 1 - jw1_u;

			//Perform bilinear interpolation for u (at center of cell)
			double interp_u = iw0_u * ( jw0_u * xVelocity[ip0_u * xRes + jp0_u]
									  + jw1_u * xVelocity[ip0_u * xRes + jp1_u])
					        + iw1_u * ( jw0_u * xVelocity[ip1_u * xRes + jp0_u]
								      + jw1_u * xVelocity[ip1_u * xRes + jp1_u]);

			//Distribute value to neighboring cells. "undo initial interpolation"
			xVelocityTemp[i*xRes + j] += 0.5*interp_u;
			xVelocityTemp[(i + 1)*xRes + j] += 0.5*interp_u;

			//Compute interpolation indices for v (=> unstaggered in x resp. i, staggered in y resp. j)
			//i-direction, staggered
			int ip0_v = i + std::floor(ay);
			int ip1_v = ip0_v + 1;
			double iw1_v = ay - std::floor(ay);
			double iw0_v = 1 - iw1_v;

			//j-direction, unstaggered
			int jp0_v = j + std::floor(ay - 0.5);
			int jp1_v = jp0_v + 1;
			double jw1_v = ay - 0.5 - std::floor(ay - 0.5);
			double jw0_v = 1 - jw1_v;

			//Perform bilinear interpolation for v (at center of cell)
			double interp_v = iw0_u * (jw0_v * yVelocity[ip0_v * yRes + jp0_v]
				                     + jw1_v * yVelocity[ip0_v * yRes + jp1_v])
				            + iw1_u * (jw0_v * yVelocity[ip1_v * yRes + jp0_v]
				                     + jw1_v * yVelocity[ip1_v * yRes + jp1_v]);
			
			//Distribute value to neighboring cells. "undo initial interpolation"
			yVelocityTemp[i*xRes + j] += 0.5*interp_v;
			yVelocityTemp[i*xRes + (j + 1)] += 0.5*interp_v;

			//double dispx = -dt*advx;
			//double dispy = -dt*advy;
			//double dx = 1 / double(xRes);
			//double dy = 1 / double(yRes);
			//int ind_i_u = i + std::floor((dispx + 0.5) / dx);
			//int ind_j_u = j + std::floor(dispy / dy);
			//double w_i_u = (dispx + 0.5) / dx - std::floor((dispx + 0.5) / dx);
			//double w_j_u = dispy / dy - std::floor(dispy / dy);
			//double int_temp1 = w_i_u*xVelocity[ind_i_u*xRes + ind_j_u] + (1 - w_i_u)*xVelocity[(ind_i_u+1)*xRes + ind_j_u];
			//double int_temp2 = w_i_u*xVelocity[ind_i_u*xRes + (ind_j_u+1)] + (1 - w_i_u)*xVelocity[(ind_i_u + 1)*xRes + (ind_j_u+1)];
			//double inp_u = w_j_u*int_temp1 + (1 - w_j_u)*int_temp2;

			//int ind_i_v = i + std::floor(dispx / dx);
			//int ind_j_v = j + std::floor((dispy + 0.5) / dy);
			//double w_i_v = dispx / dx - std::floor(dispx / dx);
			//double w_j_v = (dispy + 0.5) / dy - std::floor((dispy + 0.5) / dy);
			//int_temp1 = w_i_v*yVelocity[ind_i_v*xRes + ind_j_v] + (1 - w_i_v)*yVelocity[(ind_i_v + 1)*xRes + ind_j_v];
			//int_temp2 = w_i_v*yVelocity[ind_i_v*xRes + (ind_j_v + 1)] + (1 - w_i_v)*yVelocity[(ind_i_v + 1)*xRes + (ind_j_v + 1)];
			//double inp_v = w_j_v*int_temp1 + (1 - w_j_v)*int_temp2;

			//xVelocityTemp[i*xRes + j] += 0.5*inp_u;
			//xVelocityTemp[(i + 1)*xRes + j] += 0.5*inp_u;
			//yVelocityTemp[i*xRes + j] += 0.5*inp_v;
			//yVelocityTemp[i*xRes + (j + 1)] += 0.5*inp_v;
		}
	}

	//"Swap" buffers
	for (int i = 0; i < xRes*yRes; i++)
	{
		xVelocity[i] = xVelocityTemp[i];
		yVelocity[i] = yVelocityTemp[i];
	}
}
