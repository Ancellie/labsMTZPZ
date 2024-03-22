#include <fstream>
#include <iostream>
#include <vector>
#include "converter.hpp"

void checkArguments(int argc, char* argv[]) {
    if (argc != 2 && argc != 4) {
        throw std::runtime_error("Wrong format of input arguments");
    }

    if (argc == 4 && std::string(argv[2]) != "--out") {
        throw std::runtime_error("Wrong format of input arguments");
    }
}

int main(int argc, char* argv[]) {
    try {
        checkArguments(argc, argv);

        std::string inputFile = argv[1];
        std::string outputFile;
        if (argc == 4) {
            outputFile = argv[3];
        }

        std::ifstream markdownFile(inputFile);
        std::string markdownText((std::istreambuf_iterator<char>(markdownFile)), std::istreambuf_iterator<char>());

        std::string htmlText = Converter::markdownToHTML(markdownText);

        if (outputFile.empty()) {
            std::cout << htmlText << std::endl;
        } else {
            std::ofstream outFile(outputFile);
            outFile << htmlText;
            std::cerr << "HTML the markup has been saved in a file: " << outputFile << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error in file: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
