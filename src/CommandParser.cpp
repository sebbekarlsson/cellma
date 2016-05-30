#include "CommandParser.h"

CommandParser::CommandParser() {
}

bool CommandParser::Parse(string command) {
    this->lastCommand = command;
    return true;
}
