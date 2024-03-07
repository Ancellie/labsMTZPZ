#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>

using namespace std;

string convertMarkdownToHTML(const  string &markdown) {
    return markdown; //TODO Реалізувати цю функцію
}

void saveHTMLToFile(const string &html, const string &outputPath) {
    ofstream outputFile(outputPath);
    if (outputFile.is_open()) {
        outputFile << html;
        outputFile.close();
    } else {
        cerr << "Error: Unable to open output file" << endl;
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " /path/to/input.md [--out /path/to/output.html]" << endl;
        return EXIT_FAILURE;
    }

    ifstream inputFile(argv[1]);
    if (!inputFile.is_open()) {
        cerr << "Error: Unable to open input file" << endl;
        return EXIT_FAILURE;
    }

    stringstream buffer;
    buffer << inputFile.rdbuf();
    string markdownContent = buffer.str();

    inputFile.close();

    string htmlContent = convertMarkdownToHTML(markdownContent);

    string outputPath = "";
    for (int i = 2; i < argc; ++i) {
        if (string(argv[i]) == "--out" && i + 1 < argc) {
            outputPath = argv[i + 1];
        }
    }

    if (!outputPath.empty()) {
        saveHTMLToFile(htmlContent, outputPath);
        cout << "HTML saved to: " << outputPath << endl;
    } else {
        cout << htmlContent << endl;
    }

    return EXIT_SUCCESS;
}
