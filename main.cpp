#include <iostream>
#include <vector>
#include "statistics.h"
#include <algorithm>
#include <array>

int main()
{
	std::vector<double> sequence;
	sequence.reserve(100);
	std::cout << "Enter sequence" << std::endl;
	double val;
	while (std::cin >> val) {
		sequence.push_back(val);
	}

	// Handle invalid input data
	if (!std::cin.eof() && !std::cin.good()) {
		std::cerr << "Invalid input data\n";
		return 1;
	}

	if (sequence.size() == 0) {
		std::cout << "Empty sequence" << std::endl;
		return 0;
	}

	std::sort(
		sequence.begin(), 
		sequence.end(), 
		[](const auto &lhs, const auto &rhs) {
			return lhs < rhs; 
		});
	const size_t statistics_count = 6;
	std::array<std::unique_ptr<IStatistics>, statistics_count> statistics = {
		std::make_unique<Min>(),
		std::make_unique<Max>(),
		std::make_unique<Mean>(),
		std::make_unique<Std>(sequence),
		std::make_unique<Percentile>(90, sequence),
		std::make_unique<Percentile>(95, sequence),
	};

	for (double val : sequence) {
		for (const auto &statistic : statistics) {
			statistic->update(val);
		}
	}

	for (const auto &statistic : statistics) {
		std::cout << statistic->name() << " = " << statistic->eval() << std::endl;
	}

	return 0;
}