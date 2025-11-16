#include "TextProcessor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

TextProcessor::TextProcessor()
{
    std::cout << "[TextProcessor] Default constructor called." << std::endl;
}

TextProcessor::TextProcessor(const TextProcessor&)
{
    std::cout << "[TextProcessor] Copy constructor called." << std::endl;
}

TextProcessor::~TextProcessor()
{
    std::cout << "[TextProcessor] Destructor called." << std::endl;
}

void TextProcessor::swapAdjacentWords(const std::string& inputFile,
                                      const std::string& outputFile) const
{
    std::ifstream in(inputFile);
    if (!in.is_open())
        throw std::runtime_error("Cannot open input file: " + inputFile);

    std::ostringstream buffer;
    buffer << in.rdbuf();
    if (in.bad())
        throw std::runtime_error("Error while reading the input file.");

    std::istringstream wordsStream(buffer.str());
    std::string first, second;
    std::string result;
    bool firstOutput = true;

    while (wordsStream >> first)
    {
        if (wordsStream >> second)
        {
            if (!firstOutput) result += ' ';
            result += second;
            result += ' ';
            result += first;
            firstOutput = false;
        }
        else
        {
            if (!firstOutput) result += ' ';
            result += first;
            firstOutput = false;
        }
    }

    std::ofstream out(outputFile);
    if (!out.is_open())
        throw std::runtime_error("Cannot open output file: " + outputFile);

    out << result;
    if (!out)
        throw std::runtime_error("Error while writing the output file.");

    std::cout << "\nProcessed text:\n" << result << std::endl;
    std::cout << "Result was saved to " << outputFile << std::endl;
}
