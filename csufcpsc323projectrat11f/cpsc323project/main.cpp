/*
 * Daniel Selnick
 * Angel Castrejon
 * CPSC 323 Morning Section
 * Compiler Project - Assignment 1
 */
#include "record.h"
#include "lex.h"
#include <iostream>
#include "SyntaxAnalyzer.h"
using namespace std;
#define asgt2
// Helper function which drives the Lexical analyzer
bool RunLexer(Lexer&);

/*
 * Main
 * Arguments:
 * [0] the exe location
 * [1] the file to read
 * [2] the file to write
 * [3] if we are in silent mode
 */
int main(int argc, char **argv)
{
    string openFileLocation;
    string writeFileLocation;
	string writeTableLocation;
    int assignmentNum = 2;
    // Handle arguments before creating anything
    if(argc < 2)
    {     
        cout << "Enter input file: " << endl;
        cin >> openFileLocation;
        cout << "Enter output file: " << endl;
        cin >> writeFileLocation;
    }

#ifdef asgt1
    Lexer lexer;
    lexer.OpenFileToLex(openFileLocation);

    // If the lexer completed with no errors.
    if(!RunLexer(lexer))
    {        
        if(lexer.WriteRecordsToFile("lex_" + writeFileLocation))
        {
            // later assignments go here
            cout << "Records written to: " << "lex_" + writeFileLocation << endl;            
        }
        else
        {
            cout << "Error writing records to: " << writeFileLocation << endl;
        }
    }
    else // If the lexer had an error
    {
        if(lexer.WriteRecordsToFile("lex_" + writeFileLocation))
        {
            cout << "Records written to: lex_" << writeFileLocation << endl;
        }
        else
        {
            cout << "Error writing records to lex_" << writeFileLocation << endl;
        }
        cout << "Error in parsing input file" << endl;
    }
#endif
#ifdef asgt2
    SyntaxAnalyzer* analyzer = new SyntaxAnalyzer();
    Lexer* lex = new Lexer();
    if(argc < 2)
    {
        if(lex->OpenFileToLex(openFileLocation))
        {
            analyzer->Parse(lex);
            analyzer->PrintResults(writeFileLocation);
        }
        else
            cout << "Unable to open file! " << openFileLocation << endl;
    }
    else
    {
        if(lex->OpenFileToLex(argv[1]))
        {
            analyzer->Parse(lex);
            analyzer->PrintResults(argv[2]);
        }
        else
            cout << "Unable to open file! " << openFileLocation << endl;
    }
    delete lex;
    delete analyzer;
#endif
   
    system("pause");
}

bool RunLexer(Lexer & lexer)
{
    bool isError = false;
    while(!lexer.isEndOfFile())
    {
        Record record = lexer.Lex();
        if(record.isError())
        {
            isError = true;
        }
        cout << record.ToString() << endl;
    } // continue loop
    return isError;
}