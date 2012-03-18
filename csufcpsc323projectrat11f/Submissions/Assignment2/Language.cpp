#include "Language.h"
using namespace std;
#include <string>

//todo: make sure all the syntax matches description in project document
std::string ConvertKeyword(Keyword_t key)
{
    switch(key)
    {
    case k_function:
        return "function";
    case k_int:
        return "int";
    case k_boolean:
        return "boolean";
    case k_real:
        return "real";
    case k_if:
        return "if";
    case k_endif:
        return "endif";
    case k_else:
        return "else";
    case k_return:
        return "return";
    case k_print:
        return "print";
    case k_scan:
        return "scan";
    case k_while:
        return "while";
    case k_true:
        return "true";
    case k_false:
        return "false";
    }
    return "Error in Convert Keyword";
}

std::string ConvertToken(Token_t tok)
{
    switch(tok)
    {
    case Identifier:
        return "Identifier";
    case Keyword:
        return "Keyword";
    case Integer:
        return "Integer";
    case Real:
        return "Real";
    case Operator:
        return "Operator";
    case Seperator:
        return "Seperator";
    case Error:
        return "Error";
    case Epsilon:
        return "";
    }
    return "Error in Convert Token";
}

std::string ConvertSeperator(Seperator_t sep)
{
    switch(sep)
    {
    case s_openbracketround:
        return "(";
        break;
    case s_closebracketround:
        return ")";
        break;
    case s_openbracketsquare:
        return "[";
        break;
    case s_closebracketsquare:
        return "]";
        break;
    case s_openbracketcurly:
        return "{";
        break;
    case s_closebracketcurly:
        return "}";
        break;
    case s_semicolon:
        return ";";
        break;
    case s_colon:
        return ":";
        break;
    case s_comma:
        return ",";
        break;
    case s_pound:
        return "#";
        break;
    }
    return "Error in Convert Seperator";
}

std::string ConvertOperator(Operator_t op)
{
    switch(op)
    {
    case o_notequal:
        return "/=";
    case o_greatororequal:
        return "=>";
    case o_lessorequal:
        return "<=";
    case o_equalequal:
        return "=";
    case o_equal:
        return ":=";
    case o_greatorthan:
        return ">";
    case o_lessthan:
        return "<";
    case o_plus:
        return "+";
    case o_minus:
        return "-";
    case o_multiply:
        return "*";
    case o_divide:
        return "/";
    }
    return "Error in Convert Operator";
}

bool isKeyword(string identifier)
{
    for(int i = 0; i < k_length; i++)
    {
        string keyword = ConvertKeyword((Keyword_t)i);
        if(keyword == identifier)
        {
            return true;
        }
    }
    return false;
}