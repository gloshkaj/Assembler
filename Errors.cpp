#include "stdafx.h"
#include "Errors.h"
#include <string>
#include <iostream>
using namespace std;
vector<string> Errors::m_ErrorMsgs; // Must declare the static variable again here or the compiler will report error that it cannot find the method call
/*NAME

InitErrorReporting - initialize error reports.

SYNOPSIS

void InitErrorReporting();

DESCRIPTION

This function will initialize all error reports.
*/
void Errors::InitErrorReporting() {
	// For each value in the vector initialize it as an empty string
	for (int i = 0; i < m_ErrorMsgs.size(); i++) {
		m_ErrorMsgs.at(i) = "";
	}
}
/*
NAME

RecordError - adds a collected error message to a list of errors.

SYNOPSIS

void RecordError(string &a_emsg);

DESCRIPTION

This function will record the collected error message.
*/
void Errors::RecordError(string &a_emsg) {
	m_ErrorMsgs.push_back(a_emsg); // Adds the collected message to the vector
}

// Displays the collected error message.
/*
NAME

DisplayErrors - display the list of collected errors.

SYNOPSIS

void DisplayErrors();

DESCRIPTION

This function will display each error collected from the translation of the assembler language program.
*/
void Errors::DisplayErrors() {
	cout << "Errors collected: ";
	if (m_ErrorMsgs.size() == 0) { // If there is nothing in the vector tell the user there are no errors and get out of the function
		cout << "None" << endl;
		return;
	}
	// Otherwise iterate through the vector and print out each error message to the screen.
	cout << endl;
	for (int i = 0; i < m_ErrorMsgs.size(); i++) {
		cout << "Error #" << i + 1 << ": " << m_ErrorMsgs.at(i) << endl;
	}
}