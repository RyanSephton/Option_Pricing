/**
* Project:    Project 2
* Filename:   Vanilla_Monte_Carlo_Pricer.h
* Version:    v1 (12 April 2020)
* Author:     Ryan Sephton
* Summary:    Child Class That Implements Pricing For Vanilla European Options.
*/


#pragma once
#include "Monte_Carlo_Pricer.h"
template <typename T>class Vanilla_Monte_Carlo_Pricer : public Monte_Carlo_Pricer<T>
{
protected:


	// Attributes
	unsigned int n_simulations{ 1000000 };
	/**
	* Function to determine the fair value of the option
	* by aggregating a large number of randomly generated underlying values.
	*/
	double determine_option_price()
	{
		double S = s_0*drift;
		double option_price{ 0 };
		double S_simulation{ 0 };
		double S_antithetic{ 0 };
		double price_1{ 0 };
		double price_2{ 0 };
		
		// Initialise random path generator 
		std::random_device random_dev;
		std::mt19937 random_generator(random_dev());
		std::normal_distribution <double> distribution(0, 1);

		// Simulate underlying values at maturity
		for (unsigned int i = 1; i < n_simulations; ++i)
		{
			S_simulation = S* exp(dev*distribution(random_generator));
			S_antithetic = S* exp(dev*-distribution(random_generator));
			price_1 = option->get_payoff(S_simulation, strike);
			price_2 = option->get_payoff(S_antithetic, strike);
			option_price += (price_1 + price_2) / 2.;
		}

		// Aggregate the payoffs to estimate the option value.
		option_price /= n_simulations;
		option_price = exp(-r *maturity)*option_price;
		return option_price;
	}

public:

	/**
	* Constructor to class Vanilla_Monte_Carlo_Pricer. Used to price european options.
	* @param *option_to_price pointer to template object, denotes the option we wish to price.
	* @param interest_rate const double reference, denotes the interest rate of the market, for the option.
	* @param option_s_0 const double reference, denotes the value of the unerlying at t=0.
	* @param option_volatility const double reference, denotes the volatility of the market.
	* @param option_strike const double reference, denotes the exercise price of the option.
	* @param option_expiry const double reference, denotes the expiry date for the option, where 1 year => expiry = 365.
	*/
	Vanilla_Monte_Carlo_Pricer(T *option_to_price, const double& interest_rate, const double& option_s_0, const double& option_volatility, const double& option_strike, const double& option_expiry)
		:Monte_Carlo_Pricer<T>(option_to_price, interest_rate, option_s_0, option_volatility, option_strike, option_expiry)
	{
		// Initialise Brownian Motion parameters
		drift = exp(maturity*(r - (0.5*vol*vol)));
		dev = sqrt(vol*vol*maturity);
	};


	// Destructor
	~Vanilla_Monte_Carlo_Pricer() {};

};

