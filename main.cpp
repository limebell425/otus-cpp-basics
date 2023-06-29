// Read files and prints top k word by frequency

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <vector>
#include <chrono>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <string>
#include <future>

const size_t TOPK = 10;

using Counter = std::map<std::string, std::size_t>;

std::string tolower(const std::string &str);

void count_words(std::string filename, Counter&);

void print_topk(std::ostream& stream, const Counter&, const size_t k);

void merge_result(Counter& counter);

Counter final_freq_dict;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: topk_words [FILES...]\n";
        return EXIT_FAILURE;
    }
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<Counter> freq_dict(argc-1);
    std::vector<std::thread> threads;
    {    
        for (int i = 1; i < argc; ++i) {
            std::string filename = argv[i];
            threads.emplace_back(count_words, filename, std::ref(freq_dict[i-1]));
        }
    }
    for (auto &i : threads)
    {
        i.join();
    }

    for (int i = 1; i < argc; ++i)
    {
        merge_result(freq_dict[i-1]);
    }

    print_topk(std::cout, final_freq_dict, TOPK);
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Elapsed time is " << elapsed_ms.count() << " us\n";
}

void merge_result(Counter& counter)
{
    for (const auto& [word, count] : counter)
    {
        final_freq_dict[word] += count;
    }
}

std::string tolower(const std::string &str) {
    std::string lower_str;
    std::transform(std::cbegin(str), std::cend(str),
                   std::back_inserter(lower_str),
                   [](unsigned char ch) { return std::tolower(ch); });
    return lower_str;
};

void count_words(std::string filename, Counter& counter) {
    std::ifstream input{filename};
    if (!input.is_open()) {
        std::cerr << "Failed to open file " << filename << '\n';
        return;
    }
    std::for_each(std::istream_iterator<std::string>(input),
                  std::istream_iterator<std::string>(),
                  [&counter](const std::string &s) {++counter[tolower(s)]; });   
}

void print_topk(std::ostream& stream, const Counter& counter, const size_t k) {
    std::vector<Counter::const_iterator> words;
    words.reserve(counter.size());
    for (auto it = std::cbegin(counter); it != std::cend(counter); ++it) {
        words.push_back(it);
    }

    std::partial_sort(
         std::begin(words), std::begin(words) + k, std::end(words),
         [](auto lhs, auto &rhs) { return lhs->second > rhs->second; });

    std::for_each(
        std::begin(words), std::begin(words) + k,
        [&stream](const Counter::const_iterator &pair) {
            stream << std::setw(4) << pair->second << " " << pair->first
                      << '\n';
        });
}