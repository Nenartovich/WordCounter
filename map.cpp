#include <bits/stdc++.h>

int main() {
    std::string line;
    while (getline(std::cin, line)) {
        std::string tmp = "";
        int cnt = 0;
        for (int i = 0; i < line.size(); i++) {
            if (line[i] != '\t') {
                tmp += line[i];
            } else {
                if (tmp != "") {
                    if (cnt == 0 && (tmp == "Key" || tmp == "key")) {
                        tmp = "";
                        continue;
                    } else {
                        cnt++;
                        std::cout << tmp << "\t1" << std::endl;
                    }
                    tmp = "";
                }
            }
        }

        if (tmp != "") {
            std::cout << tmp << "\t1" << std::endl;
        }
    }
    return 0;
}
