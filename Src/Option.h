/**
* Project:    Project 2
* Filename:   Option.h
* Version:    v1 (12 April 2020)
* Author:     Ryan Sephton
* Summary:    Abstract Base Class That All Options are Derived From.
*/

#pragma once
#include<vector>
#include <cmath>
#include "CRR_tree.h"
#include "Maturity_Only_CRR_Tree.h"
#include "Early_Exercise_CRR_Tree.h"
#include "Path_Dependent_Tree.h"
#include "Vanilla_Monte_Carlo_Pricer.h"
#include "Path_Dependent_Monte_Carlo_Pricer.h"

class Option
{
protected:
	// Attributes of options
	double strike{ 0. };
	double maturity{ 0 };
	double volatility{ 0 };
	double underlying_price{ 0 };
	double option_price{ 0 };
	double r{ 0 };

	// Attributes of binomial tree
	double u{ 0 };
	double d{ 0 };
	double p{ 0 };
	int n_steps{ 600 }; //Heuristic (overriden in path dependent options)
	double delta_t{ 0 };
	double binomial_price{ 0 };


	// Option Methods
	double max(const double& value1, const double& value2) { return (value1 > value2) ? value1 : value2; }; //used for payoffs
	virtual double payoff(const double& S, const double& K) = 0;


	// Pricing Methods
	virtual double get_monte_carlo_price() = 0;
	virtual double get_binomial_price() = 0;

	// Binomial Tree Methods
	void calculate_CRR_params();

public:
	// Constructors & Destructor
	Option(const double& vol, const double& strike_price, const double& interest_rate, const double& S, const float& expiry);
	Option() {};
	~Option() {};


	// Getter Methods
	double get_strike() { return strike; };
	double get_payoff(const double& S, const double& K) { return payoff(S, K); };
	double get_underlying_price() { return underlying_price; };
	double get_option_price() { return option_price; };
};

