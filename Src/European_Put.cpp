/**
* Project:    Project 2
* Filename:   European_Put.cpp
* Version:    v1 (12 April 2020)
* Author:     Ryan Sephton
* Summary:    Child Class that stores and prices European put options.
*/


#include "European_Put.h"

/**
* Constructor to class European Put. Used to store various methods to help price any european put option.
* @param vol const double reference, denotes the volatility of the market, for the option.
* @param strike const double reference, denotes the exercise price of the option.
* @param interest_rate const double reference, denotes the interest rate of the market, for the option.
* @param S const float reference, denotes the value of the unerlying at t=0.
* @param expiry const float reference, denotes the expiry date for the option, where 1 year => expiry = 365.
*/
European_Put::European_Put(const double& vol, const double& strike, const double& interest_rate, const double& S, const float& expiry)
	:European_Vanilla_Option(vol, strike, interest_rate, S, expiry)
{
	option_price = price_option_analytic(); // Default Pricing Method
}


/**
* Function to compute the payoff of the european put option
* in diffusion equation variables. See accompanying pdf for details.
*/
double European_Put::transformed_payoff(const double& x, const double& tau)
{
	double u = -1*(exp(((k + 1)*x) / 2.) - exp(((k - 1)*x) / 2.));
	return(max(u, 0.));
}


/**
* Function to compute the analytic price for a european put option.
*/
double European_Put::price_option_analytic()
{
	// Price = -1* [SN(-d1) - E^(-rT)N(-d2) ]
	// Where N is the CDF of the standard normal distribution
	double price = -1 * (CDF_normal(-d1()) * underlying_price - CDF_normal(-d2()) * strike * exp(-r * (maturity)));
	option_price = float(price);
	return option_price;
}


/**
* Boundary condition for a european put option, in the approximate
* limit that S-> 0 (in diffusion parameters).
*/
double European_Put::value_at_N_minus(const double& x_min, const double& tau)
{
	double boundary_value = exp((((1-k)*x_min) / 2) + (((pow((k + 1), 2)/4) - k)*tau));
	return boundary_value;
}



/**
* Function to compute the binomial price of the european
* put option. Details for this calculation can be found
* in Maturity_Only_CRR_Tree.cpp
*/
double European_Put::get_binomial_price()
{
	payoffs_at_T();
	Maturity_Only_CRR_Tree binomial_tree = Maturity_Only_CRR_Tree(payoffs, u, d, p, r, n_steps, delta_t);
	return binomial_tree.get_price();
}


/**
* Function to compute the monte carlo price of the european
* put option. Details for this calculation can be found
* in Vanilla_Monte_Carlo_Pricer.cpp
*/
double European_Put::get_monte_carlo_price()
{
	Vanilla_Monte_Carlo_Pricer<European_Put> mc_solver = Vanilla_Monte_Carlo_Pricer<European_Put>(this, r, underlying_price, volatility, strike, maturity);
	return mc_solver.get_mc_price();
}
