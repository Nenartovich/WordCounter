#include <iostream>
#include <boost/process.hpp>

namespace bp = boost::process;

bool cmp(std::pair<std::string, int> first_pair,
         std::pair<std::string, int> second_pair) {
    return ((first_pair.first < second_pair.first) ||
            (first_pair.first == second_pair.first && first_pair.second < second_pair.second));
}

void Reduce(std::string script_path, std::string file_name) {
    std::string new_file_name = file_name;
    new_file_name[0] = 'b';
    bp::system(script_path, bp::std_in < file_name, bp::std_out > new_file_name);
}

void CreateFile(const std::vector<std::pair<std::string, int>>& data, std::string file_name) {
    std::ofstream output_stream(file_name);
    for (int i = 0; i < data.size(); i++) {
        output_stream << data[i].first << '\t' << data[i].second << std::endl;
    }
}

void MergeFiles(int files_count, std::string destination_file) {
    std::ofstream output_stream(destination_file);
    std::string key; int value;
    for (int i = 1; i < files_count; i++) {
        std::ifstream input_stream2("b" + std::to_string(i) + ".txt");
        input_stream2 >> key >> value;
        output_stream << key << '\t' <<  value << '\n';
    }
}

int FileSpliting(std::string script_path, std::string source_file, std::string destination_file) {
    std::vector<std::pair<std::string, int>> pairs;
    std::string word;
    int num;
    std::ifstream input_stream(source_file);
    while (input_stream >> word) {
        input_stream >> num;
        pairs.push_back({word, num});
    }

    std::sort(pairs.begin(), pairs.end(), cmp);

    int cnt = 1;
    std::vector<std::pair<std::string, int>> v;

    v.push_back(pairs[0]);
    for (int i = 1; i < pairs.size(); i++) {
        if (v.back().first == pairs[i].first) {
            v.push_back(pairs[i]);
        } else {
            std::string file_name = "a" + std::to_string(cnt) + ".txt";
            cnt++;
            CreateFile(v, file_name);
            v.clear();
            v.push_back(pairs[i]);
        }
    }

    if (!v.empty()) {
        std::string file_name = "a" + std::to_string(cnt) + ".txt";
        CreateFile(v, file_name);
        cnt++;
    }

    return cnt;
}

void Map(std::string script_path, std::string source_file, std::string destination_file) {
    bp::system(script_path, bp::std_in < source_file, bp::std_out > destination_file);
}

int main(int n, char* args[]) {
    if (n != 5) {
        std::cerr << "Wrong number of arguments" << std::endl;
        return 0;
    }

    if ((std::string)args[1] == "map") {
        Map((std::string) args[2], (std::string) args[3], (std::string) args[4]);
    } else if ((std::string)args[1] == "reduce") {
        int file_count = FileSpliting((std::string) args[2], (std::string) args[3],
                (std::string) args[4]);

        for (int i = 1; i < file_count; i++) {
            Reduce(args[2], "a" + std::to_string(i) + ".txt");
        }

        MergeFiles(file_count, args[4]);

        for (int i = 1; i < file_count; i++) {
            std::string tmp = std::to_string(i) + ".txt";
            bp::system("unlink a" + tmp);
            bp::system("unlink b" + tmp);
        }

    } else {
        std::cerr << "Wrong operation" << std::endl;
        return 0;
    }

    return 0;
}