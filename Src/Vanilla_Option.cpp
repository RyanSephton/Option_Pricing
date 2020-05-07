/**
* Project:    Project 2
* Filename:   Vanilla_Option.cpp
* Version:    v1 (12 April 2020)
* Author:     Ryan Sephton
* Summary:    Abstract Base Class That All Vanilla Options (European & American) are Derived From.
*/



#include "Vanilla_Option.h"


/**
* Constructor to class Vanilla Option. Used to store various helper methods to help price the options.
* Parent class to both "American_Vanilla_Option" and "European_Vanilla_Option", which in turn have their own child classes.
* @param vol const double reference, denotes the volatility of the market, for the option.
* @param strike_price const double reference, denotes the exercise price of the option.
* @param interest_rate const double reference, denotes the interest rate of the market, for the option.
* @param S const float reference, denotes the value of the unerlying at t=0.
* @param expiry const float reference, denotes the expiry date for the option, where 1 year => expiry = 365.
*/
Vanilla_Option::Vanilla_Option(const double& vol, const double& strike_price, const double& interest_rate, const double& S, const float& expiry)
	:Option(vol, strike_price, interest_rate, S, expiry) 
{
	u_at_t = std::vector<double>(2 * N_plus + 1); // Initialise container for using PDE method  MOVE!!!
}


/**
* Function to compute the perturbation needed to the finite difference mesh
* to ensure that the options underlying, lies on a grid point.
*/
double Vanilla_Option::get_grid_offset()
{
	double closest_n = log(underlying_price / strike) * (1 / dx);
	double offset = closest_n - round(closest_n);
	return offset;
}


/**
* Function to initialise the binomial tree parameters and get the 
* payoffs at maturity, ready to be back integrated.
*/
void Vanilla_Option::payoffs_at_T()
{
	calculate_CRR_params();

	std::vector<double> option_values((n_steps + 1), 0); // For tree of N steps, there are N+1 possible share prices

	for (int i = 0; i <= n_steps; i++)
	{
		option_values.at(i) = underlying_price * pow(u, (int(n_steps - i)))* pow(d, i); // s_M^i = s * u^(i-M) * d^(i)
		option_values.at(i) = payoff(option_values.at(i),strike); 
	}

	payoffs = option_values;
	return;
}
