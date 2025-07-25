#include "FilePrinter.h"

//Done by Ryan Ang Rui Heng 2400522

vector<string> FilePrinter::splitLine(const string& line, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(line);

    while (getline(tokenStream, token, delimiter)) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    return tokens;
}

// Function definitions for FilePrinter class
void FilePrinter::printFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        cout << line << endl;
    }
    file.close();
}

//Function to print the contents of a file with formatted columns
void FilePrinter::printFileFormatted(const string& filename,
    const string& delimiter,
    int columnWidth) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        vector<string> tokens = splitLine(line, ','); // Explicitly pass comma for CSV
        for (const auto& token : tokens) {
            cout << left << setw(columnWidth) << token << delimiter;
        }
        cout << endl;
    }
    file.close();
}

// Function to print the contents of a file with headers
void FilePrinter::printFileWithHeaders(const string& filename,
    const vector<string>& headers,
    const string& delimiter,
    int columnWidth) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return;
    }

    // Print headers
    for (const auto& header : headers) {
        cout << left << setw(columnWidth) << header << delimiter;
    }
    cout << endl;

    // Print separator line
    cout << string(headers.size() * (columnWidth + delimiter.length()), '-') << endl;

    // Print file contents
    string line;
    while (getline(file, line)) {
        vector<string> tokens = splitLine(line, ','); // Explicitly pass comma for CSV
        for (const auto& token : tokens) {
            cout << left << setw(columnWidth) << token << delimiter;
        }
        cout << endl;
    }
    file.close();
}