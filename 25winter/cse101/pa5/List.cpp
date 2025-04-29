/***
* Max Ratcliff
* mwratcli
* 2025 Winter CSE101 PA5
* List.cpp
* Implementation of doubly linked List ADT with manipulation functions
***/ 

#include "List.h"
#include <iostream>
#include <string>
#include <stdexcept>
#include <unordered_set>

// Node constructor
List::Node::Node(ListElement x) {
    data = x;
    next = nullptr;
    prev = nullptr;
}

// Class Constructors & Destructors ----------------------------------------

// Creates new List in the empty state.
List::List() {
    // set the front and back sentinal nodes 
    frontDummy = new Node(-1);
    backDummy = new Node(-1);
    frontDummy->next = backDummy;
    backDummy->prev = frontDummy;

    beforeCursor = frontDummy;
    afterCursor = backDummy;
    pos_cursor = 0;
    num_elements = 0;
}

// Copy constructor.
List::List(const List& L) {
    // set the front and back sentinal nodes 
    frontDummy = new Node(0);
    backDummy = new Node(0);
    frontDummy->next = backDummy;
    backDummy->prev = frontDummy;

    beforeCursor = frontDummy;
    afterCursor = backDummy;
    pos_cursor = 0;
    num_elements = 0;
    
    // copy elements 
    // L is a const so we manually iterate over it
    Node* curr = L.frontDummy->next; // Start from the first real node in L
    while (curr != L.backDummy) { // Iterate until the backDummy of L
        insertAfter(curr->data); // insert elements from L into new list
        moveNext();
        curr = curr->next;
    }
    moveFront(); // set cursor position to 0
}

// Destructor
List::~List() {
    clear();
    delete frontDummy;
    delete backDummy;
}


// Access functions --------------------------------------------------------

// Returns the length of this List.
int List::length() const {
    return num_elements; 
}

// Returns the front element in this List.
// pre: length()>0
ListElement List::front() const {
    if (length() > 0){
        return frontDummy->next->data; // element 1
    }
    throw std::length_error("List: front(): empty list");
}

// Returns the back element in this List.
// pre: length()>0
ListElement List::back() const {
    if (length() > 0){
        return backDummy->prev->data; // element -2 
    }
    throw std::length_error("List: back(): empty list");
}

// Returns the position of cursor in this List: 0 <= position() <= length().
int List::position() const {
    if (0 <= pos_cursor && pos_cursor <= length()){
        return pos_cursor;
    }
    throw std::range_error("List: position() Cursor out of range");
}

// Returns the element after the cursor.
// pre: position()<length()
ListElement List::peekNext() const {
    if(position() < length()){
        return afterCursor->data;
    }
    throw std::range_error("List: peekNext(): cursor at back");
}

// Returns the element before the cursor.
// pre: position()>0
ListElement List::peekPrev() const {
    if(position() > 0){
        return beforeCursor->data;
    }
    throw std::range_error("List: peekPrev(): cursor at front");

}

// Manipulation procedures -------------------------------------------------

// Deletes all elements in this List, setting it to the empty state.
void List::clear() {
    moveFront(); 
    while(length() > 0){ //while the list contains elements
        eraseAfter(); // delete the element after the cursor
    }  
    pos_cursor = 0;
    num_elements = 0;
}

// Moves cursor to position 0 in this List.
void List::moveFront() {
    beforeCursor = frontDummy;
    afterCursor = frontDummy->next;
    pos_cursor = 0;
}

// Moves cursor to position length() in this List.
void List::moveBack() {
    afterCursor = backDummy;
    beforeCursor = backDummy->prev;
    pos_cursor = length();
}

// Advances cursor to next higher position. Returns the List element that
// was passed over. 
// pre: position()<length() 
ListElement List::moveNext() {
    if(position() < length()){
        beforeCursor = beforeCursor->next;
        afterCursor = afterCursor->next;   
        pos_cursor++;
        return beforeCursor->data; // return passed over value
    }
    throw std::range_error("List: moveNext(): cursor at back");
}

// Advances cursor to next lower position. Returns the List element that
// was passed over. 
// pre: position()>0
ListElement List::movePrev() {
    if(position() > 0){
        beforeCursor = beforeCursor->prev;
        afterCursor = afterCursor->prev;   
        pos_cursor--;
        return afterCursor->data; // return passed over value
    }
    throw std::range_error("List: movePrev(): cursor at front");
}

// Inserts x after cursor.
void List::insertAfter(ListElement x) {
    Node *nEl = new Node(x); // new Element
    // set the new Element's prev and next nodes
    nEl->prev = beforeCursor;
    nEl->next = afterCursor;
    // update old elements to point towards the New Element
    beforeCursor->next = nEl;
    afterCursor->prev = nEl;
    // updates aftercursor to point at the new element
    afterCursor = nEl;

    num_elements++; // size of lise increases
}

// Inserts x before cursor.
void List::insertBefore(ListElement x) {
    Node *nEl = new Node(x); // new Element
    // set the new Element's prev and next nodes
    nEl->prev = beforeCursor;
    nEl->next = afterCursor; 
    // update old elements to point towards the New Element
    beforeCursor->next = nEl;
    afterCursor->prev = nEl;
    // updates beforeCursor to point at the new element
    beforeCursor = nEl;

    num_elements++; // size of lise increases 
    pos_cursor++; // increase cursor index
}

// Overwrites the List element after the cursor with x.
// pre: position()<length()
void List::setAfter(ListElement x) {
    if(position() < length()){
        afterCursor->data = x;
    } else {
        throw std::range_error("List: setAfter(): cursor at back");
    }
}

// Overwrites the List element before the cursor with x.
// pre: position()>0
void List::setBefore(ListElement x) {
    if(position() > 0){
        beforeCursor->data = x;
    } else {
        throw std::range_error("List: setBefore(): cursor at front"); 
    }
}

// Deletes element after cursor.
// pre: position()<length()
void List::eraseAfter() {
    if(position() < length()){
        Node* deleteMe = afterCursor; //save after cursor
        beforeCursor->next = afterCursor->next; // skip deleted node 
        deleteMe->next->prev = beforeCursor;
        afterCursor = afterCursor->next; // move after cursor

        delete deleteMe;
        num_elements--;
    } else {
        throw std::range_error("List: eraseAfter(): cursor at back");
    }
}

// Deletes element before cursor.
// pre: position()>0
void List::eraseBefore() {
    if(position() > 0){
        Node* deleteMe = beforeCursor; //save before cursor
        afterCursor->prev = beforeCursor->prev;
        beforeCursor = beforeCursor->prev;
        beforeCursor->next = afterCursor;

        delete deleteMe;
        num_elements--;
        pos_cursor--;
    } else {
        throw std::range_error("List: eraseBefore(): cursor at front"); 
    }
}


// Other Functions ---------------------------------------------------------

// Starting from the current cursor position, performs a linear search (in 
// the direction front-to-back) for the first occurrence of element x. If x
// is found, places the cursor immediately after the found element, then 
// returns the final cursor position. If x is not found, places the cursor 
// at position length(), and returns -1. 
int List::findNext(ListElement x) {
    while(position()<length()){
        if (moveNext() == x){
            return position();
        }
    }
    return -1;
}

// Starting from the current cursor position, performs a linear search (in 
// the direction back-to-front) for the first occurrence of element x. If x
// is found, places the cursor immediately before the found element, then
// returns the final cursor position. If x is not found, places the cursor 
// at position 0, and returns -1. 
int List::findPrev(ListElement x) {
    while(position() > 0){
        if(movePrev() == x){
            return position();
        }
    }
    return -1; 
}

// Removes any repeated elements in this List, leaving only unique elements.
// The order of the remaining elements is obtained by retaining the frontmost 
// occurrance of each element, and removing all other occurances. The cursor 
// is not moved with respect to the retained elements, i.e. it lies between 
// the same two retained elements that it did before cleanup() was called.
void List::cleanup() {
    int position = 0;  // Tracks the position of `cursor` in the list.
    
    Node *cursor = frontDummy->next;  // Start from the first actual element in the list.

    do {  // Outer loop iterates through each element in the list.
        Node *current = cursor;  // `current` tracks the node whose duplicates will be removed.
        Node *n = cursor->next;  // `n` starts as the next node and will be checked for duplicates.
        int inner_position = position + 1;  // Tracks `n`'s position relative to `cursor`.

        while (n != backDummy) {  // Inner loop scans the rest of the list for duplicates of `cursor->data`.
            if (cursor->data == n->data) {  // If `n` is a duplicate of `cursor`
                current->next = n->next;  // Remove `n` by linking `current`'s `next` to `n->next`.
                n->next->prev = current;  // Maintain bidirectional linking by updating `prev` pointer of the next node.
                delete n;  // Free memory allocated for the duplicate node.
                n = current->next;  // Move `n` to the next node in the list.
                num_elements--;  // Decrease the count of elements in the list.

                if (inner_position <= pos_cursor) {  // If deleted element was before the cursor, adjust cursor position.
                    pos_cursor--;  
                }
            } else {  // If `n` is not a duplicate:
                current = n;  // Move `current` forward to `n`.
                n = current->next;  // Move `n` to the next node.
            }
            inner_position++;  // Move inner position counter forward.
        }

        position++;  // Move to the next element in the outer loop.
        cursor = cursor->next;  // Advance `cursor` to process the next unique element.

    } while (cursor != backDummy);  // Continue until we reach the dummy tail node.
}

// Returns a new List consisting of the elements of this List, followed by
// the elements of L. The cursor in the returned List will be at postion 0.
List List::concat(const List& L) const {
    List newL = L;
    List temp = *this;
    newL.moveFront();
    temp.moveFront();
    while(temp.position() < temp.length()){
        ListElement x = temp.moveNext();
        newL.insertBefore(x);
    }
    newL.moveFront();
    return newL;
}

// Returns a string representation of this List consisting of a comma 
// separated sequence of elements, surrounded by parentheses.
std::string List::to_string() const {
    if(length() == 0){
        return "()";
    }

    List temp = *this;
    temp.moveFront();

    std::string str = "(" + std::to_string(temp.moveNext());

    // std::cout << "length: " << temp.length() << std::endl;
    // std::cout << "str: " << str << std::endl;

    while(temp.position() < temp.length()){
        // std::cout << "length: " << temp.length() << std::endl;
        // std::cout << "position: " << temp.position() << std::endl;
        str += ", " + std::to_string(temp.moveNext());
        // std::cout << "str: " << str << std::endl;
    }
    str += ")";
    // std::cout << "str: " << str << std::endl;
    return str;
}

// Returns true if and only if this List is the same integer sequence as R.
// The cursors in this List and in R are unchanged.
bool List::equals(const List& R) const {
    if(length() != R.length()){ 
        //if lists arent the same length they can not be equal
        return false;
    }
    List A = *this; // copy lists to move cursor
    List B = R;
    A.moveFront();
    B.moveFront();
    while(A.position() < A.length()){
        if(A.moveNext() != B.moveNext()){
            return false;
        }
    }
    return true;
}


// Overriden Operators -----------------------------------------------------

// Inserts string representation of L into stream.
std::ostream& operator<<(std::ostream& stream, const List& L) {
    stream << L.to_string();
    return stream;
}

// Returns true if and only if A is the same integer sequence as B. The 
// cursors in both Lists are unchanged.
bool operator==(const List& A, const List& B) {
    return A.equals(B);
}

// Overwrites the state of this List with state of L.
List& List::operator=(const List& L) {
    if(this != &L){
        List temp(L);
        std::swap(frontDummy, temp.frontDummy);
        std::swap(backDummy, temp.backDummy);
        std::swap(beforeCursor, temp.beforeCursor);
        std::swap(afterCursor, temp.afterCursor);
        std::swap(pos_cursor, temp.pos_cursor);
        std::swap(num_elements, temp.num_elements);
    }
    return *this;
}