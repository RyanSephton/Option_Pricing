/**
* Project:    Project 2
* Filename:   American_Vanilla_Option.h
* Version:    v1 (12 April 2020)
* Author:     Ryan Sephton
* Summary:    Abstract Base Class That All American Options are Derived From.
*/


#pragma once
#include "Vanilla_Option.h"

class American_Vanilla_Option :
	public Vanilla_Option
{
protected:

	// Methods

	//PDE Price
	unsigned int psor_solver(const std::vector<double>& b, const std::vector<double>& g,const double& omega, const unsigned int& n_iterations);
	double implicit_fdm();



public:

	// Constructor & Destructor
	American_Vanilla_Option(const double& vol, const double& strike, const double& interest_rate, const double& S, const float& expiry)
		: Vanilla_Option(vol, strike, interest_rate, S, expiry){};
	~American_Vanilla_Option() {};

	// Methods
	double get_pde_price() { return implicit_fdm(); };
	double get_monte_carlo_price() { return 0; } //LONGSTAFF SHWARZ
};

