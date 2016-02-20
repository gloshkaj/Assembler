//
//		Symbol table class.
//
#pragma once
#include <map>
#include <string>
#include <iostream>
using namespace std;


// This class is our symbol table.
class SymbolTable {

public:
	// Get rid of constructor and destructor if you don't need them.
	SymbolTable() {};
	~SymbolTable() {};

	const int multiplyDefinedSymbol = -999;

	// Add a new symbol to the symbol table.
	void AddSymbol(const string &a_symbol, int a_loc);

	// Display the symbol table.
	void DisplaySymbolTable();

	// Lookup a symbol in the symbol table.
	bool LookupSymbol(const string &a_symbol, int &a_loc);

	// Get the corresponding value for a given key. Need to do this to generate the address of instruction.
	int GetValueForKey(const string &a_symbol);

private:

	// This is the actual symbol table.  The symbol is the key to the map.
	map<string, int> m_symbolTable;

};

