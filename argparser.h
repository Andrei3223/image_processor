#pragma once

#include <string>
#include <tuple>


class Arguments {
public:
    Arguments(int argc, char *argv[]);

    std::string file_name_;
    std::string input_file_;
    std::string output_file_;
    bool gr_ = false;
    bool neg_ = false;
    bool sharp_ = false;
    std::tuple<bool, int> edge_ = {false, 0};
    std::tuple<bool, int> blur_ = {false, 0};
    std::tuple<bool, int, int> crop_ = {false, 0, 0};
    std::tuple<bool, int> my_filter_ = {false, 0};
};

