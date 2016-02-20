#include "stdafx.h"
#include "Emulator.h"
#include "Errors.h"
#include <iostream>
using namespace std;
/*
NAME

InsertMemory- adds an opcode and an address to a specified location in memory.

SYNOPSIS

bool InsertMemory(int a_location, int a_contents);

DESCRIPTION

This function will iterate through the source file and generate a translation.

RETURNS

False if we have exceeded past maximum memory size, True otherwise
*/
bool Emulator::insertMemory(int a_location, int a_contents) {
	if (a_location >= MEMSZ) { // If we have exceeded to maximum memory size then we cannot add the location to memory.
		return false;
	}
	// Otherwise add the opcode and the address at the specified location in memory
	m_memory[a_location] = a_contents;
	return true;
}
/*
NAME

runProgram - Runs the program that came from the translation.

SYNOPSIS

bool runProgram();

DESCRIPTION

This function will run the program that came from the translation.

RETURNS

False if we have exceeded past maximum memory size, True if emulation goes well
*/
bool Emulator::runProgram() {
	int loc = 100; // First location in memory
	bool done = false; // While this is false run the program
	m_acc = 0;
	while (!done) {
		if (loc >= 10000) { // If we have exceeded past memory size report error and terminate!
			string msg = "Location exceeded past memory size, emulator terminated.";
			Errors::RecordError(msg);
			Errors::DisplayErrors();
			return false;
		}
		int inst = m_memory[loc]; // location of instruction
		int opcode = inst / 10000; // opcode to process
		int address = inst % 10000; // address of instruction
		switch (opcode) {
		case 1: // ADD: Add the value to the accumulator
			m_acc += m_memory[address];
			loc++;
			break;
		case 2: // SUBTRACT: Subtract from the accumulator
			m_acc -= m_memory[address];
			loc++;
			break;
		case 3: // MULT: Multiply the values together
			m_acc *= m_memory[address];
			loc++;
			break;
		case 4: // DIV: Divide the values together. If dividing by zero report error and continue.
			m_acc /= m_memory[address];
			if (m_memory[address] == 0) {
				string msg = "Cannot divide by zero";
				Errors::RecordError(msg);
			}
			loc++;
			break;
		case 5: // LOAD: Set the accumulator equal to the value in memory
			m_acc = m_memory[address];
			loc++;
			break;
		case 6: // STORE: Store accumulator contents in address
			address = m_acc;
			loc++;
			break;
		case 7: // READ: Ask the user to enter a number which is placed in memory
			cout << "?";
			cin >> m_memory[address];
			loc++;
			break;
		case 8: // WRITE: Display the number to the screen
			cout << m_acc << endl;
			loc++;
			break;
		case 9: // B: Loop back to given label
			loc = address;
			break;
		case 10: // BM: Loop back if accumulator has a negative value
			if (m_acc < 0) loc = address;
			else loc++;
			break;
		case 11: // BZ: Loop back if the accumulator has the value of zero
			if (m_acc == 0) loc = address;
			else loc++;
			break;
		case 12: // BP: Loop back if the accumulator has a positive value
			if (m_acc > 0) loc = address;
			else loc++;
			break;
		case 13: // HALT: Terminate execution
			done = true;
			break;
		default: // Must handle the default case. In this case it will be to display an error message from emulation.
			string msg = "Error reported during emulation";
			Errors::RecordError(msg);
			break;
		}
	}
	// Display any collected error messages during emulation
	Errors::DisplayErrors();
	return true;
}
