#include <iostream>
#include <fstream>
#include <regex>

using namespace std;

bool hasBothNeighbors(const string &input, size_t pos) {
    if (pos >= input.size()) {
        return false;
    }

    char rightChar = (pos < input.size() - 1) ? input[pos + 1] : '\0';  // '\0' - if there is no character to the right
    char leftChar = (pos > 0) ? input[pos - 1] : '\0';  // '\0' - if there is no character to the left

    if(rightChar == '\0' && leftChar == ' ' || leftChar == '\0' && rightChar == ' ')
        return true;

    return isalpha(leftChar) && isalpha(rightChar);
}

bool hasNeighboringCharacters(const string &input, size_t pos, char targetChar, int count) {
    if (pos >= input.size()) {
        return false;
    }

    char rightChar = (pos < input.size() - 1) ? input[pos + 1] : '\0';
    char leftChar = (pos > 0) ? input[pos - 1] : '\0';

    switch (count) {
        case 1:
            return !(targetChar == rightChar && (input[pos + 2] != ' ' || leftChar != ' '));
        case 2:
        case 3:
            return isalpha(leftChar) || isalpha(rightChar);
        case 4:
            return leftChar == '\n' && targetChar == rightChar && input[pos + 2] == targetChar &&
                   input[pos + 3] == '\n';
        default:
            return true;
    }
}

bool hasNestedMarkup(const string &input) {
    int boldCount = 0;
    int italicCount = 0;
    int monospacedCount = 0;
    int preformattedCount = 0;

    for (int i = 0; i < input.length(); ++i) {
        if (input.substr(i, 3) == "```" && hasNeighboringCharacters(input, i, '`', 4)) {
            preformattedCount += 1;
            i += 2;
            continue;
        }
        if (preformattedCount % 2 != 0) {
            continue;
        }
        if (input.substr(i, 2) == "**" && hasNeighboringCharacters(input, i, '*', 1)) {
            boldCount += 1;
            i += 1;
        } else if (input.substr(i, 1) == "_" && hasNeighboringCharacters(input, i, '_', 2) &&
                   !hasBothNeighbors(input, i)) {
            italicCount += 1;
        } else if (input.substr(i, 1) == "`" && hasNeighboringCharacters(input, i, '`', 3)) {
            monospacedCount += 1;
        }
    }

    return boldCount % 2 != 0 || italicCount % 2 != 0 || monospacedCount % 2 != 0 || preformattedCount % 2 != 0;
}

string convertMarkdownToHTML(const string &input) {
    string result;
    bool insideBold = false;
    bool insideItalic = false;
    bool insideMonospaced = false;
    bool insidePreformatted = false;

    for (size_t i = 0; i < input.size(); ++i) {
        if ((insideBold && insideItalic) ||
            (insideBold && insideMonospaced) ||
            (insideBold && insidePreformatted) ||
            (insideItalic && insideMonospaced) ||
            (insideItalic && insidePreformatted) ||
            (insideMonospaced && insidePreformatted)) {
            return "ERROR";
        }
        if (input.substr(i, 3) == "```") {
            if (!insidePreformatted) {
                result += "<pre>";
                insidePreformatted = true;
            } else {
                result += "</pre>";
                insidePreformatted = false;
            }
            i += 3;
        } else if (insidePreformatted) {
            result += input[i];
            continue;
        } else if (input.substr(i, 2) == "**") {
            if (!insideBold) {
                result += "<b>";
                insideBold = true;
            } else {
                result += "</b>";
                insideBold = false;
            }
            i += 1;
        } else if (input.substr(i, 1) == "_" && !hasBothNeighbors(input, i)) {
            if (isalpha(input[i]) && i > 0 && input[i - 1] == '_') {
                result += input[i];
            } else {
                if (!insideItalic) {
                    result += "<i>";
                    insideItalic = true;
                } else {
                    result += "</i>";
                    insideItalic = false;
                }
            }
        } else if (input.substr(i, 1) == "`") {
            if (!insideMonospaced) {
                result += "<tt>";
                insideMonospaced = true;
            } else {
                result += "</tt>";
                insideMonospaced = false;
            }
        } else {
            result += input[i];
        }
    }
    vector<string> paragraphs;
    string htmlBuilder;
    size_t n = 0, m = 0;

    for (size_t i = 0; i <= input.size(); ++i) {
        m++;
        if (i == result.size() || result.substr(i, 2) == "\n\n") {
            paragraphs.push_back(result.substr(n, m - n - 1));
            n = m;
        }
    }

    for (const auto &paragraph: paragraphs) {
        if (!paragraph.empty()) {
            htmlBuilder.append("<p>").append(paragraph).append("</p>\n");
        }
    }
    return result;
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

    if (hasNestedMarkup(markdownContent)) {
        cerr << "Error: Nested markup detected." << endl;
        return EXIT_FAILURE;
    }

    string htmlContent = convertMarkdownToHTML(markdownContent);

    if (htmlContent == "ERROR") {
        cerr << "Error: Nested 2222." << endl;
        return EXIT_FAILURE;
    }

    string outputPath;
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
