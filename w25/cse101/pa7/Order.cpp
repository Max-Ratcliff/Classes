/***
* Max Ratcliff
* mwratcli
* 2025 Winter CSE101 PA7
* Order.cpp
* client of Dictionary ADT 
***/ 

#include <iostream>
#include <fstream>
#include <string>
#include "Dictionary.h"

using namespace std;

int main(int argc, char* argv[]){
    // Check for correct number of command-line arguments
    if(argc != 3){
        cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl;
        return 1;
    }

    // Open the input file
    ifstream infile(argv[1]);
    if(!infile){
        cerr << "Unable to open input file " << argv[1] << endl;
        return 1;
    }

    // Open the output file
    ofstream outfile(argv[2]);
    if(!outfile){
        cerr << "Unable to open output file " << argv[2] << endl;
        return 1;
    }

    // Create a Dictionary instance
    Dictionary dict;

    // read and insert into the Dictionary
    string line;
    int lineNum = 1;
    while(getline(infile, line)){
        dict.setValue(line, lineNum);
        lineNum++;
    }

    // print in key value pairs in alphabetical order
    outfile << dict.to_string() << endl;

    // print pre-order traversal
    outfile << dict.pre_string() << endl;

    // Clean up: close file streams
    infile.close();
    outfile.close();

    return 0;
}