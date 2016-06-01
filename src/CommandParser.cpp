#include "CommandParser.h"
#include <sstream>
#include <vector>

CommandParser::CommandParser() {
}

bool CommandParser::parse(string command) {
    std::stringstream test(command);
    std::string segment;
    std::vector<std::string> args;

    while(std::getline(test, segment, ' '))
    {
        args.push_back(segment);
        cout << segment << endl;
    } 

    this->lastCommand = command;
    return true;
}
