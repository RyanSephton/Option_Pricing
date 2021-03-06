/**
* Project:    Project 2
* Filename:   American_Put.cpp
* Version:    v1 (12 April 2020)
* Author:     Ryan Sephton
* Summary:    Child Class that stores and prices American put options.
*/



#include "American_Put.h"


/**
* Constructor to class American Put. Used to store various methods to help price any american put option.
* @param vol const double reference, denotes the volatility of the market, for the option.
* @param strike const double reference, denotes the exercise price of the option.
* @param interest_rate const double reference, denotes the interest rate of the market, for the option.
* @param S const float reference, denotes the value of the unerlying at t=0.
* @param expiry const float reference, denotes the expiry date for the option, where 1 year => expiry = 365.
*/
American_Put::American_Put(const double& vol, const double& strike, const double& interest_rate, const double& S, const float& expiry)
	:American_Vanilla_Option(vol, strike, interest_rate, S, expiry)
{
	option_price = get_binomial_price(); // Default Option Pricing
}



/**
* Function to compute the payoff of the american put option
* in diffusion equation variables. See accompanying pdf for details.
*/
double American_Put::transformed_payoff(const double& x, const double& tau)
{
	double discount_factor = exp((pow(k + 1, 2)*tau) / 4);
	double u = -1*(exp(((k + 1)*x) / 2.) - exp(((k - 1)*x) / 2.));

	return(max((u*discount_factor), 0.));
}


/**
* Function to compute the binomial price of the american 
* put option. Details for this calculation can be found
* in Early_Exercise_CRR_Tree.cpp
*/
double American_Put::get_binomial_price()
{
	payoffs_at_T(); 
	Early_Exercise_CRR_Tree<American_Put> binomial_tree = Early_Exercise_CRR_Tree<American_Put>(this, payoffs, u, d, p, r, n_steps, delta_t);
	return binomial_tree.get_price();
}

