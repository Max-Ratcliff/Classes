/***
* Max Ratcliff
* mwratcli
* 2025 Winter CSE101 PA6
* ListTest.cpp
* Test all of the List ADT's manipulation function
***/ 

#include <iostream>
#include <string>
#include <stdexcept>
#include "List.h"

using namespace std;

int main() {
    // Test constructor
    List A;
    if (A.length() != 0) cout << "Constructor error" << endl;

    // Test copy constructor
    A.insertBefore(1);
    if(A.length() != 1 ||
         A.back() != 1 || 
         A.front() != 1
        ) cout << "insert  error" << endl;
    A.insertBefore(2);
    if(A.length() != 2 ||
        A.back() != 2 ||
        A.front() != 1
        ) cout << "insert  error" << endl;
    A.insertBefore(3);
    if(A.length() != 3 ||
        A.back() != 3 ||
        A.front() != 1
        ) cout << "insert  error" << endl;

    List B = A;
    if(B.length() != 3 ||
        A.back() != 3 ||
        A.front() != 1 ||
        !(A == B)
        ) cout << "Copy constructor error" << endl;
    
    // Test position
    A.moveFront();
    if (A.position() != 0) cout << "Position error" << endl;
    
    // Test peekNext/peekPrev
    if (A.peekNext() != 1) cout << "PeekNext error" << endl;
    if (A.moveNext() != 1) cout << "MoveNext error" << endl;
    if (A.peekPrev() != 1) cout << "PeekPrev error" << endl;

    // Test clear
    A.clear();
    if (A.length() != 0) cout << "Clear error" << endl;

    // Test moveFront/moveBack
    A.insertAfter(1);
    A.insertAfter(2);
    A.moveFront();
    if (A.position() != 0) cout << "MoveFront error" << endl;
    A.moveBack();
    if (A.position() != 2) cout << "MoveBack error" << endl;
    // Test moveNext/movePrev
    A.moveFront();
    A.moveNext();
    if (A.position() != 1) cout << "MoveNext error" << endl;
    A.movePrev();
    if (A.position() != 0) cout << "MovePrev error" << endl;

    // Test insertAfter/insertBefore
    A.clear();
    A.insertAfter(1);
    A.insertBefore(2);
    if (A.front() != 2) cout << "InsertBefore error" << endl;
    if (A.back() != 1) cout << "InsertAfter error" << endl;
    // Test setAfter/setBefore
    A.moveFront();
    if (A.moveNext() != 2) cout << "moveNext error" << endl;
    A.setAfter(3);
    if (A.back() != 3) cout << "SetAfter error" << endl;
    A.setBefore(4);
    if (A.front() != 4) cout << "SetBefore error" << endl;
    // Test eraseAfter/eraseBefore
    A.moveFront();
    A.moveNext();
    A.eraseAfter();
    if (A.length() != 1) cout << "EraseAfter error" << endl;
    A.eraseBefore();
    if (A.length() != 0) cout << "EraseBefore error" << endl;
    // Test findNext/findPrev
    A.clear();

    A.insertBefore(1);
    A.insertBefore(2);
    A.insertBefore(3);
    A.insertBefore(2);
    A.moveFront();
    if (A.findNext(3) != 3 && A.peekPrev() != 3) cout << "FindNext error" << endl;
    A.moveBack();
    if (A.findPrev(2) != 3 && A.peekNext() != 2) cout << "FindPrev error" << endl;

    A.clear();
    B.clear();
    A.moveFront();
    B.moveFront();

    A.insertBefore(1);
    A.insertBefore(2);
    A.insertBefore(3);

    B.insertBefore(1);
    B.insertBefore(1);
    B.insertBefore(2);
    B.insertBefore(2);
    B.insertBefore(3);
    B.insertBefore(3);

    A.moveFront();
    B.moveFront();

    cout << "A" << endl;
    cout << A.to_string() << endl;
    cout << "B" << endl;
    cout << B.to_string() << endl;
    
    List C = A.concat(B);
    cout << "C = A+B" << endl;
    cout << C.to_string() << endl;
    
    B.cleanup();
    C.cleanup();
    cout << "after cleanup!" << endl;
    cout << "B" << endl;
    cout << B.to_string() << endl;
    cout << "C" << endl;
    cout << C.to_string() << endl;

    A.clear();
    B.clear();
    C.clear();
    



    cout << "All tests complete" << endl;
    return 0;
}