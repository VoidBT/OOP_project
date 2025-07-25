#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

//Done by Ryan Ang Rui Heng 2400522

using namespace std;

class FilePrinter {
public:
	// Function to print the contents of a file line by line
    static void printFile(const string& filename);
	// Function to print the contents of a file with formatted columns
    static void printFileFormatted(const string& filename,
        const string& delimiter = " | ",
        int columnWidth = 15);
	// Function to print the contents of a file with headers
    static void printFileWithHeaders(const string& filename,
        const vector<string>& headers,
        const string& delimiter = " | ",
        int columnWidth = 15);

private:
	// Helper function to split a line into tokens based on a delimiter
    static vector<string> splitLine(const string& line, char delimiter = ','); // Changed default delimiter to comma
};