/**
* Project:    Project 2
* Filename:   American_Call.h
* Version:    v1 (12 April 2020)
* Author:     Ryan Sephton
* Summary:    Child Class that stores and prices American call options.
*/



#pragma once
#include "American_Vanilla_Option.h"
class American_Call :
	public American_Vanilla_Option
{
private:

	// Methods
	double payoff(const double& S, const double& K) { return max(double(S - K), 0.); };

	// PDE Methods
	double transformed_payoff(const double& x, const double& tau = 0);

public:

	// Contructor & Destructor
	American_Call(const double& vol, const double& strike, const double& interest_rate, const double& S, const float& expiry);
	~American_Call() {};

	// Method
	double get_binomial_price();


};

