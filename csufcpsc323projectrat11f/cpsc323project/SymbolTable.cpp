#include "SymbolTable.h"
#include <string>

Symbol::Symbol(int address, Record record)
{
	this->address = address;
	this->record = record;
}

Symbol::Symbol()
{
}

SymbolTable::SymbolTable()
{
	table = new Symbol[300];
	memoryAddress = 3000;
	location = 0;
}

int SymbolTable::GetAddress(const Record & record) const
{
	// Find the record in symbol table
	for(int i = 0; i < location; i++)
	{
		if(table[i].record == record)
			return table[i].address;
	}
	// return error
	return -1;	
}

void SymbolTable::Insert(const Record & record)
{
	Symbol newSymbol(memoryAddress, record);
	table[location] = newSymbol;
	location++;
	memoryAddress++;
}

void SymbolTable::Print(const string& fileName)
{
    ofstream outFile;
    outFile.open(fileName, ofstream::out);
    if(outFile.is_open())
    while( !table )
    {
		outFile << table;
		table++;
    }
    outFile.close();

}