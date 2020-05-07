/**
* Project:    Project 2
* Filename:   Monte_Carlo_Pricer.h
* Version:    v1 (12 April 2020)
* Author:     Ryan Sephton
* Summary:    Abstract Base Class That All Monte Carlo Pricers are Derived From.
*/




#include <random>
#include <vector>
#pragma once
template <typename T>
class Monte_Carlo_Pricer
{
protected:

	// Attributes
	T *option;
	double s_0;
	double vol;
	double strike;
	double maturity;
	unsigned int n_steps;
	double r;
	double dt{0};
	double drift{0};
	double dev{ 0};

	// Methods
	virtual double determine_option_price() = 0; //implemented in both child classes
public:



	/**
	* Constructor to class Monte_Carlo_Pricer. Used to store parameters associated with the monte carlo pricing
	* of any option. Parent class to both "Path_Dependent_Monte_Carlo_Pricer" and "Vanilla_Monte_Carlo_Pricer".
	* @param *option_to_price pointer to template object, denotes the option we wish to price.
	* @param interest_rate const double reference, denotes the interest rate of the market, for the option.
	* @param option_s_0 const double reference, denotes the value of the unerlying at t=0.
	* @param option_volatility const double reference, denotes the volatility of the market.
	* @param option_strike const double reference, denotes the exercise price of the option.
	* @param option_expiry const double reference, denotes the expiry date for the option, where 1 year => expiry = 365.
	*/
	Monte_Carlo_Pricer(T *option_to_price, const double& interest_rate, const double& option_s_0, const double& option_volatility, const double& option_strike, const double& option_expiry)
	{
		option = option_to_price;
		r = interest_rate;
		s_0 = option_s_0;
		vol = option_volatility;
		strike = option_strike;
		maturity = option_expiry;
		n_steps = int(maturity*365.);
		dt = maturity / n_steps;
	};
	virtual double get_mc_price() { return determine_option_price(); }; // Implemented in both child classes

	// Destructor
	~Monte_Carlo_Pricer() {};
};

