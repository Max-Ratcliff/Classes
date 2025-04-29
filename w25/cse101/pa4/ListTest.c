/**
 * Max Ratcliff
 * mwratcli
 * 2025 Winter CSE101 PA4
 * ListTest.c
 *
 * Contains tests for all ADT operations of the List,
 * using void* nodes (storing pointers to int).
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <assert.h>
 #include "List.h"
 
 int main(void) {
     // make int variables that we can create pointers for to test the List
     int p0 = 0, p1 = 1;
     int pn1 = -1, pn2 = -2, pn3 = -3;
 
     List A = newList();
     assert(length(A) == 0);
     assert(index(A) == -1);
     printf("Passed Test: newList\n");
 
     append(A, &p0);  // Append 0 to A.
     assert(length(A) == 1);
     assert(index(A) == -1);  // Cursor remains undefined.
     assert(*(int*)front(A) == 0);
     assert(*(int*)back(A) == 0);
     printf("Passed Test: append\n");
 
     moveFront(A);  // Cursor now at the first element.
     assert(index(A) == 0);
     assert(*(int*)get(A) == 0);
     printf("Passed Test: moveFront\n");
 
     moveBack(A); // Cursor should still be at front 
     assert(index(A) == 0);
     assert(*(int*)get(A) == 0);
     printf("Passed Test: moveBack len = 1\n");
 
     append(A, &p1);  // Append 1
     assert(length(A) == 2);
     assert(*(int*)front(A) == 0);
     assert(*(int*)back(A) == 1);
     assert(*(int*)get(A) == 0);
 
     /***** Test moveBack again *****/
     moveBack(A);
     assert(index(A) == 1);
     assert(*(int*)get(A) == 1);
     printf("Passed Test: moveBack len > 1\n");
 
     prepend(A, &pn1); 
     assert(length(A) == 3);
     assert(*(int*)front(A) == -1);
     assert(*(int*)back(A) == 1);
     assert(index(A) == 2);
     assert(*(int*)get(A) == 1);
     printf("Passed Test: prepend\n");
 
     moveFront(A);
     moveNext(A); 
     insertBefore(A, &pn2);
     insertAfter(A, &pn3);
     assert(length(A) == 5);
     moveNext(A); 
     assert(*(int*)get(A) == -3);
     printf("Passed Test: insertAfter\n");
 
     deleteFront(A);
     assert(length(A) == 4);
     assert(*(int*)front(A) == -2);
     printf("Passed Test: deleteFront\n");
 
     deleteBack(A);
     assert(length(A) == 3);
     assert(*(int*)back(A) == -3);
     printf("Passed Test: deleteBack\n");
 
     moveFront(A);
     moveNext(A);
     delete(A); 
     assert(length(A) == 2);
     assert(index(A) == -1);
     printf("Passed Test: delete\n");
 
     clear(A);
     assert(length(A) == 0);
     assert(index(A) == -1);
     printf("Passed Test: clear\n");
 
     freeList(&A);
     printf("All tests passed successfully!\n");
     return 0;
 }