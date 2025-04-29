/***
* Max Ratcliff
* mwratcli
* 2025 Winter CSE101 PA8
* DictionaryTest.cpp
* Comprehensive tests for all Dictionary methods
***/
#include "Dictionary.h"
#include <iostream>
#include <cassert>
#include <stdexcept>
#include <string>

int main() {
    std::cout << "Starting Red-Black Dictionary tests..." << std::endl;

    // Test constructor, insertion, size, contains, and getValue
    Dictionary d;
    d.setValue("c", 3);
    d.setValue("a", 1);
    d.setValue("b", 2);
    d.setValue("d", 4);
    assert(d.size() == 4);
    std::cout << "Insertion and size tests passed." << std::endl;
    
    // Test contains and getValue
    assert(d.contains("a"));
    assert(d.contains("b"));
    assert(!d.contains("z"));
    assert(d.getValue("a") == 1);
    std::cout << "Contains and getValue tests passed." << std::endl;

    // Test update: using setValue to update an existing key
    d.setValue("b", 20);
    assert(d.getValue("b") == 20);
    std::cout << "Update value test passed." << std::endl;

    // Test iterator functionality: begin(), next(), currentKey(), currentVal()
    d.begin();
    if (d.hasCurrent()) {
        std::cout << "Iterator begin current: (" << d.currentKey() << ", " << d.currentVal() << ")" << std::endl;
    }
    std::cout << "In-order traversal using next(): ";
    while(d.hasCurrent()) {
        std::cout << "(" << d.currentKey() << ", " << d.currentVal() << ") ";
        d.next();
    }
    std::cout << std::endl;

    // Test reverse traversal using end() and prev()
    d.end();
    std::cout << "Reverse in-order traversal using prev(): ";
    while(d.hasCurrent()) {
        std::cout << "(" << d.currentKey() << ", " << d.currentVal() << ") ";
        d.prev();
    }
    std::cout << std::endl;

    std::cout << "Pre-order traversal (should mark red nodes with \" (RED)\"):" << std::endl;
    std::cout << d.pre_string() << std::endl;

    // Test exceptions for currentKey() and currentVal() when current is undefined
    d.clear();  // clear dictionary, so current becomes undefined
    try {
        d.currentKey();
        std::cerr << "Error: currentKey() did not throw exception on empty dictionary." << std::endl;
    } catch (std::logic_error& e) {
        std::cout << "Caught expected exception for currentKey() on empty dictionary: " << e.what() << std::endl;
    }

    try {
        d.currentVal();
        std::cerr << "Error: currentVal() did not throw exception on empty dictionary." << std::endl;
    } catch (std::logic_error& e) {
        std::cout << "Caught expected exception for currentVal() on empty dictionary: " << e.what() << std::endl;
    }

    // Test exception for getValue() with a non-existent key
    try {
        d.getValue("nonexistent");
        std::cerr << "Error: getValue() did not throw exception for non-existent key." << std::endl;
    } catch (std::logic_error& e) {
        std::cout << "Caught expected exception for getValue() on non-existent key: " << e.what() << std::endl;
    }

    // Re-insert values for removal tests
    d.setValue("c", 3);
    d.setValue("a", 1);
    d.setValue("b", 2);
    d.setValue("d", 4);

    // Test remove() for leaf node removal (remove "d")
    d.remove("d");
    assert(!d.contains("d"));
    assert(d.size() == 3);
    std::cout << "Leaf removal test passed." << std::endl;

    // Test remove() for node with one child:
    d.remove("a");
    assert(!d.contains("a"));
    assert(d.size() == 2);
    assert(d.contains("b"));
    std::cout << "One-child removal test passed." << std::endl;

    // Test remove() for node with two children
    Dictionary d2;
    d2.setValue("m", 100);
    d2.setValue("c", 50);
    d2.setValue("t", 150);
    d2.setValue("a", 25);
    d2.setValue("e", 75);
    d2.setValue("p", 125);
    d2.setValue("w", 175);
    d2.remove("m");
    assert(!d2.contains("m"));
    assert(d2.size() == 6);
    std::cout << "Two-children removal test passed." << std::endl;
    
    // Test exception for remove() with a non-existent key
    try {
        d2.remove("nonexistent");
        std::cerr << "Error: remove() did not throw exception for non-existent key." << std::endl;
    } catch (std::logic_error& e) {
        std::cout << "Caught expected exception for remove() on non-existent key: " << e.what() << std::endl;
    }
    
    // Test clear() method
    d2.clear();
    assert(d2.size() == 0);
    std::cout << "Clear test passed." << std::endl;
    
    // Test copy constructor
    Dictionary d3;
    d3.setValue("x", 10);
    d3.setValue("y", 20);
    Dictionary d4(d3);
    assert(d4.size() == d3.size());
    assert(d4.equals(d3));
    std::cout << "Copy constructor test passed." << std::endl;
    
    // Test assignment operator
    Dictionary d5;
    d5.setValue("temp", 999);
    d5 = d3;
    assert(d5.size() == d3.size());
    assert(d5.equals(d3));
    std::cout << "Assignment operator test passed." << std::endl;
    
    // Test operator<<, to_string(), and pre_string()
    std::cout << "Dictionary d3 (to_string):\n" << d3.to_string();
    std::cout << "Dictionary d3 (pre_string):\n" << d3.pre_string();
    
    // Test equality operator (==)
    Dictionary d6;
    d6.setValue("x", 10);
    d6.setValue("y", 20);
    assert(d3 == d6);
    std::cout << "Equality operator test passed." << std::endl;
    
    std::cout << "All Red-Black Dictionary tests passed successfully!" << std::endl;
    return 0;
}