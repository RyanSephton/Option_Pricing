/**
* Project:    Project 2
* Filename:   Vanilla_Option.h
* Version:    v1 (12 April 2020)
* Author:     Ryan Sephton
* Summary:    Abstract Base Class That All Vanilla Options 
*             (American & European) are Derived From.
*/


#pragma once
#include "Option.h"
class Vanilla_Option :
	public Option
{ 
protected:

	// Attributes ------------------------------------------------------------
	// PDE solver 
	// See PDF or "Wilmott - Mathematics for financial derivatives" (Part II)
	// For further details.
	double k = 2.*r / pow(volatility, 2);
	int N_plus{ 1000};
	int N_minus{ -1000 };  
	double alpha{ 0.25 };  
	double dx{0.05};
	double dt{ alpha*pow(dx,2) };
	int n_t_steps{ int((pow(volatility,2)*(maturity)) / (2.*dt)) };
	double tolerance{ 0.0000001 };
	std::vector<double> u_at_t; 


	// Binomial solver attributes
	std::vector<double> payoffs; 

	//-------------------------------------------------------------------------
	

	// Methods ----------------------------------------------------------------


	// PDE
	virtual double transformed_payoff(const double& x, const double& tau = 0)=0;
	virtual double implicit_fdm() = 0;
	virtual double get_pde_price() = 0; //Implemented in American_Vanilla_Option & European_Vanilla_Option
	double get_grid_offset();
	
	// Binomial
	void payoffs_at_T();

	//--------------------------------------------------------------------------



public:

	// Constructors & Destructor
	Vanilla_Option() 
		:Option() {};
	Vanilla_Option(const double& vol, const double& strike_price, const double& interest_rate, const double& S, const float& expiry);
	~Vanilla_Option() {};


};

