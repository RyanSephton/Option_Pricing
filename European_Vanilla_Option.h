/**
* Project:    Project 2
* Filename:   European_Vanilla_Option.h
* Version:    v1 (12 April 2020)
* Author:     Ryan Sephton
* Summary:    Abstract Base Class to implement European Vanilla Option pricing methods.
*/



#pragma once
#include "Vanilla_Option.h"


class European_Vanilla_Option: 
	public Vanilla_Option {

protected:

	// Methods

	// Analytic Price
	double CDF_normal(double x); // CDF of standard normal distribution
	double d1(void) { return 1.0 / volatility / sqrt(maturity) * (log(underlying_price / strike) + (r + volatility*volatility/ 2.0) * (maturity)); }; 
	double d2(void) { return d1() - volatility* sqrt(maturity); }; // See Paul Wilmott, Financial Derivatives
	virtual double price_option_analytic() = 0;

	//PDE Price
	virtual double value_at_N_minus(const double& x_min, const double& tau) = 0; // Implemented in European_Call and European_Put
	virtual double value_at_N_plus(const double& x_max, const double& tau) = 0;
	unsigned int sor_solver(const std::vector<double>& b, const double& omega, const unsigned int& n_iterations);
	double implicit_fdm();
	double explicit_fdm();


public:

	// Constructors & Destructor
	European_Vanilla_Option() 
		:Vanilla_Option() {};
	European_Vanilla_Option(const double& vol, const double& strike, const double& interest_rate, const double& S, const float& expiry)
		:Vanilla_Option(vol, strike, interest_rate, S, expiry) {};
	~European_Vanilla_Option() {};

	// Methods
	double get_analytic_price() { return option_price; };
	double get_pde_price() { return explicit_fdm(); };
	double get_pde_price_implicit() { return implicit_fdm();};
};