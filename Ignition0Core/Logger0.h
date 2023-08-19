/**
 * Author:	Varun Khambhata
 * Created: 12.08.2023
**/

#ifndef __LOGGER0__
#define __LOGGER0__


#ifndef RELEASE_BUILD
	#define RESET   "\033[0m"
	#define RED     "\033[31m"
	#define GREEN   "\033[32m"
	#define YELLOW  "\033[33m"
	#define BLUE    "\033[34m"

	#include <iostream>

	#define Logger0(String) \
			std::cout << GREEN << "Ω " << String <<std::endl << RESET

	#define Logger0E(String) \
			std::cout << RED << "Θ " << String <<std::endl << RESET

#else
	#define Logger0(String)
	#define Logger0E(String)

#endif



#endif