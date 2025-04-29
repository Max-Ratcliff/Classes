/***
* Max Ratcliff
* mwratcli
* 2025 Winter CSE101 PA8
* WordFrequency.cpp
* client of Dictionary ADT 
***/ 
#include "Dictionary.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string>
#include <algorithm>

using namespace std;

int main(int argc, char* argv[]) {
    // Check for the correct number of command-line arguments.
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl;
        return 1;
    }

    // Open the input and output files.
    ifstream in(argv[1]);
    ofstream out(argv[2]);
    if (!in.is_open()) {
        cerr << "Unable to open file " << argv[1] << " for reading" << endl;
        return 1;
    }
    if (!out.is_open()) {
        cerr << "Unable to open file " << argv[2] << " for writing" << endl;
        return 1;
    }

    // Set the delimiter to remove all whitespace, punctuation, digits, and special characters.
    // Only alphabetic characters will remain in each token.
    string delim = " \t\\\"',<.>/?;:[{]}|`~!@#$%^&*()-_=+0123456789";

    Dictionary D;
    string line, key;
    size_t begin, end, len;

    // Read each line from the input file.
    while (getline(in, line)) {
        len = line.length();
        // Find the beginning of the first token (a word).
        begin = line.find_first_not_of(delim, 0);
        if (begin == string::npos) continue; // If no token is found, skip this line.
        end = line.find_first_of(delim, begin);
        if (end == string::npos) end = len;
        key = line.substr(begin, end - begin);

        // Process each token in the line.
        while (!key.empty()) {
            // Convert the token to lowercase.
            transform(key.begin(), key.end(), key.begin(), ::tolower);

            // If the word is already in the dictionary, increment its frequency.
            // Otherwise, insert it with an initial frequency of 1.
            if (D.contains(key)) {
                D.getValue(key)++;
            } else {
                D.setValue(key, 1);
            }

            // Get the next token.
            begin = line.find_first_not_of(delim, end + 1);
            if (begin == string::npos) break;
            end = line.find_first_of(delim, begin);
            if (end == string::npos) end = len;
            key = line.substr(begin, end - begin);
        }
    }

    // Output the dictionary to the output file.
    out << D;

    // Clean up: clear the dictionary and close the files.
    D.clear();
    in.close();
    out.close();

    return 0;
}