#ifndef _LEX_H_
#define _LEX_H_
#include <string>
#include <fstream>
#include <list>
#include "record.h"
using namespace std;

class Lexer
{
public:
    /*
     * Public constructor, configures all data for FSM.
     */
    Lexer();
    /*
     * Close file and delete records.
     */
    ~Lexer();
    /*
     * Extract a Record from the file.
     */
    Record Lex();
    /*
     * Write the privately held records to a file.
     */
    bool WriteRecordsToFile(std::string fileLocationandName);
    /*
     * Open a file to extract tokens from.
     */
    bool OpenFileToLex(std::string filetoOpen);
    /*
     * Public access to if we're at the end of the file being parsed.
     */
    bool isEndOfFile();
private:
    
    //// Data

    // A list of all records which have been extracted from the Lex function.
    list<Record> *records;
    // The file where records are extracted.
    ifstream inputFile;
    // The current lexeme
    string lexeme;
    // The current state
    char state;
    // FSM state table
    char table[4][7];
    // Accepting states for the FSM
    static const char numberOfAcceptingStates  = 4;
    static const char digitAcceptingState      = 1;
    static const char realAcceptingState       = 3;
    static const char idAcceptingState         = 4;
    static const char idAcceptingState2        = 5;
    // Array of accepting states for iteration
    char acceptingStates[numberOfAcceptingStates];

    //// Private functions

    // No copying or assignment allowed
    Lexer & operator=(const Lexer &lex);
    Lexer(const Lexer &lex);

    //// Helper functions to the Lexer
    // If the current state of the FSM is in an accepting state
    bool isAcceptingState();    
    // Determines the current token type based on the current state of the FSM
    Token_t StateToToken();
    // Generates a record based on the current state of the FSM
    Record StateToRecord();
};

#endif // _LEX_H_