/**
* Project:    Project 2
* Filename:   CRR_tree.h
* Version:    v1 (12 April 2020)
* Author:     Ryan Sephton
* Summary:    Abstract Base Class That Stores Various Parameters
*             For Pricing Options Using a Binomial Method.
*/



#pragma once
#include <vector>

class CRR_Tree{

protected:
	// Attributes

	// Tree Params
	double price{ 0 };
	double p_up;
	double p_down;
	double prob;
	double delta_t;
	int steps;
	double interest_rate;
	std::vector<double> prices; 	// Value of the option for each node of the tree

	// Functions
	virtual void calculate_price()=0;
	double value_at_previous_node(double value_above, double value_below); // Used to back integrate the tree
	double max(const double& value1, const double& value2) { return (value1 > value2) ? value1 : value2; };

public:
	// Constructors & Destructor
	CRR_Tree(const std::vector<double>& payoffs_at_T, const double& u, const double& d, const double& p, const double& r, const int& n_steps, const double& t_step);
	CRR_Tree(const double& u, const double& d, const double& p, const double& r, const int& n_steps, const double& t_step);
	~CRR_Tree() {};
	
	// Functions
	double get_price();
};