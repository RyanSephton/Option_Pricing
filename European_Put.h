/**
* Project:    Project 2
* Filename:   European_Put.h
* Version:    v1 (12 April 2020)
* Author:     Ryan Sephton
* Summary:    Child Class that stores and prices European put options.
*/



#pragma once
#include "European_Vanilla_Option.h"
class European_Put :
	public European_Vanilla_Option
{
private:
	// Methods
	double payoff(const double& S, const double& K) { return max(double(K - S), 0.); };

	// PDE Methods
	double transformed_payoff(const double& x, const double& tau = 0);


	// Analytic Methods
	double price_option_analytic();


	// PDE Boundary Conditions
	double value_at_N_minus(const double& x_min, const double& tau);
	double value_at_N_plus(const double& x_max, const double& tau) {return 0.;};

public:

	// Constructor & Destructor
	European_Put(const double& vol, const double& strike, const double& interest_rate, const double& S, const float& expiry);
	~European_Put() {};

	// Methods
	double get_binomial_price();
	double get_monte_carlo_price();
};

