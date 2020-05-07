/**
* Project:    Project 2
* Filename:   American_Put.h
* Version:    v1 (12 April 2020)
* Author:     Ryan Sephton
* Summary:    Child Class that stores and prices American put options.
*/


#pragma once
#include "American_Vanilla_Option.h"
class American_Put :
	public American_Vanilla_Option
{
private:

	// Methods

	double payoff(const double& S, const double& K) {return max(double(K - S), 0.);};

	// PDE Method
	double transformed_payoff(const double& x, const double& tau = 0);

public:
	// Constructor & Destructor
	American_Put(const double& vol, const double& strike, const double& interest_rate, const double& S, const float& expiry);
	~American_Put() {};

	// Method
	double get_binomial_price();
};

