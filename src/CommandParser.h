#include <string>
#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

using namespace std;

class CommandParser {
    public:
        string lastCommand;
        CommandParser();
        bool Parse(string command);
};

#endif
