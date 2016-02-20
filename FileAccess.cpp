//
//  Implementation of file access class.
//
#include "stdafx.h"
#include "FileAccess.h"
#include <iostream>
using namespace std;

// Don't forget to comment the function headers.
FileAccess::FileAccess(int argc, char *argv[])
{
	// Check that there is exactly one run time parameter.
	if (argc != 2) {
		cerr << "Usage: Assem <FileName>" << endl;
		exit(1);
	}
	// Open the file.  One might question if this is the best place to open the file.
	// One might also question whether we need a file access class.
	m_sfile.open(argv[1], ios::in);

	// If the open failed, report the error and terminate.
	if (!m_sfile) {
		cerr << "Source file could not be opened, assembler terminated."
			<< endl;
		exit(1);
	}
}
FileAccess::~FileAccess() //Destructor just closes the file
{
	m_sfile.close();
}
// Get the next line from the file.
/*
NAME

GetNextLine - get the next line from the file.

SYNOPSIS

bool GetNextLine(string &a_buff);

DESCRIPTION

This function will display the next line of the file.

RETURNS

False if we have reached the end of the file, True otherwise
*/
bool FileAccess::GetNextLine(string &a_buff)
{
	if (m_sfile.eof()) return false; //If we reached the end of the file then we have no more lines to display

	getline(m_sfile, a_buff);

	// Return indicating success.
	return true;
}
/*
NAME

rewind - Go back to the beginning of the file to start Pass II.

SYNOPSIS

void rewind();

DESCRIPTION

This function will jump back to the beginning of the file. Need this for Pass II.
*/
void FileAccess::rewind()
{
	// Clean the end of file flag and go back to the beginning of the file.
	m_sfile.clear();
	m_sfile.seekg(0, ios::beg);
}

