#include "converter.hpp"
#include <string_view>

std::string Converter::markdownToHTML(std::string markdownText) {
    std::vector<std::string> preBlocks;
    std::regex preBlockPattern("```([\\s\\S]*?)```");
    std::regex boldPattern("\\*\\*(.*?)\\*\\*");
    std::regex italicPattern("_([a-zA-Z][a-zA-Z\\s_]*[a-zA-Z])_");
    std::regex ttPattern("`([a-zA-Z][a-zA-Z\\s_]*[a-zA-Z])`");


    std::smatch match;
    while (std::regex_search(markdownText, match, preBlockPattern)) {
        preBlocks.push_back(match.str(1));
        markdownText = std::regex_replace(markdownText, preBlockPattern, "%%PRE_BLOCK%%",
                                          std::regex_constants::format_first_only);
    }

    markdownText = std::regex_replace(markdownText, boldPattern, "<b>$1</b>");
    markdownText = std::regex_replace(markdownText, italicPattern, "<i>$1</i>");
    markdownText = std::regex_replace(markdownText, ttPattern, "<tt>$1</tt>");

    _checkCoherence(markdownText);
    _checkOpenedTags(markdownText);

    for (size_t i = 0; i < preBlocks.size(); ++i) {
        markdownText = std::regex_replace(markdownText, std::regex("%%PRE_BLOCK%%"), "<pre>" + preBlocks[i] + "</pre>",
                                          std::regex_constants::format_first_only);
    }

    std::vector<std::string> paragraphs = splitParagraphs(markdownText);

    return std::accumulate(paragraphs.begin(), paragraphs.end(), std::string{},
                           [](const std::string &a, const std::string &b) {
                               return a + '\n' + b;
    });
}

std::vector<std::string> Converter::splitParagraphs(const std::string& markdownText) {
    std::vector<std::string> paragraphs;

    std::regex paragraphSeparator("\\n\\s*\\n");
    std::sregex_token_iterator iter(markdownText.begin(), markdownText.end(), paragraphSeparator, -1);
    std::sregex_token_iterator end;
    for (; iter != end; ++iter) {
        std::string paragraph = *iter;
        paragraph = std::regex_replace(paragraph, std::regex("^\\s+|\\s+$"), "");

        if (!std::regex_search(paragraph, std::regex("<\\/?[a-z][\\s\\S]*>", std::regex_constants::icase))) {
            paragraph = "<p>" + paragraph + "</p>";
        }
        paragraphs.push_back(paragraph);
    }

    return paragraphs;
}

void Converter::_checkCoherence(const std::string &markdownText) {
    std::regex tagPattern("<\\/?[^>]+>");
    std::sregex_iterator it(markdownText.begin(), markdownText.end(), tagPattern);
    std::sregex_iterator it_end;

    std::vector<std::string> tags;
    while (it != it_end) {
        tags.push_back(it->str());
        ++it;
    }

    for (size_t i = 0; i < tags.size(); i += 2) {
        if (tags[i] != std::regex_replace(tags[i + 1], std::regex("/"), "")) {
            throw std::runtime_error("There are nested loops");
        }
    }
}

void Converter::_checkOpenedTags(const std::string &markdownText) {
    std::vector<std::string> words;
    std::istringstream iss(markdownText);
    for (std::string s; iss >> s;)
        words.push_back(s);

    std::vector<std::string> markdownSymbols = {"**", "_", "`"};
    for (const auto &word: words) {
        for (const auto &symbol: markdownSymbols) {
            if ((word.starts_with(symbol) || word.ends_with(symbol)) && word != symbol) {
                throw std::runtime_error("There is not finished markdown - " + word);
            }
        }
    }
}

