#include "InstructionTable.h"
#include <fstream>

InstructionTable::InstructionTable()
{
	instructionAddress = 0;
	instructions = new Instruction[300];
}

void InstructionTable::BackPatch(int jumpAddress)
{
	int address = jumpStack.top();
	jumpStack.pop();
	instructions[address].Operand = jumpAddress;
}

int InstructionTable::GenerateInstruction(const Instruction_t & instruction, int symbolAddress)
{
	instructions[instructionAddress].Operand = symbolAddress;
	instructions[instructionAddress].InstructionType = instruction;
	instructions[instructionAddress].Address = instructionAddress;
	instructionAddress++;
	return instructionAddress - 1;
}

void InstructionTable::PushJumpStack(int instructionAddress)
{
	jumpStack.push(instructionAddress);
}

void InstructionTable::PushJumpStack()
{
	jumpStack.push(instructionAddress - 1);
}

const int InstructionTable::CurrentAddress() const
{
	return this->instructionAddress;
}

void InstructionTable::GenerateInstruction(const Instruction_t &  instruction)
{
	int i = GenerateInstruction(instruction, -999);
}
void InstructionTable::Print(const string& fileName)
{
    ofstream outFile;
    outFile.open(fileName, ofstream::out);
    if(outFile.is_open())
		while( !instructions )
    {
	 outFile<< instructions;
	instructions++;
    }
    outFile.close();
}
/*
    ofstream outFile;
    outFile.open(fileName, ofstream::out);
    if(outFile.is_open())
    while(!results.empty())
    {
        string result = results.front();
        results.pop();
        outFile << result << endl;
    }
    outFile.close();
*/