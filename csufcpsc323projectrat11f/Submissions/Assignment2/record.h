#ifndef _RECORD_H_
#define _RECORD_H_
#include "Language.h"
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

class Record
{
private:
    Token_t token;
    std::string lexeme;
public:
    Record();
    // The record can be constructed with a token and lexeme
    Record(Token_t tok, std::string lex);
    // The token and lexeme in string format
    std::string ToString();
    // Pre-condition: a correctly opened file handle
    void WriteToFile(std::ofstream& fileHandle);
    // If the record is reporting an error
    bool isError();
    const string Lexeme() const;
    const Token_t Token() const;
    bool operator==(const string &) const;
    bool operator==(const Token_t &) const;
    bool operator==(const Keyword_t &) const;
    bool operator==(const Seperator_t &) const;
    bool operator==(const Operator_t &) const;
};

#endif // _RECORD_H_