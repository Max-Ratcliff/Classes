/***
* Max Ratcliff
* mwratcli
* 2025 Winter CSE101 PA6
* Arithmatic.cpp
* Client For the BigInteger ADT 
***/ 

#include<iostream>
#include<string>
#include<stdexcept>
#include<fstream>
#include "BigInteger.h"

using namespace std;

int main(int argc, char* argv[]){
    ifstream inFile;
    ofstream outFile;
    // check for correct number of arguments
    if(argc != 3){
        cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl;
        return 1;
    }

    // open input file
    inFile.open(argv[1]);
    if(!inFile){
        cerr << "Unable to open input file: " << argv[1] << endl;
        return 1;
    }

    // open output file
    outFile.open(argv[2]);
    if(!outFile){
        cerr << "Unable to open output file: " << argv[2] << endl;
        return 1;
    }

    // read in the number of test cases
    string BigNum = "";
    inFile >> BigNum; // first integer A
    BigInteger A = BigInteger(BigNum);
    inFile >> BigNum; // blank line
    inFile >> BigNum; // second integer B
    BigInteger B = BigInteger(BigNum);

    // begin operations
    outFile << A << endl<< endl;
    // cout << "printed" << endl;
    outFile << B << endl<< endl;
    // cout << "printed" << endl;
    outFile << (A+B) << endl<< endl;
    // cout << "printed" << endl;
    outFile << (A-B) << endl<< endl;
    // cout << "printed" << endl;
    outFile << (A-A) << endl<< endl;
    // cout << "printed" << endl;
    outFile << (3*A-2*B) << endl<< endl;
    // cout << "printed" << endl;
    outFile << (A*B) << endl<< endl;
    // cout << "printed" << endl;
    outFile << (A*A) << endl<< endl;
    // cout << "printed" << endl;
    outFile << (B*B) << endl<< endl;
    // cout << "printed" << endl;
    outFile << (9*A*A*A*A) + (16*B*B*B*B*B) << endl<< endl;
    // cout << "printed" << endl;

    // close files
    inFile.close();
    outFile.close();

    return 0;
}
