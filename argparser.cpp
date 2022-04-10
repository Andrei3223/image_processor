#include "argparser.h"
#include "exceptions.h"


Arguments::Arguments(int argc, char *argv[]) {
    file_name_ = argv[0];
    input_file_ = argv[1];
    output_file_ = argv[2];
    for (int i = 3; i < argc; ++i) {
        std::string temp = argv[i];
        if (temp == "-gr") {
            gr_ = true;
        } else if (temp == "-neg") {
            neg_ = true;
        } else if (temp == "-sharp") {
            sharp_ = true;
        } else if (temp == "-edge") {
            int num = 0;
            try {
                num = static_cast<int>(255. * std::stod(argv[i + 1]));
            } catch (const std::exception &e) {
                throw ArgumentsError();
            }
            edge_ = {true, num};
            ++i;
        } else if (temp == "-blur") {
            double num = 0;
            try {
                num = std::stod(argv[i + 1]);
            } catch (const std::exception &e) {
                throw ArgumentsError();
            }
            blur_ = {true, num};
            ++i;
        } else if (temp == "-my_filter") {
            int num = 0;
            try {
                num = std::stoi(argv[i + 1]);
            } catch (const std::exception &e) {
                throw ArgumentsError();
            }
            ++i;
            my_filter_ = {true, num};
        } else if (temp == "-crop") {
            int num1 = 0;
            int num2 = 0;
            try {
                num1 = std::stoi(argv[i + 1]);
            } catch (const std::exception &e) {
                throw ArgumentsError();
            }
            ++i;
            try {
                num2 = std::stoi(argv[i + 1]);
            } catch (const std::exception &e) {
                throw ArgumentsError();
            }
            crop_ = std::tuple(true, num1, num2);
            ++i;
        } else {
            throw ArgumentsError();
        }
    }
}
