#include <iostream>
#include <string>
#include <algorithm>
#include <ctime>
#include "random_value.h"
#include "check_value.h"
#include "high_scores.h"

inline bool isInteger(const std::string & s)
{
    if (s.empty() || (s.size() > 9)){
         return false; 
    }
    return std::all_of(s.begin(), s.end(), [](char c){return std::isdigit(c);});
    
}

int main(int argc, char** argv){
    std::srand(static_cast<int>(std::time(nullptr)));
    const std::string high_scores_filename = "high_scores.txt"; 
    int max = 100;
    bool is_max = false;
    bool is_table = false;
    bool is_level = false;
    bool parameter_ok = true;
    for (int i = 0; i<argc; ++i){
        if ((std::string_view("-table") == argv[i])){
           is_table = true;
           break;
        }

        if ((std::string_view("-max") == argv[i]) && ((i + 1) < argc)){
            if (!isInteger(argv[i + 1])){
                std::cout << "Parameter max error" << std::endl;
                parameter_ok = false;
                break;
            }
            max = std::stoi(argv[i + 1]);
            is_max = true;
            if (is_level){
                std::cout << "Parameter list error. Last option (-max) selected" << std::endl;
            }
         }

        if ((std::string_view("-level") == argv[i]) && ((i + 1) < argc)){
            if (!isInteger(argv[i + 1])){
                std::cout << "Parameter level error" << std::endl;
                parameter_ok = false;
                break;
            }
            is_level = true;
            switch (std::stoi(argv[i + 1])){
                case 1: 
                    max = 10;
                    break;
                case 2: 
                    max = 50;
                    break;
                case 3:
                    max = 100;
                    break;
                default:
                    max = 10;
                    std::cout << "Level error. Minimum level set." << std::endl;
                    break;
            }
            if (is_max){
                std::cout << "Parameter list error. Last option (-level) selected" << std::endl;
            }
        }
    }

    if (!parameter_ok){
        return -2;
    }

    if (is_table){
        bool file_opened = print_high_scores(high_scores_filename);
        if (!file_opened){
            return -1;
        }
        return 0;
    }
    std::string user_name = get_user_name();
    int target_value = get_random_value(max);
    int attempts_count = check_value(target_value);
    bool file_opened = update_high_scores(high_scores_filename, std::move(user_name), attempts_count);
    if (!file_opened){
        return -1;
    }
    file_opened = print_high_scores(high_scores_filename);
    if (!file_opened){
        return -1;
    }
    return 0;
}