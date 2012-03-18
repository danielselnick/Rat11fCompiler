#include "record.h"
#include <string>
using namespace std;

Record::Record()
{
}

Record::Record(Token_t tok, std::string lex)
{
    token = tok;
    lexeme = lex;
}

bool Record::isError()
{
    return (token == Error);    
}

std::string Record::ToString()
{
    if(token == Epsilon)
        return "";
    std::string result;
    result = "Token: ";
    result += ConvertToken(this->token);
    result += " Lexeme: " + lexeme;
    return result;
}

void Record::WriteToFile(std::ofstream& fileHandle)
{
    if(token != Epsilon)
        fileHandle << this->ToString() << std::endl;
}

const string Record::Lexeme() const
{
    return this->lexeme;
}

const Token_t Record::Token() const
{
    return this->token;
}

bool Record::operator==(const string & lexeme) const
{
    return this->lexeme == lexeme;
}

bool Record::operator==(const Token_t & token) const
{
    return this->token == token;
}

bool Record::operator==(const Keyword_t & keyword) const
{
    if(this->token == Keyword)
    {
        string key = ConvertKeyword(keyword);
        return this->lexeme == key;
    }
    return false;
}

bool Record::operator==(const Seperator_t & seperator) const
{
    if(this->token == Seperator)
    {
        string sep = ConvertSeperator(seperator);
        return this->lexeme == sep;
    }
    return false;
}

bool Record::operator==(const Operator_t & op) const
{
    if(this->token == Operator)
    {
        string oper = ConvertOperator(op);
        return this->lexeme == oper;
    }
    return false;
}

bool Record::operator==(const Record & record) const
{
	if((this->lexeme == record.lexeme) && (this->token == record.token))
	{
		return true;
	}
	else
		return false;
}

