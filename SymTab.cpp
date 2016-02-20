//
//		Implementation of the symbol table class.
//
#include "stdafx.h"
#include "SymTab.h"
#include <iomanip>

/*
NAME

AddSymbol - adds a new symbol to the symbol table.

SYNOPSIS

void AddSymbol( const string &a_symbol, int a_loc );

DESCRIPTION

This function will place the symbol "a_symbol" and its location "a_loc"
in the symbol table.
*/
void SymbolTable::AddSymbol(const string &a_symbol, int a_loc)
{
	// If the symbol is already in the symbol table, record it as multiply defined.
	map<string, int>::iterator st;
	st = m_symbolTable.find(a_symbol);
	if (st != m_symbolTable.end()) {

		st->second = multiplyDefinedSymbol;
		return;
	}
	// Record location in the symbol table.
	m_symbolTable[a_symbol] = a_loc;
}
/*
NAME

LookupSymbol - adds a new symbol to the symbol table.

SYNOPSIS

bool LookupSymbol( const string &a_symbol, int & a_loc );

DESCRIPTION

This function will check if "a_symbol" exists in the symbol table at the location "a_loc".

RETURNS

True if symbol is a match, False otherwise
*/
bool SymbolTable::LookupSymbol(const string &a_symbol, int & a_loc) {
	if (m_symbolTable[a_symbol] == a_loc) {
		return true;
	}
	return false;
}
/*
NAME

DisplaySymbolTable - prints out the symbol table.

SYNOPSIS

void DisplaySymbolTable();

DESCRIPTION

This function will display the items within the symbol table.
*/
void SymbolTable::DisplaySymbolTable() {
	map<string, int>::iterator mp;
	cout << "Symbol #     " << "Symbol      " << "Location" << endl;
	int symCount = 0;
	for (mp = m_symbolTable.begin(); mp != m_symbolTable.end(); mp++) {
		cout << symCount << "            " << mp->first << setw(14) << mp->second << endl;
		symCount++;
	}
}
/*
NAME

GetValueForKey - finds the location of a symbol in the symbol table.

SYNOPSIS

int GetValueForKey( const string &a_symbol, int & a_loc );

DESCRIPTION

This function will determine the value that corresponds to the key "a_symbol".

RETURNS

The value, "m_symbolTable[a_symbol]", corresponding to "a_symbol"
*/
int SymbolTable::GetValueForKey(const string &a_symbol) {
	return m_symbolTable[a_symbol]; // Return the value corresponding to the symbol given
}
