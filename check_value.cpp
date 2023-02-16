#include <iostream>

#include <cstdlib>
#include <ctime>
#include <limits>

#include "check_value.h"


int check_value(int target_value) {

	int current_value = 0;
	int attempts_count = 0;

	std::cout << "Enter your guess:" << std::endl;
	do {
		std::cin >> current_value;
		if (std::cin.fail()){
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Incorrect input" << std::endl; 
			continue;
		}
		++attempts_count;

		if (current_value > target_value) {
			std::cout << "less than " << current_value << std::endl;
		}
		else if (current_value < target_value) {
			std::cout << "greater than " << current_value << std::endl;
		}
		else {
			std::cout << "you win! attempts = " << attempts_count << std::endl;
			break;
		}

	} while(true);

	return attempts_count;

}