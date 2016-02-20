//
// Class to parse and provide information about instructions
//
#pragma once
#include <string>
#include <iostream>
using namespace std;

// The elements of an instruction.
class Instruction {

public:

	Instruction() {
		m_NumOpCode = 0;
		m_OpCode = "";
		m_IsNumericOperand = true;
		m_Label = "";
		m_OperandValue = 0;
		m_instruction = "";
		m_Operand = "";
	};
	~Instruction() { };

	// Code to indicate the type of instruction we are processing.
	// IllegialInstruction if illegal
	// Why is this inside the class?
	enum InstructionType {
		ST_End, // End instruction.
		ST_AssemblerInstr,  // Assembler Language instruction.
		ST_Comment,          // Comment or blank line
		ST_MachineLanguage,  // machine language instruction.
		ST_IllegalInstruction, // Handles invalid instructions.
	};
	// Parse the Instruction.
	InstructionType ParseInstruction(string &a_buff);

	// Compute the location of the next instruction.
	int LocationNextInstruction(int a_loc);

	int computeNumOpCode();
	// To access the label
	inline string &GetLabel() {

		return m_Label;
	};
	// Access the OpCode
	inline string &GetOpCode() {

		return m_OpCode;
	};
	// Access the symbolic OpCode
	inline int &GetNumOpCode() {

		m_NumOpCode = computeNumOpCode();
		return m_NumOpCode;
	};
	// Access the original instruction
	inline string &GetOrgInst() {

		return m_instruction;
	};
	// Access the operand
	inline string &GetOperand() {

		return m_Operand;
	};
	// To determine if a label is blank.
	inline bool isLabel() {

		return !m_Label.empty();
	};


private:


	// The elemements of a instruction
	string m_Label;            // The label.
	string m_OpCode;       // The symbolic op code.
	string m_Operand;      // The operand.
	string m_instruction;    // The original instruction.
	// Derived values.
	int m_NumOpCode;     // The numerical value of the op code.
	InstructionType m_type; // The type of instruction.
	bool m_IsNumericOperand;// == true if the operand is numeric.
	int m_OperandValue;   // The value of the operand if it is numeric.
};

