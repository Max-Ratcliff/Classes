/***
* Max Ratcliff
* mwratcli
* 2025 Winter CSE101 PA1
* Lex.c
* Sorts lines of input file lexicographically by inserting indices into a doubly Linked list and writes to output file.
***/

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<assert.h>
#include<string.h>
#include"List.h"

int main(int argc, char* argv[]){
    FILE *inFile = NULL, *outFile = NULL; // in and out files
    char buffer[300]; // buffer to read input
    List indexList = newList(); // empty list to store sorted input
    char **lines; // array of strings to store input
    int n; // number of lines in input file

    //make sure both files are provided
    if (argc != 3) {
        fprintf(stderr, "Requires 2 arguments\n");
        exit(1);
    }

    //open infile for reading and outfile for writing
    inFile = fopen(argv[1], "r");
    if (inFile == NULL) {
        fprintf(stderr, "Could not open input file: %s\n", argv[1]);
        exit(1);
    }
    outFile = fopen(argv[2], "w");
    if (outFile == NULL) {
        fprintf(stderr, "Could not open output file: %s\n", argv[2]);
        exit(1);
    }

    // count lines to allocate string array
    n = 0;
    while (fgets(buffer, sizeof(buffer), inFile) != NULL) {
        n++;
    }

    // allocate string array
    lines = calloc(n, sizeof(char*));
    if (lines == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    // go back to beginning of file
    rewind(inFile);

    // fill string array
    for (int i = 0; i < n; i++) {
        lines[i] = calloc(300, sizeof(char));
        if (lines[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }
        fgets(buffer, sizeof(buffer), inFile);
        strcpy(lines[i], buffer);
    }

    /*DEBUG: print string array
    for (int i = 0; i < n; i++) {
        printf("%s", lines[i]);
    }
    printf("\n");
    /**/

    //indirectly sort string array
    //add first line to index list
    append(indexList, 0);
    moveFront(indexList); //set cursor to first element
    //loop through string array
    // use an insertion sort like algorithm to sort the array indexes
        // select first line
        // move cursor to first element
        // loop through the lines starting at line 1
            // while the cursor is defined
            // if the line is lexographically smaller than the element at the cursor, insert it before the cursor element
            // otherwise move the cursor over 1
    for (int i = 1; i < n; i++) {
        moveFront(indexList);
        while (index(indexList) != -1){
            if (strcmp(lines[i], lines[get(indexList)]) < 0){
                insertBefore(indexList, i);
                break;
            }
            moveNext(indexList);
            if (index(indexList) == -1) {
                append(indexList, i); // is the biggest element in the list
            }
        }
    }

    /*DEBUG: print sorted linked list
    printList(stdout, indexList);
    printf("\n");
    /**/

    // write the sorted lines back into the outFile
    moveFront(indexList);
    while (index(indexList) != -1) {
        fprintf(outFile, "%s", lines[get(indexList)]);
        moveNext(indexList);
    }

    //close files
    fclose(inFile);
    fclose(outFile);

    //free memory allocations
    freeList(&indexList);
    for (int i = 0; i < n; i++) {
        free(lines[i]);
    }
    free(lines);

    return 0;
}
