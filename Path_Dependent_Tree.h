/**
* Project:    Project 2
* Filename:   Path_Dependent_Tree.h
* Version:    v1 (12 April 2020)
* Author:     Ryan Sephton
* Summary:    Child Class that implements pricing for average rate, path dependent options.
*/




#pragma once
#include "CRR_tree.h"


template <typename T>
class Path_Dependent_Tree :
	public CRR_Tree
{
protected:

	// Attributes 
	T *option;
	int k{ 300 }; // N states at node (see PDF for details).
	double S{ 0 };


	/**
	* Function to create a 3-d array on the heap.
	* This function is very rough-and-ready and could be
	* improved on considerably. It is needed to store
	* the value of the option and the associated costs.
	*/
	double ***Create3DArray(int a, int b, int c)
	{
		double ***A = new double**[a];
		for (int i = 0; i < a; i++) 
		{
			A[i] = new double*[b];
			for (int j = 0; j < b; j++) 
			{
				A[i][j] = new double[c];
			}
		}
		return A;
	}

	/**
	* Computes the upper running average at a node of the tree.
	* This is defined as the path of fewest downward movements, to 
	* a given node.
	*/
	double A_Max(int j, int i) {
		double A = (1 - pow(p_up, j - i + 1)) / (1 - p_up);
		double B = pow(p_up, j - i)*p_down*((1 - pow(p_down, i)) / (1 - p_down));
		return (S * A + S * B) / (j + 1);
	}

	/**
	* Computes the lower running average at a node of the tree.
	* This is defined as the path of fewest upward movements, to
	* a given node.
	*/
	double A_Min(int j, int i) {
		double A = (1 - pow(p_down, i + 1)) / (1 - p_down);
		double B = pow(p_down, i)*p_up*((1 - pow(p_up, j - i)) / (1 - p_up));
		return (S * A + S * B) / (j + 1);
	}

	/**
	* Linearly interpolates between an upper and lower running average at a node
	* and specific state. This prevents combinatorial explosion.
	*/
	double interpolate_nodes(int j, int i, int m) {
		return ((k - m) / (double)k)*A_Min(j, i) + (m / (double)k)*A_Max(j, i);
	}


	/**
	* Computes the change to the running average if the underlying
	* made an upwards movement to the next node.
	*/
	double upper_average(double cur_node, int j, int i) {
		return ((j + 1)*cur_node + S*pow(p_up, j + 1 - i)*pow(p_down, i)) / (j + 2);
	}

	/**
	* Computes the change to the running average if the underlying
	* made a downwards movement to the next node.
	*/
	double lower_average(double cur_node, int j, int i) {
		return ((j + 1)*cur_node + S*pow(p_up, j - i)*pow(p_down, i + 1)) / (j + 2);
	}

	/**
	* Gets the upper and lower state value that bounds the current average.
	*/
	int get_l(double A, int j, int i) {
		double delta_max_min = (A_Max(j, i) - A_Min(j, i)) / k;
		return (int)floor((A - A_Min(j, i)) / delta_max_min);
	}

	/**
	* Gets the gradient of the upper interpolant
	*/
	double get_wu(double Au, double*** asianMTree, int j, int i, int l) {
		return (Au - asianMTree[j + 1][i][l + 1]) / (asianMTree[j + 1][i][l] - asianMTree[j + 1][i][l + 1]);
	}

	/**
	* Gets the gradient of the lower interpolant
	*/
	double get_wd(double Ad, double*** asianMTree, int j, int i, int l) {
		return (Ad - asianMTree[j + 1][i + 1][l + 1]) / (asianMTree[j + 1][i + 1][l] - asianMTree[j + 1][i + 1][l + 1]);
	}

	/**
	* Analagous to the node aggregation used in the CRR tree to back integrate the cost
	*/
	double interpolate_upper_cost(double*** CTree, double x, int j, int i, int l) {
		return x*CTree[j + 1][i][l] + (1 - x)*CTree[j + 1][i][l + 1];
	}

	/**
	* Analagous to the node aggregation used in the CRR tree to back integrate the cost
	*/
	double interpolate_lower_cost(double*** CTree, double x, int j, int i, int l) {
		return x*CTree[j + 1][i + 1][l] + (1 - x)*CTree[j + 1][i + 1][l + 1];
	}

	/**
	* Implementation of the hull-white algorithm for path dependent options.
	* See "Financial Engineering and Computation" by Yuh Dauh Lyuu for 
	* specifics.
	*/
	void calculate_price() {

		int node_num = steps + 1;

		double ***value_tree = Create3DArray(node_num, node_num, k + 1);
		double ***cost_tree = Create3DArray(node_num, node_num, k + 1);

		// Construct linear interpolant for each node
		for (int j = 0; j<node_num; j++) {
			for (int i = 0; i <= j; i++) {
				for (int m = 0; m <= k; m++) {
					value_tree[j][i][m] = interpolate_nodes(j, i, m);
				}
			}
		}

		// Initialise tree parameters
		double Au;
		double Ad;
		double w_u{ 0 };
		double w_d{ 0 }; 
		double c_u;
		double c_d;
		double price_at_node;
		int l_u;
		int l_d;

		// Begin Back Integration sub-routine
		for (int j = node_num - 1; j >= 0; j--) {
			for (int i = 0; i <= j; i++) {
				for (int m = 0; m <= k; m++) {

					// Get Upper and lower running averages
					Au = upper_average(value_tree[j][i][m], j, i);
					Ad = lower_average(value_tree[j][i][m], j, i);

					if (j == node_num - 1) {
						// Determine payoffs at t=T
						c_u = option->get_payoff(Au, option->get_strike());
						c_d = option->get_payoff(Ad, option->get_strike());
					}
					else {
						// Get bounding interval for upper running average
						if (Au > A_Max(j + 1, i) || (A_Max(j + 1, i) - Au)< 0.0001) {
							c_u = cost_tree[j + 1][i][k];
						}
						else if (Au < A_Min(j + 1, i)) {
							c_u = cost_tree[j + 1][i][0];
						}
						else {
							l_u = get_l(Au, j + 1, i);
							w_u = get_wu(Au, value_tree, j, i, l_u);
							c_u = interpolate_upper_cost(cost_tree, w_u, j, i, l_u);
						}
						// Get bounding interval for lower running average
						if (Ad > A_Max(j + 1, i + 1) || (A_Max(j + 1, i + 1) - Ad)<0.0001) {
							c_d = cost_tree[j + 1][i + 1][k];
						}
						else if (Ad < A_Min(j + 1, i + 1)) {
							c_d= cost_tree[j + 1][i + 1][0];
						}
						else {
							l_d = get_l(Ad, j + 1, i + 1);
							w_d = get_wd(Ad, value_tree, j, i, l_d);
							c_d = interpolate_lower_cost(cost_tree, w_d, j, i, l_d);
						}
					}
					// Back integrate the cost at the node
					price_at_node = (prob*c_u + (1 - prob)*c_d) / exp(interest_rate*delta_t);

					cost_tree[j][i][m] = price_at_node;
				}
			}
		}
		price = cost_tree[0][0][0];

		// Free up memory to prevent leak
		delete cost_tree;
		delete value_tree;
		return;
	}


public:



	/**
	* Constructor to class Path_Dependent_Tree. Used to price a path dependent option using the 
	* Hull-White algorithm. See book reference given above and PDF for specifics.
	* @param *option_to_price pointer to templated object, denotes the path-dependent option we wish to price.
	* @param u const double reference, denotes the probability of an upwards jump along the tree.
	* @param d const double reference, denotes the probability of a downwards jump along the tree.
	* @param p const double reference, denotes the probability associated with the binomial distribution.
	* @param r const double reference, denotes the interest rate expressed as a decimal, for the option.
	* @param n_steps const int reference, denotes the width of the binomial tree.
	* @param t_step const double reference, denotes the size of the timestep for the option.
	*/
	Path_Dependent_Tree(T *option_to_price, const double& u, const double& d, const double& p, const double& r, const int& n_steps)
		:CRR_Tree(u, d, p, r, n_steps, (1./365.))
	{
		option = option_to_price;
		S = option_to_price->get_underlying_price();
	};


	// Destructor
	~Path_Dependent_Tree(){};
};

