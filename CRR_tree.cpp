/**
* Project:    Project 2
* Filename:   CRR_tree.cpp
* Version:    v1 (12 April 2020)
* Author:     Ryan Sephton
* Summary:    Abstract Base Class That Stores Various Parameters
*             For Pricing Options Using a Binomial Method.
*/



#include "CRR_tree.h"

/**
* Constructor to class CRR_Tree. 
* See: https://en.wikipedia.org/wiki/Binomial_options_pricing_model for reference
* @param payoffs_at_T const double vector reference, each element is the value of the payoff at maturity, these are back integrated in the child classes.
* @param u const double reference, denotes the probability of an upwards jump along the tree.
* @param d const double reference, denotes the probability of a downwards jump along the tree.
* @param p const double reference, denotes the probability associated with the binomial distribution.
* @param r const double reference, denotes the interest rate expressed as a decimal, for the option.
* @param n_steps const int reference, denotes the width of the binomial tree.
* @param t_step const double reference, denotes the size of the timestep for the option.
*/
CRR_Tree::CRR_Tree(const std::vector<double>& payoffs_at_T,const double& u, const double& d, const double& p, const double& r, const int& n_steps, const double& t_step)
{
	if (u < 0. || d < 0. || p < 0. || r < 0. || n_steps <= 0 || t_step <= 0.)
	{
		throw 2; //ERROR: Unphysical parameters entered
	}

	// Update Members
	prices = payoffs_at_T;
	p_up = u;
	p_down = d;
	prob = p;
	steps = n_steps;
	interest_rate = r;
	delta_t = t_step;
}


/**
* Alternative Constructor to class CRR_Tree. Used by Path Dependent Tree. No payoff prices are needed here
* See: https://en.wikipedia.org/wiki/Binomial_options_pricing_model for reference.
* @param u const double reference, denotes the probability of an upwards jump along the tree.
* @param d const double reference, denotes the probability of a downwards jump along the tree.
* @param p const double reference, denotes the probability associated with the binomial distribution.
* @param r const double reference, denotes the interest rate expressed as a decimal, for the option.
* @param n_steps const int reference, denotes the width of the binomial tree.
* @param t_step const double reference, denotes the size of the timestep for the option.
*/
CRR_Tree::CRR_Tree(const double& u, const double& d, const double& p, const double& r, const int& n_steps, const double& t_step)
{
	if (u < 0. || d < 0. || p < 0. || r < 0. || n_steps <= 0 || t_step <= 0.)
	{
		throw 2; //ERROR: Unphysical parameters entered
	}

	// Update Members
	p_up = u;
	p_down = d;
	prob = p;
	steps = n_steps;
	interest_rate = r;
	delta_t = t_step;
}

/**
* Function to aggregate the two nodes at time t+1 connected to a node at t.
* Aggregates the nodes to get value at time t
*/
double CRR_Tree::value_at_previous_node(double value_above, double value_below)
{
	// v_n^m = e^(-rt)(pv_{n+1}^{m+1} + (1-p)v_n^{m+1})
	double value = (prob*value_above + ((1 - prob)*value_below))*exp(-1 * interest_rate*delta_t);
	return(value);
}

/**
* Function to return binomial option price.
*/
double CRR_Tree::get_price()
{
	calculate_price();
	return price;
}