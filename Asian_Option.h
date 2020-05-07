/**
* Project:    Project 2
* Filename:   Asian_Option.h
* Version:    v1 (12 April 2020)
* Author:     Ryan Sephton
* Summary:    Abstract Base Class That All Asian Options are Derived From.
*/


#pragma once
#include "Option.h"
class Asian_Option :
	public Option
{
protected:
	// Analytic (Approximate) Pricing Attributes
	double m1{ 0 };
	double m2{ 0 };
	double d1{ 0 };
	double d2{ 0 };
	double sigma_a2{ 0 };
	double CDF_normal(const double& x);


	virtual double price_option_analytic() = 0;

public:
	// Constructor & Destructor
	Asian_Option(const double& vol, const double& strike, const double& interest_rate, const double& S, const float& expiry);
	~Asian_Option() {};

	// Methods
	virtual double get_analytic_price() = 0;
};

