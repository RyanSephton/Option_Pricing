/**
* Project:    Project 2
* Filename:   American_Vanilla_Option.cpp
* Version:    v1 (12 April 2020)
* Author:     Ryan Sephton
* Summary:    Abstract Base Class That All American Options are Derived From.
*/



#include "American_Vanilla_Option.h"




/**
* Function to perform the projected successive over-relaxation method
* for an american option in linear complementary form. See the 
* accompanying PDF or Wilmott's reference for details.
*/
unsigned int American_Vanilla_Option::psor_solver(const std::vector<double>& b, const std::vector<double> & g, const double& omega, const unsigned int& n_iterations)
{
	// Initialise containers
	unsigned int iterations{ 0 };
	double error{ 100 };
	double y{ 0 };

	// Set Boundary Conditions in LCF
	u_at_t[0] = g[0];
	u_at_t[(2 * N_plus)] = g[(2 * N_plus)];

	while (error > tolerance)
	{
		error = 0.;
		y = 0;
		for (int i = 1; i < (2 * N_plus); i++)
		{
			// Step along, using prior and successive point
			y = (b[i] + ((alpha/2.)*(u_at_t[i - 1] + u_at_t[i + 1]))) / (1 + alpha);
			y = max(g[i], u_at_t[i] + (omega*(y - u_at_t[i]))); // Ensure 1 sided free boundary constraint is met
			error += (u_at_t[i] - y) * (u_at_t[i] - y);
			u_at_t[i] = y;
		}
		iterations++;
	}
	return(iterations);
}




/**
* Function to perform an implicit finite
* difference method on american options, using the
* PSOR algorithm above.
*/
double American_Vanilla_Option::implicit_fdm()
{

	// Set solver parameters (mostly heuristic)
	double tau{ 0 };
	double omega{ 1.0 };
	double d_omega{ 0.05 };
	unsigned int n_iterations{ 10000 };
	unsigned int prev_n_iterations{ 10000 };
	double grid_offset = get_grid_offset();

	std::vector<double> b(int((2 * N_plus) + 1));
	std::vector <double> g{ u_at_t };

	// Evaluate u at payoff
	for (int i = N_minus; i <= N_plus; i++)
	{
		u_at_t[i - N_minus] = transformed_payoff((i + grid_offset)*dx);
	}


	// Step Chronologically
	for (int m = 1; m <= n_t_steps; m++)
	{
		tau = m*dt;

		for (int i = (1 + N_minus); i < N_plus; i++)
		{
			g[i - N_minus] = transformed_payoff(((i + grid_offset)*dx), tau);
			b[i - N_minus] = u_at_t[i - N_minus] + ((alpha/2.)*(u_at_t[i - N_minus+1] - 2* u_at_t[i - N_minus] + u_at_t[i - N_minus-1]));
		}

		g[0] = transformed_payoff(((N_minus + grid_offset)*dx), tau);
		g[(2 * N_plus)] = transformed_payoff(((N_plus + grid_offset)*dx), tau);
		u_at_t[0] = g[0];
		u_at_t[2 * N_plus] = g[2 * N_plus];
		n_iterations = psor_solver(b,g, omega, n_iterations);

		// If more iterations are needed, drop the value of omega, else increase
		if (n_iterations > prev_n_iterations)
		{
			d_omega *= -1.0;
		}

		omega += d_omega;
		prev_n_iterations = n_iterations;


	}

	// Convert from diffusion variables in linear complimentary form, to physical parameters
	double n = int(log(underlying_price / strike) * (1 / dx));
	double u = u_at_t[int(n - N_minus)];
	double price = strike*u * exp((((1 - k)*n*dx) / 2) - ((pow(k + 1, 2)* maturity*pow(volatility, 2)) / 8));
	return price;
}