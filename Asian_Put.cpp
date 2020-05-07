/**
* Project:    Project 2
* Filename:   Asian_Call.cpp
* Version:    v1 (12 April 2020)
* Author:     Ryan Sephton
* Summary:    Child Class That Stores An Average Rate European Call Option.
*/




#include "Asian_Put.h"


/**
* Constructor to class Asian Put. Used to store various methods to help price any asian put option.
* @param vol const double reference, denotes the volatility of the market, for the option.
* @param strike const double reference, denotes the exercise price of the option.
* @param interest_rate const double reference, denotes the interest rate of the market, for the option.
* @param S const float reference, denotes the value of the unerlying at t=0.
* @param expiry const float reference, denotes the expiry date for the option, where 1 year => expiry = 365.
*/
Asian_Put::Asian_Put(const double& vol, const double& strike, const double& interest_rate, const double& S, const float& expiry)
	:Asian_Option(vol, strike, interest_rate, S, expiry)
{
	option_price = price_option_analytic();
};


/**
* Function to compute the monte carlo price of the average rate put option.
* For Specifics see Path_Dependent_Monte_Carlo_Pricer.cpp
*/
double Asian_Put::get_monte_carlo_price()
{
	Path_Dependent_Monte_Carlo_Pricer<Asian_Put> mc_solver = Path_Dependent_Monte_Carlo_Pricer<Asian_Put>(this, r, underlying_price, volatility, strike, maturity);
	return mc_solver.get_mc_price();
}


/**
* Function to compute the analytic price of the average rate put option.
* According to the Turnbull Wakeman Approximation.
*/
double Asian_Put::price_option_analytic()
{
	double price = -1*exp(-r*maturity)*((m1*CDF_normal(-1*d1)) - (strike*CDF_normal(-1*d2)));
	return price;
}


/**
* Function to compute the binomial price of the average rate put option.
* This uses the 1997 Hull-White linear interpolant approximation.
* See Path_Dependent_Tree or the accompanying pdf for details.
*/
double Asian_Put::get_binomial_price()
{
	calculate_CRR_params();
	Path_Dependent_Tree<Asian_Put>my_tree(this, u, d, p, r, int(maturity * 365));
	return(my_tree.get_price());
}
