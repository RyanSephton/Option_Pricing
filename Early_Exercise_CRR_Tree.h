/**
* Project:    Project 2
* Filename:   Early_Exercise_CRR_tree.h
* Version:    v1 (12 April 2020)
* Author:     Ryan Sephton
* Summary:    Child Class that implements pricing for American Options.
*/




#include "CRR_tree.h"


template <typename T>
class Early_Exercise_CRR_Tree
	:public CRR_Tree
{
protected:

	// Methods


	/**
	* Function to compute the value of the underlying at a given node in the tree.
	*/
	double underlying_at_node(const unsigned int& layer, const unsigned int& node)
	{
		double s = option->get_underlying_price()*pow(p_up, int(layer - node))*pow(p_down, node);
		
		return s;
	}


	/**
	* Function to determine the fair price of the American option.
	*/
	void calculate_price()
	{
		double aggregated_value{ 0 };
		double S{ 0 };

		for (int tree_layer = (steps - 1); tree_layer >= 0; tree_layer--)
		{
			for (int tree_node = 0; tree_node <= tree_layer; tree_node++)
			{
				// Back integrate the tree and determine if option should be exercised or not
				aggregated_value = value_at_previous_node(prices.at(tree_node), prices.at(tree_node + 1)); 
				S = underlying_at_node(tree_layer, tree_node);
				prices.at(tree_node) = max(aggregated_value, option->get_payoff(S, option->get_strike()));
			}

			prices.at(tree_layer + 1) = 0; //for each layer we back integrate, the lowest node becomes redundant, so prune it.
		}

		price = float(prices.at(0));
		return;
	}


	// Attributes
	T *option;


public:

	/**
	* Constructor to  templated class Early_Exercise_CRR_Tree.
	* @param *option_to_price pointer to template object, this is the container for the instance of the option we wish to price.
	* @param u const double reference, denotes the probability of an upwards jump along the tree.
	* @param d const double reference, denotes the probability of a downwards jump along the tree.
	* @param p const double reference, denotes the probability associated with the binomial distribution.
	* @param r const double reference, denotes the interest rate expressed as a decimal, for the option.
	* @param n_steps const int reference, denotes the width of the binomial tree.
	* @param t_step const double reference, denotes the size of the timestep for the option.
	*/
	Early_Exercise_CRR_Tree( T *option_to_price, const std::vector<double> &initial_prices, const double& u, const double& d, const double& p, const double& r, const int& n_steps, const double& t_step)
		:CRR_Tree(initial_prices, u, d, p, r, n_steps, t_step)
	{
		option = option_to_price;
	}

	// Destructor
	~Early_Exercise_CRR_Tree() {};





};