#include "parse.h"

// Parses .asm file and returns a vector of 16-bit instructions
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

        file.close();
    }
    else
        std::cout << "File didn't open." << std::endl;

    return instructions;
}

// Performs the first pass by parsing the labels and ignoring the returned instructions
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

// Parses an instruction string and returns a 16-bit instruction, also parses labels on the first pass
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
                    // Attempts to set to a number, if it fails then sets to stack
                    int line = std::stoi(at);
                    instruction += line; 
                }
                catch (...) 
                {
                    // Only uses the stack during the second pass
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
                instruction += this->jmpTable.at(jmp);

            if (dest.size())
                for (const auto& c : dest)
                    instruction += 1 << this->destTable.at(c);

            if (ctrl.size())
                instruction += this->ctrlTable.at(ctrl) << 6;
            break;
        }
    }

    // Put together
    return instruction;
}

// Returns a string up to but not including the first of a set of comparison characters
std::string Parse::getUntil(const std::string& insStr, int startPos, std::vector<char> comp)
{
    std::vector<char> chars;

    for (int i = startPos; i < insStr.size(); i++)
    {
        // Checks whether each character is contained within the comparison vector
        if (std::find(comp.begin(), comp.end(), insStr[i]) != comp.end())
            break;
        chars.push_back(insStr[i]);
    }

    return std::string(chars.begin(), chars.end());
}

// Removes whitespace from a string
std::string Parse::removeWhitespace(const std::string& str)
{
    std::vector<char> chars;

    for (const char& c : str)
    {
        // Will only push back to chars if the character is not whitespace
        if (std::find(this->whitespace.begin(), this->whitespace.end(), c) == this->whitespace.end())
            chars.push_back(c);
    }

    return std::string(chars.begin(), chars.end());
}
