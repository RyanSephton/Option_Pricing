/**
* Project:    Project 2
* Filename:   Option.cpp
* Version:    v1 (12 April 2020)
* Author:     Ryan Sephton
* Summary:    Abstract Base Class That All Options are Derived From.
*/



#include "Option.h"

/**
* Constructor to class Option. Used to store parameters associated with any options and various virtual methods to help price them.
* Parent class to both "Asian_Option" and "Vanilla_Option" which in turn, have child classes.
* @param vol const double reference, denotes the volatility of the market, for the option.
* @param strike_price const double reference, denotes the exercise price of the option.
* @param interest_rate const double reference, denotes the interest rate of the market, for the option.
* @param S const float reference, denotes the value of the unerlying at t=0.
* @param expiry const float reference, denotes the expiry date for the option, where 1 year => expiry = 365.
*/
Option::Option(const double& vol, const double& strike_price, const double& interest_rate, const double& S, const float& expiry)
{
	if (vol < 0 || vol > 1 || strike_price < 0 || interest_rate >1 || interest_rate < 0 || S < 0 || expiry < 0)
	{
		throw 1; // Error thrown if illogical parameter values are used, handled at main
	}

	// Update Members
	volatility = vol;
	strike = strike_price;
	r = interest_rate;
	underlying_price = S;
	maturity = double(expiry/365.);
}


/**
* Function to compute the various parameters needed to build the
* Binomial pricing method for the option, see C/rR_Tree for reference.
*/
void Option::calculate_CRR_params()
{
	// Calculate parameters of the Cox, Ross & Rubenstein binomial tree
	// Reference:https://en.wikipedia.org/wiki/Binomial_options_pricing_model

	// Calculate Tree Params
	double t_step = (float)maturity / (float)n_steps;
	double up_probability = exp(volatility * sqrt(t_step));
	double down_probability = 1 / up_probability;
	double probability = (exp(r * t_step) - down_probability) / (up_probability - down_probability);


	//Update members
	u = float(up_probability);
	d = float(down_probability);
	p = float(probability);
	delta_t = float(t_step);

	return;
}
