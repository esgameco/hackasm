#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <fstream>
#include <algorithm>
#include <map>

// Unused
enum class BitType
{
    JGT,
    JEQ,
    JLT,
    M,
    D,
    A,
    c6,
    c5,
    c4,
    c3,
    c2,
    c1,
    a,
    n2,
    n1,
    ins
};

class Parse
{
public:
    std::vector<uint16_t> parseFile(const std::string& fileName);
    void parseLabels(const std::vector<std::string>& instructions, std::map<std::string, int>& labels);
    uint16_t parseInstruction
    (
        std::string insStr, 
        std::map<std::string, int>& labels, 
        int currentIns,
        int* currentStack,
        bool firstPass,
        bool* error
    );
    std::string getUntil(const std::string& insStr, int startPos, std::vector<char> comp);
    std::string removeWhitespace(const std::string& str);

private:
    // Constants
    const std::vector<char> destBreaks = {'='};
    const std::vector<char> ctrlBreaks = {';', '/'};
    const std::vector<char> jmpBreaks = {'/'};
    const std::vector<char> labelBreaks = {')'};
    const std::vector<char> atBreaks = {'/'};
    const std::vector<char> whitespace = {' ', '\t', '\r', '\n', '\v', '\f'};
    const std::map<std::string, int> defaultLabels = {
        {"R0", 0},
        {"R1", 1},
        {"R2", 2},
        {"R3", 3},
        {"R4", 4},
        {"R5", 5},
        {"R6", 6},
        {"R7", 7},
        {"R8", 8},
        {"R9", 9},
        {"R10", 10},
        {"R11", 11},
        {"R12", 12},
        {"R13", 13},
        {"R14", 14},
        {"R15", 15},
        {"SP", 0},
        {"LCL", 1},
        {"ARG", 2},
        {"THIS", 3},
        {"THAT", 4},
        {"SCREEN", 16384},
        {"KBD", 24575},
    };
};
