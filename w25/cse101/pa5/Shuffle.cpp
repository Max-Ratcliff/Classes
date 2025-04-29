/***
* Max Ratcliff
* mwratcli
* 2025 Winter CSE101 PA5
* Shuffle.cpp
* client function simulates shuffling a deck of cards, 
* and counting how many shuffles are needed to bring a list back in order
***/ 

#include "List.h"
#include <iostream>
#include <string>
#include <stdexcept>
#include <iomanip>


using namespace std;

void shuffle(List& D) {
    List A, B;
    int x, y;
    int n = D.length();
    D.moveFront();
    while (D.position() < n / 2) { // iterate until n/2 - 1
        x = D.moveNext();
        A.insertBefore(x); // copy the first n/2 - 1 elements into A
    }   
    while (D.position() < n) { // iterate till the end of the list
        y = D.moveNext(); 
        B.insertBefore(y); // copy the second half the of the list (n/2, n-1) in B
    }
    D.clear(); // clear the loop to build the return loop
    A.moveFront(); // first half of list (0, n/2 -1)
    B.moveFront(); //second half of list (n/2, n-1)
    while (A.position() < A.length() && B.position() < B.length()) { // iterate through both loops 
        x = B.moveNext();
        D.insertBefore(x); // insert elements from the Second list

        y = A.moveNext();
        D.insertBefore(y); // insert elements from the first list
    }
    // if one list runs out early, append the rest of the other list
    while (A.position() < A.length()) {
        y = A.moveNext();
        D.insertBefore(y);
    }
    while (B.position() < B.length()) {
        x = B.moveNext();
        D.insertBefore(x);
    }

    // D has now been shuffled
}

int main(int argc, char* argv[]) {
    int count;
    List Deck, Temp;
    if (argc != 2) {
        throw invalid_argument("Incorrect number of arguments. Usage: Shuffle <int>");
    }
    int num = stoi(argv[1]); // convert the size of the deck to an int 
    if (num <= 0) {
        throw invalid_argument("Argument must be a positive integer.");
    }
    cout << "deck size\tshuffle count" << endl;  
    cout << "------------------------------" << endl;    
    for (int n = 1; n <= num; n++) {
        count = 0;
        Deck.moveBack();
        Deck.insertBefore(n - 1); // build list
        Temp = Deck; // save state of deck
        shuffle(Deck); // do a shuffle operation on deck
        count++;
        while (!(Temp == Deck)) { // shuffle until the deck returns to its normal state
            shuffle(Deck);
            count++;
        }
        cout << " " << left << setw(16) << n << count << endl; // print the iteration n and the number of shuffles required for that deck count
    }    
}

