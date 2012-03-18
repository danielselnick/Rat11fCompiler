#pragma once
#ifndef _PARSE_H_
#define _PARSE_H_
#include <string>
#include <fstream>
#include <iostream>
#include "record.h"
#include "Logger.h"
#include "lex.h"
using namespace std;

class SyntaxAnalyzer
{
public:
    SyntaxAnalyzer();
    ~SyntaxAnalyzer();
    void Parse(Lexer * lexer);
    void PrintResults(string fileLocation);
private:
    SyntaxAnalyzer & operator=(const SyntaxAnalyzer &parse)
    {}
    SyntaxAnalyzer(const SyntaxAnalyzer &parse)
    {}
    Lexer * lexer;
    Record rrr;
    Logger logger;
    bool paramList;

    void NextRecord();

    void insert(Token_t, string);
    void insert(Record);
    void insert(Seperator_t);
    void insert(Operator_t);
    void insert(Keyword_t);

    /* <Rat11F> ::= # <Opt Function Definitions>  
                    # <Opt Declaration List> <Statement List> # */
    void Rat11f();

    /* <Opt Function Definitions> ::= <Function Definitions> | epsilon */
    void OptFunctionDefinitions();

    /* <Function Definitions> ::= <Function> <Function Definitions Prime> */
    void FunctionDefinitions();

    /* <Function Definitions Prime> :: <Function Definitions> | epsilon */
    void FunctionDefinitionsPrime();

    /* <Function> ::= function <Identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body> */
    void Function();

    /* <Opt Parameter List> ::= <Parameter List> | epsilon */
    void OptParameterList();

    /* <Parameter List> ::= <Parameter> <Parameter List Prime> */
    bool ParameterList();

    /* <Parameter List Prime> ::= , <Parameter List> | epsilon */
    void ParameterListPrime();

    /* <Parameter> ::= <IDs> : <Qualifier> */
    bool Parameter();

    /* <Qualifier> ::= int | boolean | real */
    bool Qualifier();

    /* <Body> ::= { <Statement List> } */
    void Body();

    /* <Opt Declaration List> ::= <Declaration List> | epsilon */
    void OptDeclarationList();

    /* <Declaration List> ::= <Declaration> ; <Declaration List Prime> */
    bool DeclarationList();

    /* <Declaration List Prime> ::= <Declaration List> | epsilon */
    void DeclarationListPrime();

    /* <Declaration> ::= <Qualifier> <IDs> */
    bool Declaration();

    /* <IDs> ::= <Identifier> <IDs Prime> */
    bool IDs();

    /* <IDs Prime> ::= , <IDs> | epsilon */
    void IDsPrime();

    /* <Statement List> ::= <Statement> <Statement List Prime> */
    bool StatementList();

    /* <Statement List Prime> ::= <Stamement List> | epsilon */
    void StatementListPrime();

    /* <Statement> ::= <Compound> | <Assign> | <If> | <Return> | <Write> | <Read> | <While> */
    bool Statement();

    /* <Compound> ::= { <Statement List> } */
    bool Compound();

    /* <Assign> ::= <Identifier> := <Expression> ; */
    bool Assign();

    /* <If> ::= if ( <Condition> ) <Statement> <If Prime> */
    bool If();

    /* <If Prime> ::= endif | else <Statement> endif */
    void IfPrime();

    /* <Return> ::=  return <Return Prime> */
    bool Return();

    /* <Return Prime> ::= <Expression> ; | ; epsilon */
    void ReturnPrime();

    /* <Write> ::= print ( <Expression> ); */
    bool Write();

    /* <Read> ::= scan ( <IDs> ); */
    bool Read();

    /* <While> ::= while ( <Condition> ) <Statement> */
    bool While();

    /* <Condition> ::= <Expression> <Relop> <Expression> */
    void Condition();

    /* <Relop> ::= = | /= | > | < | => | <= */
    void Relop();

    /* <Expression> ::= <Term> <Expression Prime> */
    void Expression();

    /* <Expression Prime> ::= + <Term> <Expression Prime> | - <Term> <Expression Prime> | epsilon */
    void ExpressionPrime();

    /* <Term> ::= <Factor> <Term Prime> */
    void Term();

    /* <Term Prime> ::= * <Factor> <Term Prime> | / <Factor> <Term Prime> | epsilon */
    void TermPrime();

    /* <Factor> ::= - <Primary> | <Primary> */
    void Factor();

    /* <Primary> ::= <Identifier> <PrimaryPrime> | <Integer> | ( <Expression> ) | <Real> | true | false */
    void Primary();

    /* <PrimaryPrime> ::= [<IDs>] | epsilon */
    void PrimaryPrime();
};

#endif // _PARSE_H_