#include <iostream>

int main() {
    std::string key;
    int value;
    int result = 0;
    while (std::cin >> key) {
        std::cin >> value;
        result += value;
    }

    std::cout << key << '\t' << result << '\n';
    return 0;
}
