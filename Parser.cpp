
#include "Parser.h"

Parser::Parser() {
}

Parser::Parser(int argc, char** argv) {
    if (argc == 1 || argc == 0) {
        std::cout << "Welcome to Image Editor." << std::endl;
        std::cout << "We've got several filters that might interest you:" << std::endl;
        std::cout << "      -crop width height --- crops an image" << std::endl;
        std::cout << "      -gs --- turns all colors in the image into gray" << std::endl;
        std::cout << "      -neg -- makes image negative" << std::endl;
        std::cout << "      -sharp -- sharpens an image" << std::endl;

        std::cout << "Command example: ./a input_file output_file filters" << '\n';
        exit(0);
    }
    if (argc == 2) {
        std::cerr << "Error: no output files found" << std::endl;
        exit(EXIT_FAILURE);
    }
    input_path_ = argv[1];
    output_path_ = argv[2];
    if (argc > 3) {
        if (argv[3][0] != '-') {
            std::cerr << "Error: invalid format of filter '" << argv[4] << "'." << std::endl;
            exit(EXIT_FAILURE);
        }
        int64_t i = 3;
        while (i < argc) {
            if (argv[i][0] == '-') {
                auto it = filters_num_of_args_.find(argv[i]);
                if (it != filters_num_of_args_.end()) {
                    if (it->second == 1) {
                        try {
                            if (i + 1 < argc && 0 <= std::stod(argv[i + 1]) && std::stod(argv[i + 1]) <= CONST_NUM) {
                                filters_args_.push_back({argv[i], {std::stod(argv[i + 1]), -1}});
                                ++i;
                            } else if (i + 1 >= argc) {
                                std::cerr << "Invalid argument of filter '" << argv[i] << "'." << std::endl;
                                exit(EXIT_FAILURE);
                            } else if (std::stod(argv[i + 1]) < 0) {
                                if (static_cast<std::string>(argv[i]) == "-blur") {
                                    filters_args_.push_back({argv[i], {std::stod(argv[i + 1]), -1}});
                                    ++i;
                                } else {
                                    std::cerr << "Invalid argument of filter '" << argv[i] << "'." << std::endl;
                                    exit(EXIT_FAILURE);
                                }
                            } else {
                                std::cerr << "Invalid argument of filter '" << argv[i] << "'." << std::endl;
                                exit(EXIT_FAILURE);
                            }
                        } catch (std::invalid_argument const& ex) {
                            std::cerr << "Invalid argument of filter '" << argv[i] << "'." << std::endl;
                            exit(EXIT_FAILURE);
                        }
                    } else if (it->second == 2) {
                        try {
                            if (i + 2 < argc && 0 <= std::stod(argv[i + 1]) && std::stod(argv[i + 1]) <= CONST_NUM &&
                                0 <= std::stod(argv[i + 2]) && std::stod(argv[i + 2]) <= CONST_NUM) {
                                filters_args_.push_back({argv[i], {std::stod(argv[i + 1]), std::stod(argv[i + 2])}});
                                i += 2;
                            } else {
                                std::cerr << "Invalid argument of filter '" << argv[i] << "'." << std::endl;
                                exit(EXIT_FAILURE);
                            }
                        } catch (std::invalid_argument const& ex) {
                            std::cerr << "Invalid argument of filter '" << argv[i] << "'." << std::endl;
                            exit(EXIT_FAILURE);
                        }
                    } else {
                        filters_args_.push_back({argv[i], {-1, -1}});
                    }
                } else {
                    std::cerr << "Error: no such filter named '" << argv[i] << "' found." << std::endl;
                    exit(EXIT_FAILURE);
                }
            } else {
                std::cerr << "Error: no such filter named '" << argv[i] << "' found." << std::endl;
                exit(EXIT_FAILURE);
            }
            ++i;
        }
    }
}
std::vector<std::pair<std::string, std::pair<double, double>>> Parser::GetFiltersArgs() {
    return filters_args_;
}
