#pragma once
#include "Asian_Option.h"


/**
* Project:    Project 2
* Filename:   Asian_Call.h
* Version:    v1 (12 April 2020)
* Author:     Ryan Sephton
* Summary:    Child Class That Stores An Average Rate European Call Option.
*/

class Asian_Call :
	public Asian_Option
{
private:


	// Methods
	double payoff(const double& S, const double& K) { return max(double(S - K), 0.); };
	double price_option_analytic();

public:

	// Constructors & Destructors
	Asian_Call(const double& vol, const double& strike, const double& interest_rate, const double& S, const float& expiry);
	~Asian_Call() {};


	// Methods
	double get_binomial_price();
	double get_monte_carlo_price();
	double get_analytic_price() { return option_price; };
};

