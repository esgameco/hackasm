#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <fstream>

class Parse
{
public:
    std::vector<uint16_t> parseFile(std::string fileName);
    uint16_t parseInstruction(std::string insStr);
};
