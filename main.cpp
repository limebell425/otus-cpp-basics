#include <iostream>
#include <vector>
#include "statistics.h"
#include <algorithm>
#include <array>
#include <memory>
#include <exception>

int main()
{
	const size_t statistics_count = 6;
	std::array<std::unique_ptr<IStatistics>, statistics_count> statistics = {
	std::make_unique<Min>(),
	std::make_unique<Max>(),
	std::make_unique<Mean>(),
	std::make_unique<Std>(),
	std::make_unique<Percentile>(90),
	std::make_unique<Percentile>(95),
	};

	std::cout << "Enter sequence" << std::endl;
	double val = 0;
	while (std::cin >> val) {
		for (size_t i = 0; i < statistics_count; ++i) {
			statistics[i]->update(val);
		}
	}

	// Handle invalid input data
	if (!std::cin.eof() && !std::cin.good()) {
		std::cerr << "Invalid input data\n";
		return 1;
	}

	for (const auto &statistic : statistics) {
		try {
			double result = statistic->eval();
			std::cout << statistic->name() << " = " << result << std::endl;
		}
		catch(const std::exception &ex) {
			std::cout << "Error " << statistic->name() << ": " << ex.what() << std::endl;  
		}
	}

	return 0;
}