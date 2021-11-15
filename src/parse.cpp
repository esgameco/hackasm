#include "parse.h"

std::vector<uint16_t> Parse::parseFile(std::string fileName)
{
    std::ifstream file;
    std::vector<uint16_t> instructions;
    std::string line;

    file.open(fileName);
    while (file.getLine(line))
    {
        instructions.push_back(parseInstruction(line));
    }
    file.close();

    return instructions;
}

uint16_t Parse::parseInstruction(std::string insStr)
{
    uint16_t instruction;

    // Split into components

    
    // Translate into binary

    // Put together
}
