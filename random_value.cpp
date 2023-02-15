#include <iostream>
#include "random_value.h"

int get_random_value(int max){
	return (std::rand() % max);
}
