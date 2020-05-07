/**
* Project:    Project 2
* Filename:   Path_Dependent_Monte_Carlo_Pricer.h
* Version:    v1 (12 April 2020)
* Author:     Ryan Sephton
* Summary:    Child Class That Implements Pricing For Path
*             dependent options.
*/


#pragma once
#include "Monte_Carlo_Pricer.h"


template <typename T>class Path_Dependent_Monte_Carlo_Pricer : public Monte_Carlo_Pricer<T>
{
protected:

	// Attributes
	unsigned int n_simulations{ 100000 };

	/* Function to generate a random path for a unit of underlying.
	   These paths are averaged to determine fair value of the option*/ 
	std::vector<double> calculate_path(const bool& antithetic)
	{
		std::vector<double> path(n_steps);
		path.at(0) = s_0;
		
		// Initialise standard normal distribution 
		std::random_device random_dev;
		std::mt19937 random_generator(random_dev());
		std::normal_distribution <double> distribution(0,1);

		for (unsigned int i = 1; i < n_steps; ++i)
		{
			double random_number = distribution(random_generator);

			// Generate random path (see pdf for details)
			if (antithetic)
			{
				double random_number = -distribution(random_generator);
			}

			path.at(i) = path.at(i-1)*drift* exp(dev*random_number);
		}

		return path;
	}


	/**
	* Function to determine the fair value of the option 
	* by aggregating a large number of randomly generated paths
	*/
	double determine_option_price()
	{
		double option_price{ 0 };

		for (unsigned int i = 0; i < n_simulations; i++)
		{
			// Generate Path
			std::vector<double> path_1 = calculate_path(false);
			std::vector<double> path_2 = calculate_path(true);

			double s_mean_1{ 0 };
			double s_mean_2{ 0 };

			// Compute arithmetic path mean
			for (auto n : path_1){ s_mean_1 += n; };
			for (auto n : path_2){ s_mean_2 += n; }

			s_mean_1 /= n_steps;
			s_mean_2 /= n_steps;

			double path_price_1 = option->get_payoff(s_mean_1, strike);
			double path_price_2 = option->get_payoff(s_mean_2, strike);
			option_price += (path_price_1 + path_price_2)/2.;
		}

		// Option price is the mean of all such payoffs
		option_price /= n_simulations;
		option_price = exp(-r *maturity)*option_price;
		return option_price;
	}





public:


	/**
	* Constructor to class Path_Dependent_Monte_Carlo_Pricer. Used to price path dependent options.
	* @param *option_to_price pointer to template object, denotes the option we wish to price.
	* @param interest_rate const double reference, denotes the interest rate of the market, for the option.
	* @param option_s_0 const double reference, denotes the value of the unerlying at t=0.
	* @param option_volatility const double reference, denotes the volatility of the market.
	* @param option_strike const double reference, denotes the exercise price of the option.
	* @param option_expiry const double reference, denotes the expiry date for the option, where 1 year => expiry = 365.
	*/
	Path_Dependent_Monte_Carlo_Pricer(T *option_to_price, const double& interest_rate, const double& option_s_0, const double& option_volatility, const double& option_strike, const double& option_expiry)
		:Monte_Carlo_Pricer<T>(option_to_price, interest_rate, option_s_0, option_volatility, option_strike, option_expiry) 
	{
		// Initialise Brownian Motion parameters
		drift = exp(dt*(r - (0.5*vol*vol)));
		dev = sqrt(vol*vol*dt);
	};


	// Destructor
	~Path_Dependent_Monte_Carlo_Pricer() {};

};

