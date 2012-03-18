#ifndef _SYMBOLTABLE_H_
#define _SYMBOLTABLE_H_
#include "record.h"
#include <vector>
class SymbolTable;
class Symbol
{
private:
	int address;
	Record record;

public:
	Symbol(int address, Record record);
	Symbol();
	friend SymbolTable;
};

class SymbolTable
{
private:
	Symbol* table;
	int memoryAddress;
	int location;
public:
	SymbolTable();
	int GetAddress(const Record & record) const;
	void Insert(const Symbol &);
	void Insert(const Record &);
	void Print(const string& fileName);
};
#endif // _SYMBOLTABLE_H_