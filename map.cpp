#include <bits/stdc++.h>

int main() {
    std::string key;
    std::string line;
    while (std::getline(std::cin, key, '\t')) {
        std::getline(std::cin, line);
        std::stringstream words;
        words << line;
        std::string word;
        while (words >> word) {
            std::cout << word << "\t1" << std::endl;
        }
    }
    return 0;
}