/***
* Max Ratcliff
* mwratcli
* 2025 Winter CSE101 PA1
* ListTest.c
* Contains tests for all ADT operations of the List.
***/
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<assert.h>
#include"List.h"

int main(void){

    //test newList
    List A = newList();
    assert(length(A) == 0);
    assert(index(A) == -1);
    printList(stdout, A);
    printf("\nPassed Test: newList\n");

    // add first element
    append(A, 0);
    assert(length(A) == 1);
    assert(index(A) == -1);
    assert(front(A) == 0);
    assert(back(A) == 0);
    printList(stdout, A);
    printf("\nPassed Test: append 0\n");

    //move cursor to front
    moveFront(A);
    assert(index(A) == 0);
    assert(get(A) == 0);
    printList(stdout, A);
    printf("\nPassed Test: moveFront\n");

    //move cursor to back
    moveBack(A);
    assert(index(A) == 0);
    assert(get(A) == 0);
    printList(stdout, A);
    printf("\nPassed Test: moveBack\n");

    //add second element
    append(A, 1);
    assert(length(A) == 2);
    assert(index(A) == 0);
    assert(front(A) == 0);
    assert(back(A) == 1);
    assert(get(A) == 0);
    printList(stdout, A);
    printf("\nPassed Test: append 1\n");

    //move cursor to back
    moveBack(A);
    assert(index(A) == 1);
    assert(get(A) == 1);
    printList(stdout, A);
    printf("\nPassed Test: moveBack\n");

    // test prepend
    prepend(A, -1);
    assert(length(A) == 3);
    assert(front(A) == -1);
    assert(back(A) == 1);
    assert(index(A) == 2);
    assert(get(A) == 1);
    printList(stdout, A);
    printf("\nPassed Test: prepend -1\n");

    // test insertBefore
    moveFront(A);
    moveNext(A);
    insertBefore(A, -2);
    assert(length(A) == 4);
    assert(index(A) == 2);
    assert(get(A) == 0);
    movePrev(A);
    assert(get(A) == -2);
    printList(stdout, A);
    printf("\nPassed Test: insertBefore -2\n");

    // test insertAfter
    insertAfter(A, -3);
    assert(length(A) == 5);
    moveNext(A);
    assert(get(A) == -3);
    printList(stdout, A);
    printf("\nPassed Test: insertAfter -3\n");

    // test deleteFront
    deleteFront(A);
    assert(length(A) == 4);
    assert(front(A) == -2);
    printList(stdout, A);
    printf("\nPassed Test: deleteFront\n");

    // test deleteBack
    deleteBack(A);
    assert(length(A) == 3);
    assert(back(A) == 0);
    printList(stdout, A);
    printf("\nPassed Test: deleteBack\n");

    // test delete
    moveFront(A);
    moveNext(A);
    delete(A);
    assert(length(A) == 2);
    assert(index(A) == -1);
    printList(stdout, A);
    printf("\nPassed Test: delete\n");

    // test clear
    clear(A);
    assert(length(A) == 0);
    assert(index(A) == -1);
    printList(stdout, A);
    printf("\nPassed Test: clear\n");

    // test equals
    List B = newList();
    append(B, 1);
    append(B, 2);
    append(B, 3);
    List C = newList();
    append(C, 1);
    append(C, 2);
    append(C, 3);
    assert(equals(B, C));
    printList(stdout, B);
    printf("\nPassed Test: equals B\n");
    printList(stdout, C);
    printf("\nPassed Test: equals C\n");

    // test copyList
    List D = copyList(B);
    assert(equals(B, D));
    printList(stdout, D);
    printf("\nPassed Test: copyList\n");

    // test concatList
    List E = concatList(B, C);
    assert(length(E) == 6);
    moveFront(E);
    assert(get(E) == 1);
    moveNext(E);
    assert(get(E) == 2);
    moveNext(E);
    assert(get(E) == 3);
    moveNext(E);
    assert(get(E) == 1);
    moveNext(E);
    assert(get(E) == 2);
    moveNext(E);
    assert(get(E) == 3);
    printList(stdout, E);
    printf("\nPassed Test: concatList\n");

    freeList(&A);
    freeList(&B);
    freeList(&C);
    freeList(&D);
    freeList(&E);

    return 0;
}
