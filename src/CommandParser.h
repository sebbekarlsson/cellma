#include <string>
#include <iostream>


#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

using namespace std;

class CommandParser {
    public:
        string lastCommand;
        CommandParser();
        bool parse(string command);
};

#endif
