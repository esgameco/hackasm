#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <fstream>
#include <algorithm>
#include <map>

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
    const std::map<std::string, int> defaultLabels{
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
    // Stats at 0
    const std::map<std::string, int> jmpTable{
        {"JGT", 0b1},
        {"JEQ", 0b10},
        {"JGE", 0b11},
        {"JLT", 0b100},
        {"JNE", 0b101},
        {"JLE", 0b110},
        {"JMP", 0b111},
    };
    // 1 << x
    const std::map<char, int> destTable{
        {'A', 5},
        {'D', 4},
        {'M', 3}
    };
    // Do << 6 at the end
    const std::map<std::string, int> ctrlTable{
        {"0", 0b0101010},
        {"1", 0b0111111},
        {"-1", 0b0111010},
        {"D", 0b0001100},
        {"A", 0b0110000},
        {"!D", 0b0001101},
        {"!A", 0b0110001},
        {"-D", 0b0001111},
        {"-A", 0b0110011},
        {"D+1", 0b0011111},
        {"A+1", 0b0110111},
        {"D-1", 0b0001110},
        {"A-1", 0b0110010},
        {"D+A", 0b0000010},
        {"D-A", 0b0010011},
        {"A-D", 0b0000111},
        {"D&A", 0b0000000},
        {"D|A", 0b0010101},
        {"M", 0b1110000},
        {"!M", 0b1110001},
        {"-M", 0b1110011},
        {"M+1", 0b1110111},
        {"M-1", 0b1110010},
        {"D+M", 0b1000010},
        {"D-M", 0b1010011},
        {"M-D", 0b1000111},
        {"D&M", 0b1000000},
        {"D|M", 0b1010101},
    };
};
