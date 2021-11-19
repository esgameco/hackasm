#include "parse.h"

std::vector<uint16_t> Parse::parseFile(const std::string& fileName)
{
    std::ifstream file;
    std::vector<std::string> lines;
    std::map<std::string, int> labels = this->defaultLabels;
    std::vector<uint16_t> instructions;
    std::string line;
    bool error;
    uint16_t instruction;
    int currentStack = 16;
    int tmp;

    file.open(fileName);
    if (file.is_open())
    {
        while (std::getline(file, line))
            lines.push_back(line);

        this->parseLabels(lines, labels);

        for (const auto& line : lines)
        {
            instruction = parseInstruction(line, labels, tmp, &currentStack, false, &error);

            if (!error)
                instructions.push_back(instruction);
        }
    }
    else
        std::cout << "File didn't open." << std::endl;

    file.close();

    return instructions;
}

void Parse::parseLabels(const std::vector<std::string>& instructions, std::map<std::string, int>& labels)
{
    int currIns = 0;
    int tmp;
    bool error;

    for (const auto& line : instructions)
    {
        parseInstruction(line, labels, currIns, &tmp, true, &error);
        if (!error)
            currIns++;
    }
}

uint16_t Parse::parseInstruction
(
    std::string insStr, 
    std::map<std::string, int>& labels, 
    int currentIns,
    int* currentStack,
    bool firstPass,
    bool* error
)
{
    *error = false;
    uint16_t instruction = 0;
    int eqPos, semiPos, comPos, parenPos, atPos;
    std::string dest, ctrl, jmp, at, label;

    // Remove whitespace
    insStr = this->removeWhitespace(insStr);

    // If no characters after removing whitespace, return
    if (!insStr.size())
    {
        *error = true;
        return -1;
    }
 
    // Parse positions of separators
    eqPos = insStr.find("=");
    semiPos = insStr.find(";");
    comPos = insStr.find("//");
    parenPos = insStr.find("(");
    atPos = insStr.find("@");

    // Determine type of operation
    switch(insStr[0])
    {
        // A operation
        case '@':
        {
            at = this->getUntil(insStr, 1, this->atBreaks);

            // If at is a label, it will set the instruction to it
            if (labels.count(at))
                instruction += labels[at];
            // Otherwise, it will set the instruction to the number
            else
            {
                try
                {
                    int line = std::stoi(at);
                    instruction += line; 
                }
                catch (...) 
                {
                    if (!firstPass)
                    {
                        instruction += *currentStack;
                        labels[at] = *currentStack;
                        (*currentStack)++;
                    }
                }
            }
            break;
        }

        // Label declaration
        case '(':
            *error = true;

            if (firstPass)
            {
                label = this->getUntil(insStr, 1, this->labelBreaks);
                labels[label] = currentIns;
            }
            break;

        // Comment
        case '/':
            *error = true;
            break;
         
        // C operation
        default:
        {
            instruction += 7 << 13;

            // Split into components
            if (eqPos != std::string::npos)
                // dest = 'until ='
                dest = this->getUntil(insStr, 0, this->destBreaks); 
         
            // ctrl = 'after =, until semi or com
            ctrl = this->getUntil(insStr, (eqPos != std::string::npos) ? eqPos+1 : 0, this->ctrlBreaks);
            
            // jmp = 'after ;, until eol or com
            if (semiPos != std::string::npos)
                jmp = this->getUntil(insStr, semiPos+1, this->jmpBreaks);

            if (jmp.size())
            {
                if (jmp == "JGT")
                    instruction += 0b1;
                else if (jmp == "JEQ")
                    instruction += 0b10;
                else if (jmp == "JGE")
                    instruction += 0b11;
                else if (jmp == "JLT")
                    instruction += 0b100;
                else if (jmp == "JNE")
                    instruction += 0b101;
                else if (jmp == "JLE")
                    instruction += 0b110;
                else if (jmp == "JMP")
                    instruction += 0b111;
            }

            if (dest.size())
            {
                for (const auto& c : dest)
                {
                    switch (c)
                    {
                        case 'A':
                            instruction += 1 << 5;
                            break;
                        case 'D':
                            instruction += 1 << 4;
                            break;
                        case 'M':
                            instruction += 1 << 3;
                            break;
                        default:
                            break;
                    }
                }
            }

            if (ctrl.size())
            {
                if (ctrl == "0")
                    instruction += 0b0101010 << 6;
                else if (ctrl == "1")
                    instruction += 0b0111111 << 6;
                else if (ctrl == "-1")
                    instruction += 0b0111010 << 6;
                else if (ctrl == "D")
                    instruction += 0b0001100 << 6;
                else if (ctrl == "A")
                    instruction += 0b0110000 << 6;
                else if (ctrl == "!D")
                    instruction += 0b0001101 << 6;
                else if (ctrl == "!A")
                    instruction += 0b0110001 << 6;
                else if (ctrl == "-D")
                    instruction += 0b0001111 << 6;
                else if (ctrl == "-A")
                    instruction += 0b0110011 << 6;
                else if (ctrl == "D+1")
                    instruction += 0b0011111 << 6;
                else if (ctrl == "A+1")
                    instruction += 0b0110111 << 6;
                else if (ctrl == "D-1")
                    instruction += 0b0001110 << 6;
                else if (ctrl == "A-1")
                    instruction += 0b0110010 << 6;
                else if (ctrl == "D+A")
                    instruction += 0b0000010 << 6;
                else if (ctrl == "D-A")
                    instruction += 0b0010011 << 6;
                else if (ctrl == "A-D")
                    instruction += 0b0000111 << 6;
                else if (ctrl == "D&A")
                    instruction += 0b0000000 << 6;
                else if (ctrl == "D|A")
                    instruction += 0b0010101 << 6;
                else if (ctrl == "M")
                    instruction += 0b1110000 << 6;
                else if (ctrl == "!M")
                    instruction += 0b1110001 << 6;
                else if (ctrl == "-M")
                    instruction += 0b1110011 << 6;
                else if (ctrl == "M+1")
                    instruction += 0b1110111 << 6;
                else if (ctrl == "M-1")
                    instruction += 0b1110010 << 6;
                else if (ctrl == "D+M")
                    instruction += 0b1000010 << 6;
                else if (ctrl == "D-M")
                    instruction += 0b1010011 << 6;
                else if (ctrl == "M-D")
                    instruction += 0b1000111 << 6;
                else if (ctrl == "D&M")
                    instruction += 0b1000000 << 6;
                else if (ctrl == "D|M")
                    instruction += 0b1010101 << 6;
            }
            break;
        }
    }

    // Put together
    return instruction;
}

std::string Parse::getUntil(const std::string& insStr, int startPos, std::vector<char> comp)
{
    std::vector<char> insChars;

    for (int i = startPos; i < insStr.size(); i++)
    {
        if (std::find(comp.begin(), comp.end(), insStr[i]) != comp.end())
            break;
        insChars.push_back(insStr[i]);
    }

    std::string charsStr(insChars.begin(), insChars.end());
    return charsStr;
}

// Removes whitespace and returns a vector of chars
std::string Parse::removeWhitespace(const std::string& str)
{
    std::vector<char> chars;

    for (const char& c : str)
    {
        if (std::find(this->whitespace.begin(), this->whitespace.end(), c) == this->whitespace.end())
            chars.push_back(c);
    }

    std::string charsStr(chars.begin(), chars.end());
    return charsStr;
}
