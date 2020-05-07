
/**
* Project:    Project 2
* Filename:   Asian_Put.h
* Version:    v1 (12 April 2020)
* Author:     Ryan Sephton
* Summary:    Child Class That Stores An Average Rate European Put Option.
*/

#pragma once
#include "Asian_Option.h"
class Asian_Put :
	public Asian_Option
{
private:

	// Methods
	double payoff(const double& S, const double& K) { return max(double(K-S), 0.); };
	double price_option_analytic();


public:

	//Constructors & Destructors
	Asian_Put(const double& vol, const double& strike, const double& interest_rate, const double& S, const float& expiry);
	~Asian_Put() {};


	// Methods
	double get_binomial_price();
	double get_monte_carlo_price();
	double get_analytic_price() { return option_price; };
};
