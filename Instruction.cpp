#include "stdafx.h"
#include "Instruction.h"
#include <sstream>
#include <iterator>
/*
NAME

ParseInstruction - Parses the current line in the file and gets the instruction type.

SYNOPSIS

Instruction::InstructionType ParseInstruction(string &a_buff);

DESCRIPTION

This function will determine the type of instruction in "a_buff".

RETURNS

The type of instruction
*/
Instruction::InstructionType Instruction::ParseInstruction(string & a_buff) {
	// Machine Language
	string inst_ops[]  { "ADD", "add", "SUB", "sub", "MULT", "mult", "DIV", "div", "LOAD", "load", "STORE", "store", "READ", "read", "WRITE", "write", "B", "b", "BM", "bm", "BZ", "bz", "BP", "bp", "HALT", "halt"};
	// Assembler Instruction
	string assem_ops[]  { "DC", "dc", "DS", "ds", "ORG", "org" };
	// End
	string end_ops[] { "END", "end", "eND", "End", "EnD", "ENd", "eNd", "enD"};
	// Halt
	string halt_ops[]{ "HALT, halt", "Halt", "HALt", "HAlT", "HaLt", "hALT", "hALt", "HAlt", "HalT", "HaLT", "hAlT", "halT", "haLt", "haLT", "hAlt"};
	m_instruction = a_buff; // Save original instruction for printing
	if (m_instruction.at(0) == ';' || m_instruction.empty()) { // If we have a ; at the beginning of the line or the line is blank then this must be a comment
		return ST_Comment;
	}
	int j = 0;
	while (m_instruction[j] == ' ' && j != m_instruction.length()) { // Remove all trailing spaces
		j++;
	}
	if (find(begin(end_ops), end(end_ops), m_instruction.substr(j, 3)) != end(end_ops)) { // End instruction
		return ST_End;
	}
	if (find(begin(halt_ops), end(halt_ops), m_instruction.substr(j, 4)) != end(halt_ops)) { // Halt instruction
		return ST_MachineLanguage;
	}
	int is = a_buff.find(";"); // Remove all characters past and including ;
	if (is != string::npos) {
		a_buff.erase(is);
	}
	for (int i = 0; i < a_buff.length(); i++) { // Convert to lowercase because instructions are not case sensitive
		if (!isspace(a_buff.at(i)) && !ispunct(a_buff.at(i))) {
			a_buff.at(i) = tolower(a_buff.at(i));
		}
	}
	// Use istringstream to parse instruction into label, opcode, and operand
	istringstream input(a_buff);
	string tmp1, tmp2, tmp3, tmp4;
	input >> tmp1 >> tmp2 >> tmp3 >> tmp4;
	if (a_buff[0] == ' ') { // If the beginning has a space then there is no label
		m_Label = "";
		m_OpCode = tmp1;
		m_Operand = tmp2;
		// If the end is not empty or some parts on the beginning are empty then this is an illegal instruction. Report error during pass II
		if (!tmp3.empty()) {
			return ST_IllegalInstruction;
		}
	}
	else { // Otherwise we have a label
		m_Label = tmp1;
		m_OpCode = tmp2;
		m_Operand = tmp3;
		// Same as before, must check for missing or extra parts of instruction
		if (!tmp4.empty()) {
			return ST_IllegalInstruction;
		}
	}
	if (find(begin(assem_ops), end(assem_ops), m_OpCode) != end(assem_ops)) { // Assembler instruction
		m_type = ST_AssemblerInstr;
	}

	else if (find(begin(inst_ops), end(inst_ops), m_OpCode) != end(inst_ops)){ // Machine Language Instruction
		m_type = ST_MachineLanguage;
	}
	else { // If it is not any type then it must be illegal
		m_type = ST_IllegalInstruction;
	}
	return m_type; // Return the type of instruction
}
/*
NAME

LocationNextInstruction - computes the location of the next instruction given the current one.

SYNOPSIS

int LocationNextInstruction(int a_loc);

DESCRIPTION

This function will compute the location of the next instruction given the current.

RETURNS

loc + 1 unless the operand is a DS or an ORG in which case the location is added on to the previous one
*/
int Instruction::LocationNextInstruction(int a_loc) {
	// If this is a DS or an ORG add the operand onto the location. Must convert string to integer
	if (m_OpCode == "DS" || m_OpCode == "ds") { // Each time, return the base case, loc + 1 if the operand is not numeric
		for (int i = 0; i < m_Operand.length(); i++) {
			if (!isdigit(m_Operand.at(i))) return a_loc + 1;
		}
		return a_loc + stoi(m_Operand);
	}
	if (m_OpCode == "ORG" || m_OpCode == "org") {
		for (int i = 0; i < m_Operand.length(); i++) {
			if (!isdigit(m_Operand.at(i))) return a_loc + 1;
		}
		return a_loc + stoi(m_Operand);
	}
	if (m_OpCode == "DC" || m_OpCode == "dc") {
		for (int i = 0; i < m_Operand.length(); i++) {
			if (!isdigit(m_Operand.at(i))) return a_loc;
		}
	}
	// Otherwise add 1 to the location
	return a_loc + 1;
}
/*
NAME

computeNumOpCode - determines the number associated with a particular instruction.

SYNOPSIS

void PassII();

DESCRIPTION

This function will compute the number between 0 and 13 associated with the instruction.

RETURNS

A number between 0 and 13 based on the opcode
*/
int Instruction::computeNumOpCode() {
	if (m_OpCode == "ADD" || m_OpCode == "add") {
		m_NumOpCode = 1; // ADD
	}
	else if (m_OpCode == "SUB" || m_OpCode == "sub") {
		m_NumOpCode = 2; // SUBTRACT
	}
	else if (m_OpCode == "MULT" || m_OpCode == "mult") {
		m_NumOpCode = 3; // MULT
	}
	else if (m_OpCode == "DIV" || m_OpCode == "div") {
		m_NumOpCode = 4; // DIV
	}
	else if (m_OpCode == "LOAD" || m_OpCode == "load") {
		m_NumOpCode = 5; // LOAD
	}
	else if (m_OpCode == "STORE" || m_OpCode == "store") {
		m_NumOpCode = 6; // STORE
	}
	else if (m_OpCode == "READ" || m_OpCode == "read") {
		m_NumOpCode = 7; // READ
	}
	else if (m_OpCode == "WRITE" || m_OpCode == "write") {
		m_NumOpCode = 8; // WRITE
	}
	else if (m_OpCode == "B" || m_OpCode == "b") {
		m_NumOpCode = 9; // B
	}
	else if (m_OpCode == "BM" || m_OpCode == "bm") {
		m_NumOpCode = 10; // BM
	}
	else if (m_OpCode == "BZ" || m_OpCode == "bz") {
		m_NumOpCode = 11; // BZ
	}
	else if (m_OpCode == "BP" || m_OpCode == "bp") {
		m_NumOpCode = 12; // BP
	}
	else if (m_OpCode == "HALT" || m_OpCode == "halt") {
		m_NumOpCode = 13; // HALT
	}
	else {
		m_NumOpCode = 0; // This means it has to be an assembler instruction
	}
	return m_NumOpCode;
}