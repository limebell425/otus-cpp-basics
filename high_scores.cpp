#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include <iomanip>

#include "high_scores.h"

std::string get_user_name(){
	std::string user_name;
    std::cout << "Hi! Enter your name, please:" << std::endl;
	std::cin >> user_name;
	return user_name;
}

std::unordered_map<std::string, int> get_scores_table(const std::string &high_scores_filename){
	std::ifstream in_file{high_scores_filename, std::ios_base::in};
	if (!in_file.is_open()) {
		std::cout << "Warning: failed to open file for read: " << high_scores_filename << "!" << std::endl;
		return {};
	}
	std::unordered_map<std::string, int> high_scores_table;
	std::string user_name;
	int high_score = 0;
	while (true) {
		in_file >> user_name >> high_score;
		if (in_file.fail()) {
			break;
		}
		if ((high_score < high_scores_table[user_name]) || (high_scores_table[user_name] == 0)){
			high_scores_table[user_name] = high_score;
		}
		in_file.ignore();
	}
	return high_scores_table;
}

std::vector<std::pair<std::string, int>> sort_high_scores(std::unordered_map<std::string, int> high_scores_table){
	std::vector<std::pair<std::string, int>> sorted_table;
	sorted_table.reserve(high_scores_table.size());

	std::copy(high_scores_table.begin(), high_scores_table.end(), std::back_inserter(sorted_table));
	std::sort(sorted_table.begin(), sorted_table.end(), [](const auto &lhs, const auto &rhs){return lhs.second < rhs.second;});

	return sorted_table;
}

bool save_high_scores(const std::string &high_scores_filename, std::vector<std::pair<std::string, int>> sorted_table){
	std::ofstream out_file{high_scores_filename, std::ios_base::out};
	if (!out_file.is_open()) {
		std::cout << "Failed to open file for write: " << high_scores_filename << "!" << std::endl;
		return false;
	}
	for (const auto& [name, score] : sorted_table){
		out_file << std::left << std::setw(10) << name << ' ' << score << std::endl;
	}
	return true;
}

bool update_high_scores(const std::string &high_scores_filename, std::string user_name, int attempts_count){
	std::unordered_map<std::string, int> high_scores_table = get_scores_table(high_scores_filename);
	if ((attempts_count < high_scores_table[user_name]) || (high_scores_table[user_name] == 0)){
		high_scores_table[user_name] = attempts_count;
	}
	auto sorted_table = sort_high_scores(std::move(high_scores_table));
	return (save_high_scores(high_scores_filename, std::move(sorted_table)));
}



bool print_high_scores(const std::string &high_scores_filename){
	std::ifstream in_file{high_scores_filename};
	if (!in_file.is_open()) {
		std::cout << "Failed to open file for read: " << high_scores_filename << "!" << std::endl;
		return false;
	}
	std::cout << "High scores table:" << std::endl;
	std::string user_name;
	int high_score = 0;
	while (true) {
		in_file >> user_name >> high_score;
		in_file.ignore();
		if (in_file.fail()) {
			break;
		}
		std::cout << std::left << std::setw(10) << user_name << ' ' << high_score << std::endl;
	}
	return true;
}

//функция для третьего дополнительного задания
bool filtr_high_scores(const std::string &high_scores_filename){
	std::unordered_map<std::string, int> high_scores_table = get_scores_table(high_scores_filename);
	auto sorted_table = sort_high_scores(std::move(high_scores_table));
	std::cout << "High scores table:" << std::endl;
		
	for (const auto& [name, score] : sorted_table){
		std::cout << std::left << std::setw(10) << name << ' ' << score << std::endl;
	}
	return true;
}
