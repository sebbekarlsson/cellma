#include "CommandParser.h"
#include <sstream>
#include <vector>

CommandParser::CommandParser() {
}

bool CommandParser::parse(string command) {
    std::stringstream cmd(command);

    while(std::getline(cmd, this->segment, ' '))
    {
        this->args.push_back(this->segment);
        cout << segment << endl;
    } 

    this->lastCommand = command;
   
    return true;
}
