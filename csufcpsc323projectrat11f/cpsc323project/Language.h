#pragma once
#ifndef _LANGUAGE_H_
#define _LANGUAGE_H_
using namespace std;
#include <string>

enum Token_t
{
    Identifier,
    Keyword,
    Integer,
    Real,
    Operator,
    Seperator,
    Error,
    Epsilon
};
enum Keyword_t
{
    k_function,
    k_int,
    k_boolean,
    k_real,
    k_if,
    k_endif,
    k_else,
    k_return,
    k_print,
    k_scan,
    k_while,
    k_true,
    k_false,
    k_length
};
enum Seperator_t
{
    s_openbracketround,
    s_closebracketround,
    s_openbracketsquare,
    s_closebracketsquare,
    s_openbracketcurly,
    s_closebracketcurly,
    s_semicolon,
    s_colon,
    s_comma,
    s_pound,
    s_length
};
enum Operator_t
{
    o_notequal,
    o_greatororequal,
    o_lessorequal,
	o_equalequal,
    o_equal,
    o_greatorthan,
    o_lessthan,
    o_plus,
    o_minus,
    o_multiply,
    o_divide,
    o_length
};

// Convert the keyword to a string
std::string ConvertKeyword(Keyword_t key);

// Convert the token to a string
std::string ConvertToken(Token_t tok);

// Convert the seperator to a string
std::string ConvertSeperator(Seperator_t sep);

// Convert the operator to a string
std::string ConvertOperator(Operator_t op);

bool isKeyword(std::string identifier);

#endif // _LANGUAGE_H_