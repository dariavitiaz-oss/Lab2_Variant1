#pragma once
#include <string>

class TextProcessor
{
public:
    TextProcessor();
    TextProcessor(const TextProcessor& other);
    ~TextProcessor();

    void swapAdjacentWords(const std::string& inputFile,
                           const std::string& outputFile) const;
};
