#include "CommandParser.h"


CommandParser::CommandParser() {
}

bool CommandParser::parse(string command) {
    cout << command << endl;
    this->lastCommand = command;
    return true;
}
