/**
* Project:    Problem 1 Assignment 3
* Filename:   Maturity_Only_CRR_tree.cpp
* Version:    v1 (12 April 2020)
* Author:     Ryan Sephton
* Summary:    Child class that implements binomial pricing methods for 
*			  non path-dependent options that are exercised at maturity only.
*/

#pragma once
#include "CRR_tree.h"
class Maturity_Only_CRR_Tree :
	public CRR_Tree
{
private:
	

	// Functions
	void calculate_price();

public:
	
	// Constructors & Destructors
	Maturity_Only_CRR_Tree(std::vector<double> &initial_prices, const double& u, const double& d, const double& p, const double& r, const int& n_steps, const double& t_step);
	~Maturity_Only_CRR_Tree() {};

};

