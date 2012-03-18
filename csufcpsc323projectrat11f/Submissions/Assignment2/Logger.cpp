#include "Logger.h"
#include <ostream>
#include <fstream>
#include <string>
#include <queue>
#include <iostream>
using namespace std;

Logger::Logger()
{
}

Logger::~Logger()
{
}

Logger & Logger::operator=(const Logger & log)
{
	return *this;
}

Logger::Logger(const Logger & log)
{
   
}

void Logger::WriteToLogFile(const string& fileName)
{
    ofstream outFile;
    outFile.open(fileName, ofstream::out);
    if(outFile.is_open())
    while(!results.empty())
    {
        string result = results.front();
        results.pop();
        outFile << result << endl;
    }
    outFile.close();
}

void Logger::Log(const string& text)
{
    results.push(text);
    cout << text << endl;
}