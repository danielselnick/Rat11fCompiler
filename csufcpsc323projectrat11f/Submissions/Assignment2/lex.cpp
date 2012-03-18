#include "lex.h"
#include "record.h"
#include <iostream>
#include <ctype.h>
#include <stdlib.h>

inline bool isDigit(char c);
inline bool isPeriod(char c);
inline bool isUnderscore(char c);
inline bool isLetter(char c);

Lexer::Lexer()
{
    records = new std::list<Record>();
    state = 0;

    // Set the table to an incorrect state
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 7; j++)
            // -1 is non accepting state
            table[i][j] = -1;

    //// Add transition states to table
    // Table [0][] == d
    table[0][0] = 1;
    table[0][1] = 1;
    table[0][2] = 3;
    table[0][3] = 3;

    // Table [1][] == .
    table[1][0] = 2;
    table[1][1] = 2;

    // Table [2][] == _
    table[2][4] = 5;

    // Table [3][] == l 
    table[3][0] = 4;
    table[3][4] = 4;
    table[3][5] = 4;
    
    // Set the accepting states
    acceptingStates[0] = digitAcceptingState;
    acceptingStates[1] = realAcceptingState;
    acceptingStates[2] = idAcceptingState;
    acceptingStates[3] = idAcceptingState2;
}

Lexer::~Lexer()
{
    if(inputFile.is_open())
    {
        inputFile.close();
    }
    delete records;
}

/*
 * Main functionality of class.
 * Uses the input file, and parses one lexeme at a time
 */
Record Lexer::Lex()
{
    if(!inputFile.is_open())
    {
        return Record(Error, "Must successfully open a file to lex before lexing");
    }
    // Common functions:
    // http://www.cplusplus.com/reference/iostream/fstream/
    // file.get() get the next character and increment pointer
    // file.peek() get the next character and do not increment pointer
    // file.tellg() get the current location in the file
    // file.unget() decrement the pointer by one
    // Begin FSM
    char c; // The current character
    char d; // The character after the the current character
    lexeme = "";

    while(1) 
    {
        // Handle ending file with an accepting state
	    if(inputFile.eof() && isAcceptingState())
        {
		    return StateToRecord();
	    }
		// Handle end of file
        if(inputFile.eof())
        {
            return Record(Epsilon, "End of file");
        }
		
		// Set the current character to the next character.
        c = inputFile.get();
		// Peek at the character after the current character.
        d = inputFile.peek();
        
		//// Check to see if the current character is an epsilon
	    char epsilon[5];
	    epsilon[0] = ' ';	// Check to see if it's white space
	    epsilon[1] = 10;	// this is for end of line
	    epsilon[2] = 9;		// this one is for tab
	    epsilon[3] = 4;		// this checks for eof
	
	    bool check = false; 
	    for (int i = 0 ; i < 4 ; i++)
	    {
		    if (epsilon[i] == c)
		    {
			    if(isAcceptingState())
			    { 
				    return StateToRecord();
			    }
			    else
			    {
				    check = true;
					break;
			    }
		    } 
	    }
		// Go back to the beginning of the while loop if we encountered an epsilon
	    if( check == true)
	    {	
		    continue;
	    }

        //// Check to see if it's an operator
        for(int i = 0; i < o_length; i++)
        {
            string op = ConvertOperator((Operator_t)i);
            if(op[0] == c)
            {
                // If it's a two character operator
                if(op.length() == 2)
                {
                    // If the second character is the next character in the file
                    if(op[1] == d)
                    {
                        if(isAcceptingState())
                        {
                            return StateToRecord();
                        }
                        else
                        {
                            //We are consuming d, so increment the inputFile
                            char increment = inputFile.get();
                            // Return our record
                            Record result(Operator, op);
                            records->push_back(result);
                            return result;
                        }
                    }
                }
                else // It is a one character operator, and the first character was correct
                {
                    if(isAcceptingState())
                    {                    
                        return StateToRecord();
                    }
                    else
                    {
                        Record result(Operator, op);
                        records->push_back(result);
                        return result;
                    }
                }
            }
        }

        //// Check to see if it's a seperator
        for(int i = 0; i < s_length; i++)
        {
            string seperator = ConvertSeperator((Seperator_t)i);
            if(seperator[0] == c)
            {   
                if(isAcceptingState())
                {
                    return StateToRecord();
                }
                else
                {
                    Record result(Seperator, seperator);
				    records->push_back(result);
				    return result;
                }
            }
        }

        //// Real and integer finite state machine
        // Determine which column is the input
		char inputClass = 0;
        if(isDigit(c))
        {
            inputClass = 0;
        }
        else if(isPeriod(c))
        {
            inputClass = 1;
        }
        else if(isUnderscore(c))
        {
            inputClass = 2;
        }
        else if(isLetter(c))
        {
            inputClass = 3;
        }
	    else // This isn't a valid character for any part of the lexer
	    {
			lexeme+=c;
			Record result(Error, lexeme);
            records->push_back(result);
			return result;
	    }
       
        // update the state via the table
        state = table[inputClass][state];

        // If we go into a bad state
        if(state == -1)
        {
            lexeme += c;
            Record result(Error, lexeme);
            records->push_back(result);
            return result;
        }
		
        // Add the current character to the current lexeme
        lexeme += c;
		
		// Go back to the top of the loop
    }
	// If the loop exits, then we have a problem.
    records->push_back(Record(Error, "Error Parsing"));
    return Record(Error, "Error Parsing");
}

Record Lexer::StateToRecord()
{
    Token_t token = StateToToken();
    Record result(token, lexeme);
    records->push_back(result);
    inputFile.unget();
    state = 0;
    return result;
}

Token_t Lexer::StateToToken()
{
    switch (state)
    {
    case digitAcceptingState:
        return (Token_t)Integer;
    case realAcceptingState:
        return (Token_t)Real;
    case idAcceptingState:
        // continue to next case
    case idAcceptingState2:
        if(isKeyword(lexeme))
            return (Token_t)Keyword;
        else
            return (Token_t)Identifier;
    }
    return Error;
}

bool Lexer::WriteRecordsToFile(std::string outputFileName)
{
    ofstream outputFile;
    outputFile.open(outputFileName, ofstream::out);
    if(outputFile.is_open() && (records->size() > 0))
    {
        // foreach loop to write every record in the list of records to the file
        for(list<Record>::iterator it = records->begin(); it != records->end(); it++)
        {
            it->WriteToFile(outputFile);
        }         
        outputFile.close();

        return true;
    }
    else
    {
        return false;
    }
}

bool Lexer::isAcceptingState()
{
    for(int i = 0; i < numberOfAcceptingStates; i++)
    {
        if(acceptingStates[i] == state)
            return true;
    }
    return false;
}

bool Lexer::isEndOfFile()
{
    if(inputFile.is_open())
        return inputFile.eof();
    return false;
}

bool Lexer::OpenFileToLex(std::string fileToOpen)
{
    if(inputFile.is_open())
    {
        inputFile.close();
    }
    inputFile.open(fileToOpen, ifstream::in);
    return inputFile.is_open();
}

/*
 * Overloaded and made private so that the lexer can't be copied
 */
Lexer& Lexer::operator=(const Lexer &lex)
{
    return *this;
}
/*
 * Overloaded and made private so that the lexer can't be copied
 */
Lexer::Lexer(const Lexer &lex)
{
}

bool isLetter(char c)
{
	return (((c>=65) && (c<= 90)) ||( (c>=97) && (c<=122)));
}

bool isDigit(char c)
{
    return(('0' <= c) && (c <= '9'));
}

bool isPeriod(char c)
{
    return (c == '.');    
}

bool isUnderscore(char c)
{
    return (c == '_');   
}