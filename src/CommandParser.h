#include <string>
#include <iostream>
#include <vector>


#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

using namespace std;

class CommandParser {
    public:
        string lastCommand;
        std::string segment;
        std::vector<std::string> args;
        CommandParser();
        bool parse(string command);
};

#endif
