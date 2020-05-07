/**
* Project:    Project 2
* Filename:   Asian_Option.cpp
* Version:    v1 (12 April 2020)
* Author:     Ryan Sephton
* Summary:    Abstract Base Class That All Options are Derived From.
*/


#include "Asian_Option.h"


/**
* Constructor to class Asian Option. Used to store various virtual methods to help price them any average rate european options.
* Parent class to both "Asian Call" and "Asia Put".
* @param vol const double reference, denotes the volatility of the market, for the option.
* @param strike const double reference, denotes the exercise price of the option.
* @param interest_rate const double reference, denotes the interest rate of the market, for the option.
* @param S const float reference, denotes the value of the unerlying at t=0.
* @param expiry const float reference, denotes the expiry date for the option, where 1 year => expiry = 365.
*/
Asian_Option::Asian_Option(const double& vol, const double& strike, const double& interest_rate, const double& S, const float& expiry)
	:Option(vol, strike, interest_rate, S, expiry)
{
	// Initialise Analytic Pricing Parameters (Turnbull Wakeman Approximation)
	double temp_1 = (2 * exp((2 * r + pow(volatility, 2))*maturity)*pow(underlying_price, 2)) / (((r + pow(volatility, 2))*((2 * r) + pow(volatility, 2))*pow(maturity, 2)));
	double temp_2 = ((2 * pow(underlying_price, 2)) / (r*pow(maturity, 2)))*(1 / (2 * r + pow(volatility, 2)) - (exp(r*maturity) / (r + pow(volatility, 2))));
	
	m1 = ((exp(r*maturity) - 1.) / (r*maturity))*underlying_price;
	m2 = temp_1 + temp_2;
	sigma_a2 = ((1 / maturity) * log(m2/pow(m1,2)));
	d1 = (log(m1 / strike) + (sigma_a2 * maturity / 2)) / (sqrt(sigma_a2*maturity));
	d2 = d1 - sqrt(sigma_a2 * maturity);


	// Binomial Tree Steps
	n_steps = int(expiry); // Approximation uses 1 step per day 
}


/**
* Function to compute the CDF of the standard normal distribution.
*/
double Asian_Option::CDF_normal(const double& x)
{
	return 0.5 * erfc(-x / sqrt(2));
}
