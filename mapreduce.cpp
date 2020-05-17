#include <iostream>
#include <boost/process.hpp>

namespace bp = boost::process;
const std::string DIRNAME = "temporary_files_directory/";

void Reduce(const std::string& script_path, const std::string& file_name) {
    std::string new_file_name = file_name;
    new_file_name[0] = 'b';
    bp::system(script_path, bp::std_in < (DIRNAME + file_name), bp::std_out > (DIRNAME + new_file_name));
}

void CreateFile(const std::pair<std::string, std::vector<int>>& data, const std::string& file_name) {
    std::ofstream output_stream(DIRNAME + file_name);
    for (const int& value : data.second) {
        output_stream << data.first << '\t' << value << std::endl;
    }
}

void MergeFiles(int files_count, const std::string& destination_file) {
    std::ofstream output_stream(destination_file);
    std::string key; int value;
    for (int i = 1; i < files_count; i++) {
        std::ifstream input_stream2(DIRNAME + "b" + std::to_string(i) + ".txt");
        input_stream2 >> key >> value;
        output_stream << key << '\t' <<  value << '\n';
    }
}

int FileSpliting(const std::string& script_path, const std::string& source_file, const std::string& destination_file) {
    std::map<std::string, std::vector<int>> pairs;
    std::string word;
    int num;
    std::ifstream input_stream(source_file);
    while (input_stream >> word) {
        input_stream >> num;
        pairs[word].push_back(num);
    }

    int cnt = 1;
    for (auto& x: pairs) {
        std::string file_name = "a" + std::to_string(cnt) + ".txt";
        cnt++;
        CreateFile(x, file_name);
    }

    return cnt;
}

void Map(const std::string& script_path, const std::string& source_file, const std::string& destination_file) {
    bp::system(script_path, bp::std_in < source_file, bp::std_out > destination_file);
}

int main(int n, char* args[]) {
    if (n != 5) {
        std::cerr << "Wrong number of arguments" << std::endl;
        return 0;
    }

    if (std::string(args[1]) == "map") {
        Map(std::string(args[2]) , std::string(args[3]), std::string(args[4]));
    } else if (std::string(args[1]) == "reduce") {
        bp::system("mkdir " + DIRNAME);

        int file_count = FileSpliting(std::string(args[2]), std::string(args[3]),
                std::string(args[4]));

        for (int i = 1; i < file_count; i++) {
            Reduce(args[2], "a" + std::to_string(i) + ".txt");
        }

        MergeFiles(file_count, args[4]);

        bp::system("rm -rf " + DIRNAME);

    } else {
        std::cerr << "Wrong operation" << std::endl;
        return 0;
    }

    return 0;
}