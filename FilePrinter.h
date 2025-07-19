#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>

using namespace std;

class FilePrinter {
public:
    static void printFile(const string& filename);
    static void printFileFormatted(const string& filename,
        const string& delimiter = " | ",
        int columnWidth = 15);
    static void printFileWithHeaders(const string& filename,
        const vector<string>& headers,
        const string& delimiter = " | ",
        int columnWidth = 15);

private:
    static vector<string> splitLine(const string& line, char delimiter = ','); // Changed default delimiter to comma
};