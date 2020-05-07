#include "Maturity_Only_CRR_Tree.h"

/**
* Project:    Problem 1 Assignment 3
* Filename:   Maturity_Only_CRR_tree.cpp
* Version:    v1 (12 April 2020)
* Author:     Ryan Sephton
* Summary:    Solution to Project 2
*/


/**
* Constructor to class CCR_Tree. Used to build and back integrate a binomial tree to price an option
* See: https://en.wikipedia.org/wiki/Binomial_options_pricing_model for reference
* @param initial_prices const double vector reference, each element is the value of the payoff at maturity, these are back integrated in this class.
* @param u const double reference, denotes the probability of an upwards jump along the tree.
* @param d const double reference, denotes the probability of a downwards jump along the tree.
* @param p const double reference, denotes the probability associated with the binomial distribution.
* @param r const double reference, denotes the interest rate expressed as a decimal, for the option.
* @param n_steps const int reference, denotes the width of the binomial tree.
* @param t_step const double reference, denotes the size of the timestep for the option.
*/
Maturity_Only_CRR_Tree::Maturity_Only_CRR_Tree(std::vector<double> &initial_prices, const double& u, const double& d, const double& p, const double& r, const int& n_steps, const double& t_step)
	:CRR_Tree(initial_prices,u, d, p, r, n_steps, t_step)
{
}



/**
* Function to back integrate the binomial tree.
*/
void Maturity_Only_CRR_Tree::calculate_price()
{

	for (int tree_layer = (steps - 1); tree_layer >= 0; tree_layer--)
	{
		for (int tree_node = 0; tree_node <= tree_layer; tree_node++)
		{
			prices.at(tree_node) = value_at_previous_node(prices.at(tree_node), prices.at(tree_node + 1)); // aggregate nodes at t+1 to get node at t
		}

		prices.at(tree_layer + 1) = 0; //for each layer we back integrate, the lowest node becomes redundant, so prune it.
	}

	price = float(prices.at(0));
	return;
}
