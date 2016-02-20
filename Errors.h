//
// Class to manage error reporting. Note: all members are static so we can access them anywhere.
// What other choices do we have to accomplish the same thing?
//
#ifndef _ERRORS_H
#define _ERRORS_H
#include "stdafx.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;

class Errors {

public:
	// Initialize ALL error reports
	static void InitErrorReporting();
	// Records an error message.
	static void RecordError(string &a_emsg);
	// Displays the collected error messages.
	static void DisplayErrors();
private:
	// vector to hold list of errors
	static vector<string> m_ErrorMsgs;
};
#endif

