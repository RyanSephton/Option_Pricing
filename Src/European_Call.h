/**
* Project:    Project 2
* Filename:   European_Call.h
* Version:    v1 (12 April 2020)
* Author:     Ryan Sephton
* Summary:    Child Class that stores and prices European call options.
*/



#pragma once
#include "European_Vanilla_Option.h"


class European_Call :
	public European_Vanilla_Option
{
private:

	// Methods
	double payoff(const double& S, const double& K) { return max(double(S - K), 0.); };

	// PDE Methods
	double transformed_payoff(const double& x, const double& tau=0);

	// Analytic Methods
	double price_option_analytic();
	

	// PDE Boundary Conditions
	double value_at_N_minus(const double& x_max, const double& tau) { return 0.; }; // Boundary Conditions
	double value_at_N_plus(const double& x_max, const double& tau);
public:

	// Constructor & Destructor
	European_Call(const double& vol, const double& strike, const double& interest_rate, const double& S, const float& expiry);
	~European_Call() {};


	// Methods
	double get_binomial_price();
	double get_monte_carlo_price();

};

