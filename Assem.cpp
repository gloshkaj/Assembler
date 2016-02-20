// Assembler.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include "Assembler.h"
#include "Errors.h"

int main(int argc, char *argv[])
{
	Assem assem(argc, argv);

	// Establish the location of the labels:
	assem.PassI();

	// Display the symbol table.
	assem.DisplaySymbolTable();

	// Output the symbol table and the translation.
	assem.PassII();

	// Run the emulator on the VC3600 program that came from the translation.
	assem.RunProgramInEmulator();

	// Terminate indicating all is well.  If there is an unrecoverable error, the 
	// program will terminate at the point that it occurred.
	return 0;
}
