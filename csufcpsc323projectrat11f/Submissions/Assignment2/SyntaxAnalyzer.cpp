#include "SyntaxAnalyzer.h"
#include "lex.h"
#include "record.h"
#include "Logger.h"

SyntaxAnalyzer::SyntaxAnalyzer()
{
    rrr = Record(Epsilon, "Begin Syntax Analysis");
}
SyntaxAnalyzer::~SyntaxAnalyzer()
{
}
/* Parse the syntax for every lexeme in the lexer. */     
void SyntaxAnalyzer::Parse(Lexer * lex)
{
    lexer = lex;
    Rat11f();
}

void SyntaxAnalyzer::PrintResults(string filelocation)
{
    this->logger.WriteToLogFile(filelocation);
}
/* Sets the internal record variable to the next record produced by the lexer. */
void SyntaxAnalyzer::NextRecord()
{
    this->logger.Log(rrr.ToString());
    this->rrr = lexer->Lex();
}

void SyntaxAnalyzer::insert(Record record)
{
    this->logger.Log("Error! Expected Record { " + record.ToString() + "}");
    this->logger.Log("Inserted fake expected Record");
    this->rrr = record;
    NextRecord();
}

void SyntaxAnalyzer::insert(Token_t token, string lexeme)
{
    Record record(token, lexeme);
    insert(record);
}

void SyntaxAnalyzer::insert(Seperator_t sep)
{
    string lexeme = ConvertSeperator(sep);
    insert(Seperator, lexeme);
}

void SyntaxAnalyzer::insert(Operator_t op)
{
    string lexeme = ConvertOperator(op);
    insert(Operator, lexeme);
}

void SyntaxAnalyzer::insert(Keyword_t key)
{
    string lexeme = ConvertKeyword(key);
    insert(Keyword, lexeme);
}

/* <Rat11F> ::= # <Opt Function Definitions>
                # <Opt Declaration List> <Statement List> # */
void SyntaxAnalyzer::Rat11f()
{
    // start it off
    NextRecord();
    if(rrr == s_pound)
    {   
        NextRecord();
        OptFunctionDefinitions();
        // will return with next next record as a #
        if(rrr == s_pound)
        {
            NextRecord();
            OptDeclarationList();
            StatementList();
            if(rrr == s_pound)
            {
                NextRecord();
                logger.Log("<Rat11F> ::= # <Opt Function Definitions># <Opt Declaration List> <Statement List> #");
            }
			else
			{
                insert(s_pound);
			}
        }
		else
		{
            insert(s_pound);
		}
    }
    else
    {
        insert(s_pound);
    }
}
/* <Opt Function Definitions> ::= <Function Definitions> | epsilon */
void SyntaxAnalyzer::OptFunctionDefinitions()
{
    if(rrr == k_function)
    {  
		FunctionDefinitions();
        logger.Log("<Opt Function Definitions> ::= <Function Definitions>");
    }
    else
    {
        // epsilon
		logger.Log("<Opt Function Definitions> ::= epsilon");
    }
}
/* <Function Definitions> ::= <Function> <Function Definitions Prime> */
void SyntaxAnalyzer::FunctionDefinitions()
{
	Function();
	FunctionDefinitionsPrime();
    logger.Log("<Function Definitions> ::= <Function> <Function Definitions Prime>");
}
/* <Function Definitions Prime> :: <Function Definitions> | epsilon */
void SyntaxAnalyzer::FunctionDefinitionsPrime()
{
    if(rrr == k_function)
    {       
	   FunctionDefinitions();
       logger.Log("<Opt Function Definitions Prime> ::= <Function Definitions>");
    }
    else
    {
        // epsilon
		logger.Log("<Opt Function Definitions> ::= epsilon");
    }
}
/* <Function> ::= function <Identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body> */
void SyntaxAnalyzer::Function()
{
    if(rrr == k_function)
    {
        NextRecord();
        if(rrr == Identifier)
        {
            NextRecord();
            if(rrr == s_openbracketround)
            {
                NextRecord();
                OptParameterList();                
                if(rrr == s_closebracketround)
                {			
                    NextRecord();
                    OptDeclarationList();
                    Body();
                    logger.Log("<Function> ::= function <Identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body>");
                }
                else
                {
                    insert(s_closebracketround);
                }
            }
            else
            {
                insert(s_openbracketround);
            }
        }
        else
        {
            insert(Identifier, "MISSING IDENTIFIER");
        }
    }
    else
    {
        insert(k_function);
    }
}
/* <Opt Parameter List> ::= <Parameter List> | epsilon */
void SyntaxAnalyzer::OptParameterList()
{
    paramList = false;
	if(ParameterList())
    {
		logger.Log("<Opt Parameter List> ::= <Parameter List>");
    }
	else
    {
        // epsilon
		logger.Log("<Opt Parameter List> ::= epsilon");
    }
}
/* <Parameter List> ::= <Parameter> <Parameter List Prime> */
bool SyntaxAnalyzer::ParameterList()
{    
	if(Parameter())
    {
	    ParameterListPrime();
		return paramList;
    }
    else
    {
        // end of paramlist
        return paramList;
    }
}
/* <Parameter List Prime> ::= , <Parameter List> | epsilon */
void SyntaxAnalyzer::ParameterListPrime()
{
    if(rrr == s_comma)
    {
        NextRecord();
        paramList = false;
		ParameterList();
        logger.Log("<Parameter List Prime> ::= , <Parameter List>");
    }
    else
    {
        // epsilon
		logger.Log("<Parameter List Prime> ::= epsilon");
    }
}
/* <Parameter> ::= <IDs> : <Qualifier> */
bool SyntaxAnalyzer::Parameter()
{
	if(IDs())
    {        
        if(rrr == s_colon)
        {
            NextRecord();
	        if(Qualifier())
            {                
				logger.Log("<Parameter> ::= <IDs> : <Qualifier>");
                return true;
            }
            else
            {
               insert(k_int);
               return true;
            }
        }
        else
        {
            insert(s_colon);
            return true;
        }
    }
    else
    {
        // no error
        return false;
    }
}
/* <Qualifier> ::= int | boolean | real */
bool SyntaxAnalyzer::Qualifier()
{
	if(rrr == k_int)
	{
        NextRecord();
		logger.Log("<Qualifier> ::= int");
        return true;
	}
	else if(rrr == k_boolean)
	{
        NextRecord();        
		logger.Log("<Qualifier> ::= boolean");
        return true;
	}
	else if(rrr == k_real)
	{
        NextRecord();
		logger.Log("<Qualifier> ::= real");
        return true;
	}
	else{
        // no error
        return false;
	}
}
/* <Body> ::= { <Statement List> } */
void SyntaxAnalyzer::Body()
{
    if(rrr == s_openbracketcurly)
    {
        NextRecord();
        StatementList();
        
        if(rrr == s_closebracketcurly)
        {
            NextRecord();
			logger.Log("<Body> ::= { <Statement List> }");
        }
        else
        {
            insert(s_closebracketcurly);
        }
    }
    else
    {
        insert(s_openbracketcurly);
    }
}
/* <Opt Declaration List> ::= <Declaration List> | epsilon */
void SyntaxAnalyzer::OptDeclarationList()
{
	if(DeclarationList())
    {
        logger.Log("<Opt Declaration List> ::= <Declaration List>");
    }
	else
    {
        // epsilon
        logger.Log("<Opt Declaration List> ::= epsilon");
    }
}
/* <Declaration List> ::= <Declaration> ; <Declaration List Prime> */
bool SyntaxAnalyzer::DeclarationList()
{
	if(Declaration())
    {        
        if(rrr == s_semicolon)
        {
		    NextRecord();
	        DeclarationListPrime();
            logger.Log("<Declaration List> ::= <Declaration> ; <Declaration List Prime>");
            return true;
        }
        else
        {
            insert(s_semicolon);
            return true;
        }
	}
    else
    {
        // no error
        return false;
    }
}
/* <Declaration List Prime> ::= <Declaration List> | epsilon */
void SyntaxAnalyzer::DeclarationListPrime()
{
	if(DeclarationList())
    {
		logger.Log("<Declaration List Prime> ::= <Declaration List>");
    }
	else // if(epsilon())
    {
		logger.Log("<Declaration List Prime> ::= epsilon");
    }
}
/* <Declaration> ::= <Qualifier> <IDs> */
bool SyntaxAnalyzer::Declaration()
{
	if(Qualifier())
	{ 
        IDs();
        logger.Log("<Declaration> ::= <Qualifier> <IDs>");
        return true;
    }
    else
    {
        return false;
    }
}
/* <IDs> ::= <Identifier> <IDs Prime> */
bool SyntaxAnalyzer::IDs()
{
    if(rrr == Identifier)
    {		
        NextRecord();
        IDsPrime();
        logger.Log("<IDs> ::= <Identifier> <IDs Prime>");
        return true;
    }
    else
    {
        // no error
        return false;
    }
}
/* <IDs Prime> ::= , <IDs> | epsilon */
void SyntaxAnalyzer::IDsPrime()
{
    if(rrr == s_comma)
    {
        NextRecord();
	    IDs();
        logger.Log("<IDs Prime> ::= , <IDs>");
    }
    else
    {
		logger.Log("<IDs Prime> ::= epsilon");
    }
}
/* <Statement List> ::= <Statement> <Statement List Prime> */
bool SyntaxAnalyzer::StatementList()
{
	if(Statement())
    {
	    StatementListPrime();
        logger.Log("<Statement List> ::= <Statement> <Statement List Prime>");
        return true;
    }
    else
    {
        return false;
    }
}
/* <Statement List Prime> ::= <Stamement List> | epsilon */
void SyntaxAnalyzer::StatementListPrime()
{
	if(StatementList())
    {
		logger.Log("<Statement List Prime> ::= <Stamement List>");
    }
	else
    {
        // epsilon
		logger.Log("<Statement List Prime> ::= epsilon");
    }    
}
/* <Statement> ::= <Compound> | <Assign> | <If> | <Return> | <Write> | <Read> | <While> */
bool SyntaxAnalyzer::Statement()
{
    
	if(Compound())
    {
		logger.Log("<Statement> ::= <Compund>");
        return true;
    }
	else if(Assign())
    {
        logger.Log("<Statement> ::= <Assign>");
        return true;
    }
	else if(If())
    {
		logger.Log("<Statement> ::= <If>");
        return true;
    }
	else if(Return())
    {
		logger.Log("<Statement> ::= <Return>");
        return true;
    }
	else if(Write())
    {
		logger.Log("<Statement> ::= <Write>");
        return true;
    }
	else if(Read())
    {
		logger.Log("<Statement> ::= <Read>");
        return true;
    }
	else if(While())
    {
		logger.Log("<Statement> ::= <While>");
        return true;
    }
    else
    {
        return false;
    }
}
/* <Compound> ::= { <Statement List> } */
bool SyntaxAnalyzer::Compound()
{ 
	if(rrr == s_openbracketcurly)
	{
        NextRecord();
        StatementList();
        if(rrr == s_closebracketcurly)
        {
            NextRecord();
			logger.Log("<Compound> ::= { <Statement List> }");
			return true;
        }
        else
        {
			insert(s_closebracketcurly);
            return false;
        }
	}
    else
    {
        // no error
	    return false;
    }
}
/* <Assign> ::= <Identifier> := <Expression> ; */
bool SyntaxAnalyzer::Assign()
{
	if(rrr == Identifier)
	{
        NextRecord();
        if(rrr == o_equal)
        {
            NextRecord();
            Expression();
            if(rrr == s_semicolon)
            {
                NextRecord();
                logger.Log("Assign ::= <Identifier> := <Expression>");
                return true;
            }
            else
            {
                insert(s_semicolon);
                return true;
            }               
        }
        else
        {
            insert(o_equal);
            return true;
        }
	}
    return false;
}
/* <If> ::= if ( <Condition> ) <Statement> <If Prime> */
bool SyntaxAnalyzer::If()
{
    if(rrr == k_if)
    {
        NextRecord();
        if(rrr == s_openbracketround)
        {
            NextRecord();
            Condition();
            if(rrr == s_closebracketround)
            {
                NextRecord();
				Statement();
                IfPrime();
                logger.Log("<If> ::= if ( <Condition> ) <Statement> <If Prime>");
                return true;
            }
            else
            {
                insert(s_closebracketround);
                return true;
            }
        }
        else
        {
            insert(s_openbracketround);
            return true;
        }
    }
    else
    {
        // no error
        return false;
    }
}
/* <If Prime> ::= endif | else <Statement> endif */
void SyntaxAnalyzer::IfPrime()
{
    if(rrr == k_endif)
    {
        NextRecord();
        logger.Log("<If Prime> ::= endif>");
    }
    else if(rrr == k_else)
    {
        NextRecord();
        Statement();
        if(rrr == k_endif)
        {
            NextRecord();
			logger.Log("<If Prime> ::= endif | else <Statement> endif");
        }
        else
        {
            insert(k_endif);
        }
    }
    else
    {
        // assume they forgot an else
        insert(k_else);
        if(Statement())
        {
            if(rrr == k_endif)
            {
                NextRecord();
			    logger.Log("<If Prime> ::= endif | else <Statement> endif");
            }
            else
            {
                insert(k_endif);
            }
        }
        else
        {
            insert(k_endif);
        }
    }
}
/* <Return> ::=  return <Return Prime> */
bool SyntaxAnalyzer::Return()
{
    if(rrr == k_return)
    {		
        NextRecord();
		ReturnPrime();
        logger.Log("<Return> ::=  return <Return Prime>");
        return true;
    }
    return false;
}
/* <Return Prime> ::= <Expression> ; | ; epsilon */
void SyntaxAnalyzer::ReturnPrime()
{
    if(rrr == s_semicolon)
    {
        NextRecord();
        logger.Log("<Return Prime> ::= ; epsilon");
    }
    else
    {
		Expression();
        if(rrr == s_semicolon)
        {
            NextRecord();
            // log
            logger.Log("<Return Prime> ::= <Expression> ;");
        }
        else
        {
            insert(s_semicolon);
        }
    }
}
/* <Write> ::= print ( <Expression> ); */
bool SyntaxAnalyzer::Write()
{
    if(rrr == k_print)
    {
        NextRecord();
        if(rrr == s_openbracketround)
        {
            NextRecord();
            Expression();
            if(rrr == s_closebracketround)
            {
                NextRecord();
                if(rrr == s_semicolon)
                {
                    NextRecord();
					logger.Log("<Write> ::= print ( <Expression> );");
                    
                    return true;
                }
                else
                {
                    insert(s_semicolon);
                    return true;
                }
            }
            else
            {
                insert(s_closebracketround);
                return true;
            }
        }
        else
        {
            insert(s_openbracketround);
            return true;
        }
    }
    else
    {
        // no error
		return false;
    }
}
/* <Read> ::= scan ( <IDs> ); */
bool SyntaxAnalyzer::Read()
{
    if(rrr == k_scan)
    { 
        NextRecord();
        if(rrr == s_openbracketround)
        {
            NextRecord();
            IDs();
            
            if(rrr == s_closebracketround)
            {
                NextRecord();
                if(rrr == s_semicolon)
                {
                    NextRecord();
					logger.Log("<Read> ::= scan ( <IDs> );");
                    return true;
                }
                else
                {
                    insert(s_semicolon);
                    return true;
                }
            }
            else
            {
                insert(s_closebracketround);
                return true;
            }
        }
        else
        {
            insert(s_openbracketround);
            return true;
        }
    }
    else
    {
        // no error
        return false;
    }
}

/* <While> ::= while ( <Condition> ) <Statement> */
bool SyntaxAnalyzer::While()
{  
    if(rrr == k_while)
    {
        NextRecord();
        if(rrr == s_openbracketround)
        {
            NextRecord();
            Condition();
            if(rrr == s_closebracketround)
            {
				NextRecord();
                Statement();
                // log
				logger.Log("While> ::= while ( <Condition> ) <Statement>");
                return true;
            }
            else
            {
                insert(s_closebracketround);
                return true;
            }
        }
        else
        {
            insert(s_openbracketround);
            return true;
        }
    }
    else
    {
        // no error
        return false;
    } 
}

/* <Condition> ::= <Expression> <Relop> <Expression> */
void SyntaxAnalyzer::Condition()
{
    Expression();
    Relop();
    Expression();
    logger.Log("<Condition> ::= <Expression> <Relop> <Expression>");
}

/* <Relop> ::= = | /= | > | < | => | <= */
void SyntaxAnalyzer::Relop()
{
    if(rrr == o_equalequal)
    {
        NextRecord();
		logger.Log("<Relop> ::= =");
    }
    else if(rrr == o_notequal)
    {
        NextRecord();
		logger.Log("<Relop> ::= /=");
    }
    else if(rrr == o_greatorthan)
    {
        NextRecord();
		logger.Log("<Relop> ::= >");
	}
	else if(rrr == o_lessthan)
	{
        NextRecord();
		logger.Log("<Relop> ::= <");
	}
    else if(rrr == o_greatororequal)
    {
        NextRecord();
		logger.Log("<Relop> ::= =>");
    }
    else if(rrr == o_lessorequal)
    {
        NextRecord();
		logger.Log("<Relop> ::= =<");
    }
    else
    {
		insert(o_equalequal);		
	}
}
/* <Expression> ::= <Term> <Expression Prime> */
void SyntaxAnalyzer::Expression()
{
    Term();
    ExpressionPrime();
    logger.Log("<Expression ::= <Term><Expression Prime>");
}
/* <Expression Prime> ::= + <Term> <Expression Prime> | - <Term> <Expression Prime> | epsilon */
void SyntaxAnalyzer::ExpressionPrime()
{
    
    if(rrr == o_plus)
    {
        NextRecord();
        Term();
        ExpressionPrime();
		logger.Log("<Expression Prime> ::= + <Term> <Expression Prime>");
    }
    else if(rrr == o_minus)
    {	
        NextRecord();
		Term();
        ExpressionPrime();
        logger.Log("<Expression Prime> ::= + <Term> <Expression Prime>");        
    }
    else// if(epsilon())
    {
		logger.Log(" <Expression Prime> ::= epsilon");
    }
}
/* <Term> ::= <Factor> <Term Prime> */
void SyntaxAnalyzer::Term()
{
	Factor();
	TermPrime();
    logger.Log("<Term> ::= <Factor><Term Prime>");
}
/* <Term Prime> ::= * <Factor> <Term Prime> | / <Factor> <Term Prime> | epsilon */
void SyntaxAnalyzer::TermPrime()
{
    
    if(rrr == o_multiply)
    {
        NextRecord();
		logger.Log("<Term Prime> ::= * <Factor> <Term Prime>");
		Factor();
	    TermPrime();
    }
    else if(rrr == o_divide)
    {
        NextRecord();
		logger.Log("<Term Prime> ::= / <Factor> <Term Prime>");
        Factor();
        TermPrime();
    }
    else
    {
		logger.Log("<Term Prime> ::= epsilon");
    }
}
/* <Factor> ::= - <Primary> | <Primary> */
void SyntaxAnalyzer::Factor()
{ 
    if(rrr == o_minus)
    {	
        NextRecord();
	    Primary();
        logger.Log("<Factor> ::= - <Primary>");
    }
	else
    {		
        Primary();
        logger.Log("<Factor> ::= <Primary>");
    }
}
/* <Primary> ::= <Identifier> <PrimaryPrime> | <Integer> | ( <Expression> ) | <Real> | true | false */
void SyntaxAnalyzer::Primary()
{
    
    if(rrr == Identifier)
    {   
        NextRecord();
		PrimaryPrime();
        logger.Log("<Primary> ::= <Identifier><PrimaryPrime> ");
    }
    else if(rrr == Integer)
    {
        NextRecord();
		logger.Log("<Primary> ::= <Integer>");
    }
    else if(rrr == s_openbracketround)
    {
        NextRecord();
        Expression();
        if(rrr == s_closebracketround)
        {
            NextRecord();
			logger.Log("<Primary> := ( <Expression> )");
        }
        else
        {
            insert(s_openbracketround);
        }
    }
    else if(rrr == Real)
    {
        NextRecord();
		logger.Log("<Primary> ::= <Real> ");
    }
    else if(rrr == k_true)
    {
        NextRecord();
		logger.Log("<Primary> ::= <true> ");
    }
    else if(rrr == k_false)
    {
        NextRecord();
		logger.Log("<Primary> ::= <false> ");
    }   
    else
    {
        insert(Identifier, "MISSING_PRIMARY");
    }
}

/* <PrimaryPrime> ::= [<IDs>] | epsilon */
void SyntaxAnalyzer::PrimaryPrime()
{
    if(rrr == s_openbracketsquare)
    {
        NextRecord();
        IDs();
        if(rrr == s_closebracketsquare)
        {
            NextRecord();
            // log
            logger.Log("<PrimaryPrime> ::= [<IDs>]");
        }
        else
        {
            insert(s_closebracketsquare);
        }
    }
    else
    {       
		logger.Log("<PrimaryPrime> ::= epsilon");
    }
}