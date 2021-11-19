#include <bitset>

#include "parse.h"

/*
 * args:
 * 0 -> input
 * 1 -> output
 */
int main(int argc, char* argv[])
{
    std::string inFile(argv[1]);
    std::string outFile(argv[2]);

    Parse parser;
    std::vector<uint16_t> instructions = parser.parseFile(inFile);

    std::ofstream out;
    out.open(outFile);

    if (out.is_open())
    {
        for (const auto& ins : instructions)
            out << std::bitset<16>(ins) << std::endl;
    }
    else
        std::cout << "Couldn't open file";

    return 0;
}
