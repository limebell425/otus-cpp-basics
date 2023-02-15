#pragma once

std::string get_user_name();
bool update_high_scores(const std::string &high_scores_filename, std::string user_name, int attempts_count);
bool print_high_scores(const std::string &high_scores_filename);
bool filtr_high_scores(const std::string &high_scores_filename);