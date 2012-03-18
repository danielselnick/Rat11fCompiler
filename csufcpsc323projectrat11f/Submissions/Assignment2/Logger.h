#pragma once
#ifndef _LOGGER_H_
#include <iostream>
#include <string>
#include <queue>
using namespace std;

class Logger
{
public:
    Logger();
    ~Logger();
    void WriteToLogFile(const string& fileName);
    void Log(const string& text);

private:
    Logger & operator=(const Logger & log);
    Logger(const Logger & log);
    queue<string> results;
};

#endif // _LOGGER_H_