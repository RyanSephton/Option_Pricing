// mainP2.cpp : Defines the entry point for the console application.
//

#include "European_Call.h"
#include "European_Put.h"
#include "American_Call.h"
#include "American_Put.h"
#include "Asian_Call.h"
#include "Asian_Put.h"
#include <iostream>


int main()
{
	try {
		//Test Program for S = 100, T = 1 year
		American_Put my_am_put(0.4, 100, 0.04, 100, 365);
		std::cout << "American Put Binomial Price: \t" << my_am_put.get_binomial_price() << std::endl;
		std::cout << "American Put PDE Price:\t " << my_am_put.get_pde_price() << "\n\n" << std::endl;

		American_Call my_am_call(0.4, 100, 0.04, 100, 365);
		std::cout << "American Call Binomial Price: \t" << my_am_call.get_binomial_price() << std::endl;
		std::cout << "American Call PDE Price: \t" << my_am_call.get_pde_price() << "\n\n" << std::endl;

		European_Put my_euro_put(0.4, 100, 0.04, 100, 365);
		std::cout << "European Put Binomial Price: \t" << my_euro_put.get_binomial_price() << std::endl;
		std::cout << "European Put Analytic Price: \t" << my_euro_put.get_analytic_price() << std::endl;
		std::cout << "European Put MC Price:  \t\t" << my_euro_put.get_monte_carlo_price() << std::endl;
		std::cout << "European Put PDE Price: \t" << my_euro_put.get_pde_price() << "\n\n" << std::endl;

		European_Call my_euro_call(0.4, 100, 0.04, 100, 365);
		std::cout << "European Call Analytic Price: \t" << my_euro_call.get_analytic_price() << std::endl;
		std::cout << "European Call Binomial Price: \t" << my_euro_call.get_binomial_price() << std::endl;
		std::cout << "European Call MC Price: \t" << my_euro_call.get_monte_carlo_price() << std::endl;
		std::cout << "European Call PDE Price: \t" << my_euro_call.get_pde_price() << "\n\n" << std::endl;


		Asian_Call my_asian_call(0.4, 100, 0.04, 100, 365);
		std::cout << "Asian Call MC Price: \t\t" << my_asian_call.get_monte_carlo_price() << std::endl; // SLOW
		std::cout << "Asian Call Analytic Price: \t" << my_asian_call.get_analytic_price() << std::endl;
		std::cout << "Asian Call Binomial Price: \t" << my_asian_call.get_binomial_price() << "\n\n" << std::endl; // SLOW


		Asian_Put my_asian_put(0.4, 100, 0.04, 100, 365);
		std::cout << "Asian Put MC Price: \t" << my_asian_put.get_monte_carlo_price() << std::endl; // SLOW
		std::cout << "Asian Put Analytic Price: \t" << my_asian_put.get_analytic_price() << std::endl;
		std::cout << "Asian Put Binomial Price: \t" << my_asian_put.get_binomial_price() << "\n\n" << std::endl; // SLOW

	}catch (int error)
	{
		std::cout << "Unphysical Parameter Entered! ERROR CODE:" << error << std::endl;
	}
    return 0;
}

