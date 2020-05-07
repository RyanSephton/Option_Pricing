/**
* Project:    Project 2
* Filename:   European_Vanilla_Option.cpp
* Version:    v1 (12 April 2020)
* Author:     Ryan Sephton
* Summary:    Abstract Base Class to implement European Vanilla Option pricing methods.
*/



#include "European_Vanilla_Option.h"


/**
* Function to compute the CDF of the standard normal distribution.
*/
double European_Vanilla_Option::CDF_normal(double x)
{
	return 0.5 * erfc(-x / sqrt(2));
}



/**
* Function to perform an implicit finite
* difference method on european options, using the
* SOR algorithm below.
*/
double European_Vanilla_Option::implicit_fdm()
{

	// Set solver parameters (mostly heuristic)
	double tau{ 0 };
	double omega{ 1.0 };
	double d_omega{ 0.05 };
	unsigned int n_iterations{ 10000 };
	unsigned int prev_n_iterations{ 10000 };
	double grid_offset = get_grid_offset();

	std::vector<double> b(int((2 * N_plus) + 1));

	// Evaluate u at payoff (stepping spacially)
	for (int i = N_minus; i <= N_plus; i++)
	{
		u_at_t[i-N_minus] = transformed_payoff((i + grid_offset)*dx);
	}


	// Now step from boundary, chronologically
	for (int m = 1; m <= n_t_steps; m++)
	{
		tau = m*dt;

		for (int i = 1; i < (N_plus - N_minus); i++)
		{
			b[i] = u_at_t[i];
		}

		u_at_t[0] = value_at_N_minus(((N_minus + grid_offset)*dx), tau);
		u_at_t[(2*N_plus)] = value_at_N_plus(((N_plus + grid_offset)*dx), tau);

		n_iterations = sor_solver(b,omega, n_iterations);

		// If more iterations are needed- decrease omega, else increase it
		if (n_iterations > prev_n_iterations)
		{
			d_omega *= -1.0;
		}

		omega += d_omega;
		prev_n_iterations = n_iterations;

	}

	// Convert from diffusion parameters, back to physical parameters
	double n = int(log(underlying_price / strike) * (1 / dx));
	double u = u_at_t[ int(n- N_minus)];
	double price = strike*u * exp((((1 - k)*n*dx) / 2) - ((pow(k + 1, 2)* maturity*pow(volatility, 2)) / 8));
	return price;
}


/**
* Function to perform the successive over-relaxation method
* for a european option in diffusion equation form. See the
* accompanying PDF or Wilmott's reference for details.
*/
unsigned int European_Vanilla_Option::sor_solver(const std::vector<double>& b, const double& omega, const unsigned int& n_iterations)
{
	// Initialise containers
	unsigned int iterations{ 0 };
	double error{ 100 };
	double y{ 0 };
	while(error > tolerance)
	{
		error = 0.;
		y = 0;
		for (int i = 1; i < (2*N_plus); i++)
		{
			// Step along, using prior and successive point
			y = (b[i] + (alpha*(u_at_t[i - 1] + u_at_t[i + 1]))) / (1 + (2 * alpha));
			y = u_at_t[i] + (omega*(y - u_at_t[i]));
			error += (u_at_t[i] - y) * (u_at_t[i] - y);
			u_at_t[i] = y;
		}
		iterations++;
	}
	return(iterations);
}



/**
* Function to perform an explicit finite
* difference method on european options. This algorithm
* is inherently unstable for alpha>=0.5, but it is faster than
* the SOR method for european options.
*/
double European_Vanilla_Option::explicit_fdm()
{
	// Initialise containers and shift mesh
	std::vector<double> oldu((2 * N_plus) + 1);
	std::vector<double> newu((2 * N_plus) + 1);
	double offset = get_grid_offset();

	double tau = 0;

	// Evaluate u at payoff (stepping spacially)
	for (int i = N_minus; i <= N_plus; i++)
	{
		oldu[i - N_minus] = transformed_payoff((i + offset)*dx);
	}

	// Now step from boundary, chronologically
	for (int j = 1; j <= n_t_steps; j++)
	{
		tau = j*dt;

		newu[0] = value_at_N_minus(((N_minus + offset)*dx), tau);
		newu[2 * N_plus] = value_at_N_plus(((N_plus+offset)* dx), tau);

		for (int i = 1; i < (N_plus - N_minus); i++)
		{
			newu[i ] = oldu[i ] + (alpha*(oldu[i - 1] - (2 * oldu[i]) + oldu[i + 1]));
		}

		for (int i = 0; i <= (2 * N_plus); i++)
		{
			oldu[i] = newu[i];
		}
	}

	// Transform from diffusion variables to physical variables
	double n = int(log(underlying_price / strike) * (1 / dx));
	double u = oldu[int(n - N_minus)];
	double price = strike*u * exp((((1 - k)*n*dx) / 2) - ((pow(k + 1, 2)* maturity*pow(volatility, 2)) / 8));
	return price;
}
