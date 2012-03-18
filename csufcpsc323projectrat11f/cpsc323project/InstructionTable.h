#ifndef _INSTRUCTIONTABLE_H_
#define _INSTRUCTIONTABLE_H_
#include <stack>
using namespace std;
#define blank -999
enum Instruction_t
{
	i_pushi,	// {Integer Value} Pushes the {Integer Value} onto the Top of the Stack (TOS)
	i_pushm,	// {ML - Memory Location} Pushes the value stored at {ML} onto TOS
	i_popm,		// {ML} Pops the value from the top of the stack and stores it at {ML}
	i_stdout,	// Pops the value from TOS and outputs it to the standard output
	i_stdin,	// Get the value from the standard input and place in onto the TOS
	i_add,		// Pop the first two items from stack and push the sum onto the TOS
	i_sub,		// Pop the first two items from stack and push the difference onto the TOS ( Second item - First item)
	i_mul,		// Pop the first two items from stack and push the product onto the TOS
	i_div,		// Pop the first two items from stack and push the result onto the TOS ( Second item /  First item and ignore the remainder)
	i_grt,		//  Pops two items from the stack and pushes 1 onto TOS if second item is larger otherwise push 0
	i_les,		// Pops two items from the stack and pushes 1 onto TOS if the second item is smaller than first item otherwise push 0
	i_equ,		// Pops two items from the stack and pushes 1 onto TOS if they are equal otherwise push 0
	i_jumpz,	// {IL - Instruction Location} Pop the stack and  if the value is 0 then jump to {IL}
	i_jump,		// {IL} Unconditionally jump to {IL}
	i_label,	// Empty Instruction; Provides the instruction location to jump to
	i_neq,
	i_leq,
	i_geq,
	i_length
};

struct Instruction
{
	Instruction_t InstructionType;
	int Operand;
	int Address;
};

class InstructionTable
{
private:
	Instruction* instructions;
	int instructionAddress;
	stack<int> jumpStack;

public:
	InstructionTable();
	int GenerateInstruction(const Instruction_t &, int symbolAddress);
	void GenerateInstruction(const Instruction_t &);
	void BackPatch(int jumpAddress);
	void PushJumpStack(int instructionAddress);
	// Pushes the last instruction address to the jump stack
	void PushJumpStack(/* The last instruction address Generated */);
	const int CurrentAddress() const;
	void Print(const string& fileName);
};

#endif // _INSTRUCTIONTABLE_H_