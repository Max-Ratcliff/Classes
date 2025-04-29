/***
* Max Ratcliff
* mwratcli
* 2025 Winter CSE101 PA1
* List.c 
* Implementation of doubly linked List ADT with manipulation functions
***/ 

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<assert.h>
#include "List.h"

// structs --------------------------------------------------------------------
typedef struct NodeObj* Node;

typedef struct NodeObj{
    ListElement data;
    Node prev;
    Node next;
} NodeObj;

// private ListObj type
typedef struct ListObj{
    Node front;
    Node back;
    Node cursor;
    int length;
    int index;  // cursor index, -1 if cursor is undefined
} ListObj;

// Constructors-Destructors ---------------------------------------------------

// Creates and returns a new Node with data x
// Returns reference to new Node object, itially with prev and next NULL
Node newNode(ListElement data) {
    Node N = malloc(sizeof(NodeObj));
    N->data = data;
    N->prev = NULL;
    N->next = NULL;
    return N;
}

// Frees heap memory pointed to by *pN, sets *pN to NULL
void freeNode(Node* pN) {
    if(pN != NULL && *pN != NULL) {
        free(*pN);
        *pN = NULL;
    }
}

// Creates and returns a new empty List
List newList(void) {
    List L = malloc(sizeof(ListObj));
    // len is 0 so front and back and cursor all point to the same node which is NULL
    L->front = L->back = L->cursor = NULL;
    L->length = 0;
    L->index = -1;
    return L;
}

// Frees all heap memory associated with List *pL, and sets *pL to NULL
void freeList(List* pL) {
    //check pointers
    if(pL != NULL && *pL != NULL) {
        clear(*pL); 
        free(*pL);
        *pL = NULL;
    }
}

// Access functions -----------------------------------------------------------
int length(List L) {
    // Returns the number of elements in L
    assert(L != NULL);
    return L->length;
}

int index(List L) {
    // Returns index of cursor element if defined, -1 otherwise
    assert(L != NULL);
    return L->index;
}

int front(List L) {
    // Returns front element of L. Precondition: length()>0
    assert(length(L) > 0);
    return L->front->data;
}

int back(List L) {
    assert(length(L) > 0);
    return L->back->data;
}

int get(List L) {
    // Returns cursor element of L. Pre: length()>0, index()>=0
    assert(length(L) > 0 && index(L) >= 0);
    return L->cursor->data;
}

bool equals(List A, List B) {
    // Returns true if Lists A and B are in same state, false otherwise
    assert(A != NULL && B != NULL);
    if(A->length != B->length) {
        return false;
    }
    Node currA = A->front;
    Node currB = B->front;
    while(currA != NULL && currB != NULL) {
        if(currA->data != currB->data) {
            return false;
        }
        currA = currA->next;
        currB = currB->next;
    }
    return true;
}
// Manipulation procedures ----------------------------------------------------

// Resets the list to its original empty state by freeing all nodes and resetting all fields.
// Precondition: L is not NULL.
void clear(List L) {
    // Check if L is not NULL
    // While front is not NULL: save front->next, free current front node, update front
    // Reset all List fields to construction state
    assert(L != NULL);
    while(L->front != NULL) {
        Node next = L->front->next;
        freeNode(&L->front);
        L->front = next;
    }
    L->front = L->back = L->cursor = NULL;
    L->length = 0;
    L->index = -1;
}

void set(List L, int x) {
    // Overwrites the cursor element's data with x. Pre: length()>0, index()>=0
    assert(length(L) > 0 && index(L) >= 0);
    L->cursor->data = x;
}


void moveFront(List L) {
    // If L is non-empty, sets cursor to the front element, otherwise does nothing
    if(length(L) > 0){
    L->cursor = L->front;
    L->index = 0;
    }
}

void moveBack(List L) {
    // If L is non-empty, sets cursor under the back element, otherwise does nothing
    assert(length(L) > 0);
    L->cursor = L->back;
    L->index = L->length - 1;
}

void movePrev(List L) {
    // Move cursor one step toward the front of L; if cursor is defined and not at front,
    // cursor becomes undefined if cursor is at front, cursor undefined does nothing
    assert(length(L) > 0);
    if(L->cursor != NULL && L->cursor != L->front) { // cursor is defined and not at front
        L->cursor = L->cursor->prev; // move cursor to previous node
        L->index--; // decrement index
    } else if(L->cursor == L->front) { // cursor is at front, set it to undefined
        L->cursor = NULL;
        L->index = -1;
    }
    // cursor undefined does nothing
}

void moveNext(List L) {
    // Move cursor one step toward the back of L; if cursor is defined and not at back,
    // cursor becomes undefined if cursor is at back, cursor undefined does nothing
    assert(length(L) > 0);
    if(L->cursor != NULL && L->cursor != L->back) { // cursor is defined and not at back
        L->cursor = L->cursor->next; // move cursor to next node
        L->index++; // increment index
    } else if(L->cursor == L->back) { // cursor is at back, set it to undefined
        L->cursor = NULL;
        L->index = -1;
    }
    // cursor undefined does nothing
}

void prepend(List L, int x) {
    // Insert new element into L. If L is non-empty, insertion takes place before front element
    // check preconditions: L not NULL
    // create new node with x
    // update front pointer
    // increment length
    assert(L != NULL);
    Node N = newNode(x);
    if (length(L) == 0) { // list is empty
        L->front = L->back = N;
        L->length = 1;
    }
    else { // list is not empty
        // link new node to front
        N->next = L->front;
        L->front->prev = N;
        // update front
        L->front = N;
        // increment length of list
        L->length++;
        // update cursor index if defined
        if(L->cursor != NULL) {
            L->index++;
        }
    }
}

void append(List L, int x) {
    // Insert new element into L. If L is non-empty, insertion takes place after back element
    assert(L != NULL);
    Node N = newNode(x);

    if (length(L) == 0) { // list is empty
        L->front = L->back = N;
        L->length = 1;
    }
    else { // list is not empty
        // link new node to back
        N->prev = L->back;
        L->back->next = N;
        // update back
        L->back = N;
        // increment length of list
        L->length++;
        // no need to update cursor index
    }
}

void insertBefore(List L, int x) {
    // Insert new element before cursor. Pre: length()>0, index()>=0
    assert(length(L) > 0 && index(L) >= 0);

    // check if cursor is at front
    if (L->index == 0) {
        prepend(L, x);
    }
    // otherwise insert before cursor
    else {
        // list: [A] <-> [Cursor] <-> [B]
        Node N = newNode(x);
        // link new node to cursor
        N->next = L->cursor; // new node's next is cursor
        // list: [A] <-> [Cursor] <-> [B]
        //       [N] -> [Cursor] <-> [B] 
        N->prev = L->cursor->prev; // new node's prev is cursor's prev
        // list: [A] <-> [Cursor] <-> [B]
        //       [A] <- [N] -> [Cursor] <-> [B] 
        L->cursor->prev->next = N; // move cursor's prev's next to new node
        // list: [A] <- [Cursor] <-> [B]
        //       [A] <-> [N] -> [Cursor] <-> [B] 
        L->cursor->prev = N; // move cursor's prev to new node
        // list: [A] <-> [N] <-> [Cursor] <-> [B]
        // keep cursor's next node the same
        // update cursor index and list length
        L->index++;
        L->length++;
    }
}

void insertAfter(List L, int x) {
     // Insert new element after cursor. Pre: length()>0, index()>=0
    assert(length(L) > 0 && index(L) >= 0);

    // check if cursor is at back
    if (L->index == L->length - 1) {
        append(L, x);
    }
    // otherwise insert after cursor
    else {
        // list: [A] <-> [Cursor] <-> [B]
        Node N = newNode(x);
        // List: [A] <-> [Cursor] <-> [B]
        //                        [N]
        // link new node to cursor
        N->prev = L->cursor; // new node's prev is cursor
        // list: [A] <-> [Cursor] <-> [B]
        //               [Cursor] <-  [N]
        N->next = L->cursor->next; // new node's next is cursor's old next
        // list: [A] <-> [Cursor] <---------> [B]
        //               [Cursor] <-  [N]  -> [B]
        L->cursor->next->prev = N; // move cursor's next's prev to new node
        // list: [A] <-> [Cursor] ----------> [B]
        //               [Cursor] <-  [N] <-> [B]
        L->cursor->next = N; // move cursor's next to new node
        // list: [A] <-> [Cursor] <-> [N] <-> [B]
        // keep cursor's prev node and index are unchanged
        //update length
        L->length++;
    }
}

void deleteFront(List L) {
    // Delete the front element. Pre: length()>0
    assert(length(L) > 0);
    if (length(L) == 1) { // list has 1 element
        freeNode(&L->front);
        // reset all values to construction state
        L->front = NULL;
        L->back = NULL;
        L->cursor = NULL;
        L->index = -1;
        L->length = 0;
        return;
    }
    // store front node in temporary variable
    Node temp = L->front;
    // update front pointer
    L->front = L->front->next;
    // update cursor if defined
    if(L->cursor != NULL) { // cursor is defined
        if (index(L) == 0) { // cursor is at front
            L->cursor = NULL; // update cursor to NULL
            L->index = -1; // update index to -1
        } else { // cursor is not at front
            L->index--; // decrement index because the front node is deleted
        }
    }
    // update front's prev to NULL
    L->front->prev = NULL;
    // free temp
    freeNode(&temp);
    // decrement length
    L->length--;
}

void deleteBack(List L) {
    // Delete the back element. Pre: length()>0
    assert(length(L) > 0);
    if (length(L) == 1) { // list has 1 element
        freeNode(&L->back);
        // reset all values to construction state
        L->front = NULL;
        L->back = NULL;
        L->cursor = NULL;
        L->index = -1;
        L->length = 0;
        return;
    }
    // store back node in temporary variable
    Node temp = L->back;
    // update back pointer
    L->back = L->back->prev;
    // update cursor if defined
    if(L->cursor != NULL) { // cursor is defined
        if (index(L) == L->length - 1) { // cursor is at back
            L->cursor = NULL; // update cursor to NULL
            L->index = -1; // update index to -1
        }
    }
    // update back's next to NULL
    L->back->next = NULL;
    // free temp
    freeNode(&temp);
    // decrement length
    L->length--;

}

void delete(List L) {
    // Delete cursor element, making cursor undefined. Pre: length()>0, index()>=0
    assert(length(L) > 0 && index(L) >= 0 && L->cursor != NULL);
    if (length(L) == 1) { // list has 1 element and cursor is pointing to it
        freeNode(&L->cursor);
        // reset all values to construction state
        L->front = NULL;
        L->back = NULL;
        L->cursor = NULL;
        L->index = -1;
        L->length = 0;
        return;
    } else if (index(L) == 0) { // cursor is at front
        deleteFront(L);
        return;
    } else if (index(L) == L->length - 1) { // cursor is at back
        deleteBack(L);
        return;
    }
    
    // store cursor node in temporary variable
    Node temp = L->cursor;
    // update nodes before and after cursor to skip cursor node
    L->cursor->prev->next = L->cursor->next;
    L->cursor->next->prev = L->cursor->prev;
    // update cursor node to NULL
    L->cursor = NULL;
    // free temp
    freeNode(&temp);
    // decrement length
    L->length--;
    // update index
    L->index = -1;
}

// Other operations -----------------------------------------------------------

void printList(FILE* out, List L) {
    // Prints L to the file pointed to by out, formatted as a space-separated string
    assert(out != NULL);
    if (length(L) == 0) return;
    Node N = L->front;
    while (N != NULL) {
        fprintf(out, "%d", N->data);
        if (N->next != NULL) {
            fprintf(out, " ");  // Add a space only between elements
        }
        N = N->next;
    }
    //fprintf(out, "\n");
}

List copyList(List L) {
    // Returns a new List representing the same integer sequence as L
    // The cursor in the new list is undefined, regardless of the state of the cursor in L
    // The state of L is unchanged
    List copy = newList();
    Node N = L->front;
    while (N != NULL) {
        append(copy, N->data);
        N = N->next;
    }
    return copy;
}

List concatList(List A, List B){
    // Returns a new List which is the concatenation of
    // A and B. The cursor in the new List is undefined,
    // regardless of the states of the cursors in A and B.
    // The states of A and B are unchanged.
    List concat = copyList(A); // concat is a copy of A
    Node N = B->front;
    while (N != NULL) { // append all elements of B to concat in order
        append(concat, N->data);
        N = N->next;
    }
    return concat;
}
