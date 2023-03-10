#pragma once
#include <vector>
#include <unordered_map>
#include <iostream>
const double CONST_NUM = 2147483647;
struct Parser {
    Parser();
    Parser(int argc, char** argv);
    std::vector<std::pair<std::string, std::pair<double, double>>> GetFiltersArgs();
    char* input_path_;
    char* output_path_;

private:
    std::vector<std::pair<std::string, std::pair<double, double>>> filters_args_;
    const std::unordered_map<std::string, int64_t> filters_num_of_args_ = {{"-crop", 2},  {"-gs", 0},   {"-neg", 0},
                                                                           {"-sharp", 0}, {"-edge", 1}, {"-blur", 1}};
};
