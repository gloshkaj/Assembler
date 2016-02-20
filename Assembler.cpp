//
//      Implementation of the Assembler class.
//
#include "stdafx.h"
#include "Assembler.h"
#include "Errors.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <iterator>
using namespace std;
// Constructor for the assembler.  Note: we are passing argc and argv to the file access constructor.
Assem::Assem(int argc, char *argv[])

	: m_facc(argc, argv)
	// Must put statement above braces or there will be an error.
{
// Nothing else to do here.
}

/*
NAME

PassI - establishes label positions.

SYNOPSIS

void PassI();

DESCRIPTION

This function will iterate through the source file and add symbols and establish label positions
*/
void Assem::PassI()
{
	int loc = 0;        // Tracks the location of the instructions to be generated.
						// Successively process each line of source code.
	for (; ; ) {

		// Read the next line from the source file.
		string buff;
		if (!m_facc.GetNextLine(buff)) {

			// If there are no more lines, we are missing an end statement.
			// We will let this error be reported by Pass II.
			return;
		}
		// Parse the line and get the instruction type.
		Instruction::InstructionType st = m_inst.ParseInstruction(buff);
		// If this is an end statement, there is nothing left to do in pass I.
		// Pass II will determine if the end is the last statement.
		if (st == Instruction::ST_End) return;

		// Labels can only be on machine language and assembler language
		// instructions.
		if (st != Instruction::ST_MachineLanguage && st != Instruction::ST_AssemblerInstr) continue;

		// If the instruction has a label, record it and its location in the
		// symbol table.
		if (m_inst.isLabel()) {

			m_symtab.AddSymbol(m_inst.GetLabel(), loc);
		}
		// Compute the location of the next instruction.
		loc = m_inst.LocationNextInstruction(loc);
	}
}
/*
NAME

PassII - outputs a translation and symbol table.

SYNOPSIS

void PassII();

DESCRIPTION

This function will iterate through the source file and generate a translation.
*/
void Assem::PassII() {
	string msg;
	m_facc.rewind(); // Go back to beginning of file
	Errors::InitErrorReporting();
	int loc = 0; // Tracks location of instructions to be generated
	cout << "Location    " << "Contents     " << "Original Statement" << endl;
	string newBuff = "";
	// Successively process each line of source code.
	for (; ; ) {
		// Read the next line from the source file.
		string buff; // instruction
		if (!m_facc.GetNextLine(buff)) {
			msg = "No End Statement Exists ";
			// If there are no more lines, report error that an end statement is missing.
			Errors::RecordError(msg);
			Errors::DisplayErrors();
			return;
		}
		// Parse the line and get the instruction type.
		Instruction::InstructionType st = m_inst.ParseInstruction(buff);
		// If this is an end statement, break out of the loop and check if there are more lines after the statement
		if (st == Instruction::ST_End) {
			cout << setw(35) << right << buff << endl;
			newBuff = buff;
			break;
		}
		// If this is a comment, just print it out and continue
		if (st == Instruction::ST_Comment){
			cout << "                         " << buff << endl;
			continue;
		}
		// If this is an invalid instruction report error next to the offending line.
		if (st == Instruction::ST_IllegalInstruction) {
			cout << "?????? " << buff << ": Invalid instruction";
			Errors::RecordError(buff + ": Invalid Instruction");
			cout << endl;
			continue;
		}
		cout << loc << "         ";
		string m_Oprd = m_inst.GetOperand(); // Operand
		string m_Op = m_inst.GetOpCode(); // Opcode
		int m_NumOp = m_inst.GetNumOpCode(); // NumOpCode
		// If this is a machine language instruction then report an error if the operand is numeric.
		if (st == Instruction::ST_MachineLanguage) {
			if (m_NumOp < 10) cout << "0";
			cout << m_NumOp;
			// Validate Label. Report error if it has digits
			for (int i = 0; i < m_inst.GetLabel().length(); i++) {
				if (isdigit(m_inst.GetLabel().at(i))) {
					cout << "??????" << buff << " Label not appropriate";
					msg = "Label in incorrect format";
					Errors::RecordError(msg);
					continue;
				}
			}
			for (int i = 0; i < m_Oprd.length(); i++) {
				if (isdigit(m_Oprd.at(i))) {
					cout << "????";
					msg = "Invalid Operand Choice";
					Errors::RecordError(msg);
				}
			}
			int value = m_symtab.GetValueForKey(m_Oprd);
			int symVal = m_symtab.GetValueForKey(m_inst.GetLabel());
			if (symVal == -999) { // If symbol already exists report error for duplicate symbol
				msg = "Multiply defined symbol";
				cout << "????" << msg;
				Errors::RecordError(msg);
			}
			else if (value < 1000) cout << "0";
			if (value == 0 && m_Op != "HALT" && m_Op != "halt") {
				cout << "???? + Undefined Symbol: ";
				msg = "Undefined Symbol";
				Errors::RecordError(msg);
			}
			else if (m_Op == "HALT" || m_Op == "halt") cout << "000";
			else cout << value;
			cout << "       " << setw(15) << right << m_inst.GetOrgInst();
			for (int i = 0; i < m_Oprd.length(); i++) {
				if (isdigit(m_Oprd.at(i))) {
					msg = "Operand must be symbolic";
					cout << msg;
					Errors::RecordError(msg);
				}
			}
			// If this is not a halt and either the operand or opcode are empty then report error that the instruction has missing parts
			if (m_Op != "HALT" && m_Op != "halt" && (m_Oprd.empty() || m_inst.GetOpCode().empty())) {
				msg = "Invalid instruction";
				cout << msg;
				Errors::RecordError(msg);
			}
			// Determine location of instruction in memory
			int contents;
			if (m_Op == "HALT" || m_Op == "halt") contents = stoi(to_string(m_NumOp) + "0000");
			else if (m_symtab.GetValueForKey(m_Oprd) < 1000) {
				contents = stoi(to_string(m_NumOp) + "0" + to_string(m_symtab.GetValueForKey(m_Oprd)));
			}
			else contents = stoi(to_string(m_NumOp) + to_string(m_symtab.GetValueForKey(m_Oprd)));
			if (!m_emul.insertMemory(loc, contents)) {
				// If location value exceeds past 10000 then report error that the location does not fit in VC3600 memory
				msg = " Location too large for VC3600 memory!!!!!!!! ";
				cout << msg;
				Errors::RecordError(msg);
			}
		}
		else if (m_Op == "DC" || m_Op == "dc") { // DC operand assumes that everything is an integer. we will not support strings.
			// If we enter a string the program will terminate. The m_NumOpCode is equal to zero
			for (int i = 0; i < m_Oprd.length(); i++) {
				if (!isdigit(m_Oprd.at(i))) {
					m_Oprd = "Undefined";
					break;
				}
			}
			if (m_Oprd == "Undefined" || m_Oprd.empty()) {
				msg = "DC operand must be numeric.";
				cout << "????" << setw(16) << right << m_inst.GetOrgInst() << msg << endl;
				Errors::RecordError(msg);
				continue;
			}
			if (stoi(m_Oprd) < 10) cout << "00000";
			else if (stoi(m_Oprd) < 100) cout << "0000";
			else if (stoi(m_Oprd) < 1000) cout << "000";
			else if (stoi(m_Oprd) < 10000) cout << "00";
			else if (stoi(m_Oprd) < 100000) cout << "0";
			else if (stoi(m_Oprd) > 999999) { // Report error if greater than 999999
				cout << "??????" << buff << " Invalid constant" << endl;
				msg = "Invalid constant";
				Errors::RecordError(msg);
				loc = m_inst.LocationNextInstruction(loc);
				continue;
			}
			cout << m_Oprd << "       " << setw(10) << right << m_inst.GetOrgInst();
			if (!m_emul.insertMemory(loc, stoi(m_Oprd))) {
				msg = " Location too large for VC3600 memory!!!!!!!! ";
				cout << msg;
				Errors::RecordError(msg);
			}
		}
		else if (m_Op == "ORG" || m_Op == "org") {
			cout << "             " << setw(15) << right << m_inst.GetOrgInst();
			for (int i = 0; i < m_Oprd.length(); i++) {
				if (!isdigit(m_Oprd.at(i))) {
					m_Oprd = "Undefined";
					break;
				}
			}
			if (m_Oprd == "Undefined" || m_Oprd.empty()) {
				msg = "ORG operand must be numeric.";
				cout << msg << endl;
				Errors::RecordError(msg);
				continue;
			}
			if (stoi(m_Oprd) > 9999) { // Report error if greater than 9999
				cout << buff << " Invalid constant" << endl;
				Errors::RecordError(buff + " Invalid constant");
				continue;
			}
		}
		else if (m_Op == "DS" || m_Op == "ds") {
			// Validate operand value
			cout << "             " << setw(15) << right << m_inst.GetOrgInst();
			for (int i = 0; i < m_Oprd.length(); i++) {
				if (!isdigit(m_Oprd.at(i))) {
					m_Oprd = "Undefined";
					break;
				}
			}
			if (m_Oprd == "Undefined" || m_Oprd.empty()) {
				msg = "DS operand must be numeric.";
				cout << msg << endl;
				Errors::RecordError(msg);
				continue;
			}
			if (stoi(m_Oprd) > 9999) { // Report error if greater than 9999
				cout << "Invalid constant" << endl;
				msg = "Invalid constant";
				Errors::RecordError(msg);
				loc = m_inst.LocationNextInstruction(loc);
				continue;
			}
		}
		cout << endl;
		// Compute the location of the next instruction.
		loc = m_inst.LocationNextInstruction(loc);	
	}
	if (m_facc.GetNextLine(newBuff)) {
		msg = "Not supposed to have any lines after end instruction ";
		// If there are more lines after end, report error that there cannot be data after the end of the program.
		Errors::RecordError(msg);
	}
	Errors::DisplayErrors();
}

/*
NAME

RunInEmulator - runs the program by loading up the accumulator.

SYNOPSIS

void RunInEmulator();

DESCRIPTION

This function will run the source code within the file.
*/
void Assem::RunProgramInEmulator() {
	m_emul.runProgram();
}