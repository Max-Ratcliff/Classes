/***
* Max Ratcliff
* mwratcli
* 2025 Winter CSE101 PA6
* BigIntegerTest.cpp
* Comprehensive tests for all BigInteger methods and operators with enhanced messaging.
***/

#include <iostream>
#include <string>
#include <stdexcept>
#include "BigInteger.h"  // make sure this header includes your BigInteger class

using namespace std;

// Helper functions for printing messages.
void printSuccess(const string& testName) {
    cout << "[PASS] " << testName << endl;
}

void printError(const string& testName, const string& errorMsg) {
    cout << "[ERROR] " << testName << ": " << errorMsg << endl;
}

int main() {
    bool allPassed = true;

    // ***** Default Constructor *****
    cout << "Testing Default Constructor..." << endl;
    {
        BigInteger a;
        if(a.to_string() != "0") {
            printError("Default Constructor", "Expected 0, got " + a.to_string());
            allPassed = false;
        } else {
            printSuccess("Default Constructor");
        }
    }

    // ***** Constructor from long *****
    cout << "\nTesting Constructor from long..." << endl;
    {
        BigInteger a(0);
        if(a.to_string() != "0") {
            printError("Constructor(long) with 0", "Expected 0, got " + a.to_string());
            allPassed = false;
        } else {
            printSuccess("Constructor(long) with 0");
        }
        
        BigInteger b(123456789);
        if(b.to_string() != "123456789") {
            printError("Constructor(long) with positive number", "Expected 123456789, got " + b.to_string());
            allPassed = false;
        } else {
            printSuccess("Constructor(long) with positive number");
        }
        
        BigInteger c(-987654321);
        if(c.to_string() != "-987654321") {
            printError("Constructor(long) with negative number", "Expected -987654321, got " + c.to_string());
            allPassed = false;
        } else {
            printSuccess("Constructor(long) with negative number");
        }
    }

    // ***** Constructor from string *****
    cout << "\nTesting Constructor from string..." << endl;
    {
        try {
            BigInteger a("0");
            if(a.to_string() != "0") {
                printError("Constructor(string) for \"0\"", "Expected 0, got " + a.to_string());
                allPassed = false;
            } else {
                printSuccess("Constructor(string) for \"0\"");
            }
        } catch (...) {
            printError("Constructor(string) for \"0\"", "Unexpected exception thrown");
            allPassed = false;
        }

        try {
            BigInteger b("12345678901234567890");
            if(b.to_string() != "12345678901234567890") {
                printError("Constructor(string) for large number", "Expected 12345678901234567890, got " + b.to_string());
                allPassed = false;
            } else {
                printSuccess("Constructor(string) for large number");
            }
        } catch (...) {
            printError("Constructor(string) for large number", "Unexpected exception thrown");
            allPassed = false;
        }

        try {
            BigInteger c("+123456789");
            if(c.to_string() != "123456789") {
                printError("Constructor(string) for \"+123456789\"", "Expected 123456789, got " + c.to_string());
                allPassed = false;
            } else {
                printSuccess("Constructor(string) for \"+123456789\"");
            }
        } catch (...) {
            printError("Constructor(string) for \"+123456789\"", "Unexpected exception thrown");
            allPassed = false;
        }

        try {
            BigInteger d("-123456789");
            if(d.to_string() != "-123456789") {
                printError("Constructor(string) for \"-123456789\"", "Expected -123456789, got " + d.to_string());
                allPassed = false;
            } else {
                printSuccess("Constructor(string) for \"-123456789\"");
            }
        } catch (...) {
            printError("Constructor(string) for \"-123456789\"", "Unexpected exception thrown");
            allPassed = false;
        }

        // Invalid strings should throw exceptions.
        try {
            BigInteger e("12345abc6789");
            printError("Constructor(string) for \"12345abc6789\"", "Failed to throw exception for invalid characters");
            allPassed = false;
        } catch (const std::invalid_argument& err) {
            printSuccess("Constructor(string) for \"12345abc6789\" (expected exception)");
        }

        try {
            BigInteger f("");
            printError("Constructor(string) for empty string", "Failed to throw exception for empty string");
            allPassed = false;
        } catch (const std::invalid_argument& err) {
            printSuccess("Constructor(string) for empty string (expected exception)");
        }
    }

    // ***** Copy Constructor *****
    cout << "\nTesting Copy Constructor..." << endl;
    {
        BigInteger a("987654321");
        BigInteger b(a);
        if(b.to_string() != "987654321") {
            printError("Copy Constructor", "Expected 987654321, got " + b.to_string());
            allPassed = false;
        } else {
            printSuccess("Copy Constructor");
        }
    }

    // ***** sign() Accessor *****
    cout << "\nTesting sign() Accessor..." << endl;
    {
        BigInteger a("123");
        BigInteger b("-456");
        BigInteger c("0");
        if(a.sign() != 1) {
            printError("sign() for positive number", "Expected 1, got " + to_string(a.sign()));
            allPassed = false;
        } else {
            printSuccess("sign() for positive number");
        }
        if(b.sign() != -1) {
            printError("sign() for negative number", "Expected -1, got " + to_string(b.sign()));
            allPassed = false;
        } else {
            printSuccess("sign() for negative number");
        }
        if(c.sign() != 0) {
            printError("sign() for zero", "Expected 0, got " + to_string(c.sign()));
            allPassed = false;
        } else {
            printSuccess("sign() for zero");
        }
    }

    // ***** compare() Function *****
    cout << "\nTesting compare() Function..." << endl;
    {
        BigInteger a("100");
        BigInteger b("200");
        BigInteger c("100");
        BigInteger d("-50");
        if(a.compare(b) != -1) {
            printError("compare() for 100 vs 200", "Expected -1, got " + to_string(a.compare(b)));
            allPassed = false;
        } else {
            printSuccess("compare() for 100 vs 200");
        }
        if(b.compare(a) != 1) {
            printError("compare() for 200 vs 100", "Expected 1, got " + to_string(b.compare(a)));
            allPassed = false;
        } else {
            printSuccess("compare() for 200 vs 100");
        }
        if(a.compare(c) != 0) {
            printError("compare() for 100 vs 100", "Expected 0, got " + to_string(a.compare(c)));
            allPassed = false;
        } else {
            printSuccess("compare() for 100 vs 100");
        }
        if(d.compare(a) != -1) {
            printError("compare() for -50 vs 100", "Expected -1, got " + to_string(d.compare(a)));
            allPassed = false;
        } else {
            printSuccess("compare() for -50 vs 100");
        }
    }

    // ***** makeZero() Method *****
    cout << "\nTesting makeZero() Method..." << endl;
    {
        BigInteger a("123456");
        a.makeZero();
        if(a.to_string() != "0") {
            printError("makeZero()", "Expected 0, got " + a.to_string());
            allPassed = false;
        } else {
            printSuccess("makeZero()");
        }
    }

    // ***** negate() Method *****
    cout << "\nTesting negate() Method..." << endl;
    {
        BigInteger a("123456");
        a.negate();
        if(a.to_string() != "-123456") {
            printError("negate() on positive number", "Expected -123456, got " + a.to_string());
            allPassed = false;
        } else {
            printSuccess("negate() on positive number");
        }
        a.negate();
        if(a.to_string() != "123456") {
            printError("negate() on negative number", "Expected 123456, got " + a.to_string());
            allPassed = false;
        } else {
            printSuccess("negate() on negative number");
        }
        BigInteger b("0");
        printSuccess("b(0)");
        b.negate();
        printSuccess(b.to_string());
        if(b.to_string() != "0") {
            printError("negate() on zero", "Expected 0, got " + b.to_string());
            allPassed = false;
        } else {
            printSuccess("negate() on zero");
        }
    }

    // ***** add() Method *****
    cout << "\nTesting add() Method..." << endl;
    {
        {
            BigInteger a("123456789");
            BigInteger b("987654321");
            BigInteger c = a.add(b);
            if(c.to_string() != "1111111110") {
                printError("add() with positive numbers", "Expected 1111111110, got " + c.to_string());
                allPassed = false;
            } else {
                printSuccess("add() with positive numbers");
            }
        }
        {
            BigInteger d("1000");
            BigInteger e("-500");
            BigInteger f = d.add(e);
            if(f.to_string() != "500") {
                printError("add() with mixed sign (1000 + (-500))", "Expected 500, got " + f.to_string());
                allPassed = false;
            } else {
                printSuccess("add() with mixed sign (1000 + (-500))");
            }
        }
        {
            BigInteger g("-1000");
            BigInteger h("500");
            BigInteger i = g.add(h);
            if(i.to_string() != "-500") {
                printError("add() with mixed sign (-1000 + 500)", "Expected -500, got " + i.to_string());
                allPassed = false;
            } else {
                printSuccess("add() with mixed sign (-1000 + 500)");
            }
        }
        {
            BigInteger j("-200");
            BigInteger k("-300");
            BigInteger l = j.add(k);
            if(l.to_string() != "-500") {
                printError("add() with two negatives (-200 + (-300))", "Expected -500, got " + l.to_string());
                allPassed = false;
            } else {
                printSuccess("add() with two negatives (-200 + (-300))");
            }
        }
        {
            BigInteger m("12345");
            BigInteger n("0");
            BigInteger o = m.add(n);
            if(o.to_string() != "12345") {
                printError("add() with zero", "Expected 12345, got " + o.to_string());
                allPassed = false;
            } else {
                printSuccess("add() with zero");
            }
        }
    }

    // ***** sub() Method *****
    cout << "\nTesting sub() Method..." << endl;
    {
        {
            BigInteger a("1000");
            BigInteger b("500");
            BigInteger c = a.sub(b);
            if(c.to_string() != "500") {
                printError("sub() with positive result (1000 - 500)", "Expected 500, got " + c.to_string());
                allPassed = false;
            } else {
                printSuccess("sub() with positive result (1000 - 500)");
            }
        }
        {
            BigInteger d("500");
            BigInteger e("1000");
            BigInteger f = d.sub(e);
            if(f.to_string() != "-500") {
                printError("sub() with negative result (500 - 1000)", "Expected -500, got " + f.to_string());
                allPassed = false;
            } else {
                printSuccess("sub() with negative result (500 - 1000)");
            }
        }
        {
            BigInteger g("123456789");
            BigInteger h("123456789");
            BigInteger i = g.sub(h);
            if(i.to_string() != "0") {
                printError("sub() resulting in zero", "Expected 0, got " + i.to_string());
                allPassed = false;
            } else {
                printSuccess("sub() resulting in zero");
            }
        }
        {
            BigInteger j("1000");
            BigInteger k("1");
            BigInteger l = j.sub(k);
            if(l.to_string() != "999") {
                printError("sub() with borrow (1000 - 1)", "Expected 999, got " + l.to_string());
                allPassed = false;
            } else {
                printSuccess("sub() with borrow (1000 - 1)");
            }
        }
    }

    // ***** mult() Method *****
    cout << "\nTesting mult() Method..." << endl;
    {
        {
            BigInteger a("12345");
            BigInteger b("6789");
            BigInteger c = a.mult(b);
            if(c.to_string() != "83810205") {
                printError("mult() with positive numbers", "Expected 83810205, got " + c.to_string());
                allPassed = false;
            } else {
                printSuccess("mult() with positive numbers");
            }
        }
        {
            BigInteger d("-12345");
            BigInteger e("6789");
            BigInteger f = d.mult(e);
            if(f.to_string() != "-83810205") {
                printError("mult() with mixed sign (-12345 * 6789)", "Expected -83810205, got " + f.to_string());
                allPassed = false;
            } else {
                printSuccess("mult() with mixed sign (-12345 * 6789)");
            }
        }
        {
            BigInteger g("-12345");
            BigInteger h("-6789");
            BigInteger i = g.mult(h);
            if(i.to_string() != "83810205") {
                printError("mult() with two negatives (-12345 * -6789)", "Expected 83810205, got " + i.to_string());
                allPassed = false;
            } else {
                printSuccess("mult() with two negatives (-12345 * -6789)");
            }
        }
        {
            BigInteger j("12345");
            BigInteger k("0");
            BigInteger l = j.mult(k);
            if(l.to_string() != "0") {
                printError("mult() with zero", "Expected 0, got " + l.to_string());
                allPassed = false;
            } else {
                printSuccess("mult() with zero");
            }
        }
    }

    // ***** Overloaded Operators *****
    cout << "\nTesting Overloaded Operators..." << endl;
    {
        {
            BigInteger a("1000");
            BigInteger b("500");
            BigInteger c = a + b;
            if(c.to_string() != "1500") {
                printError("operator+ (1000 + 500)", "Expected 1500, got " + c.to_string());
                allPassed = false;
            } else {
                printSuccess("operator+ (1000 + 500)");
            }
        }
        {
            BigInteger a("1000");
            BigInteger b("500");
            BigInteger d = a - b;
            if(d.to_string() != "500") {
                printError("operator- (1000 - 500)", "Expected 500, got " + d.to_string());
                allPassed = false;
            } else {
                printSuccess("operator- (1000 - 500)");
            }
        }
        {
            BigInteger a("1000");
            BigInteger b("500");
            BigInteger e = a * b;
            if(e.to_string() != "500000") {
                printError("operator* (1000 * 500)", "Expected 500000, got " + e.to_string());
                allPassed = false;
            } else {
                printSuccess("operator* (1000 * 500)");
            }
        }
    }
    
    // ***** Compound Assignment Operators *****
    cout << "\nTesting Compound Assignment Operators..." << endl;
    {
        {
            BigInteger f("200");
            f += BigInteger("300");
            if(f.to_string() != "500") {
                printError("operator+= (200 += 300)", "Expected 500, got " + f.to_string());
                allPassed = false;
            } else {
                printSuccess("operator+= (200 += 300)");
            }
        }
        {
            BigInteger f("1000");
            f -= BigInteger("400");
            if(f.to_string() != "600") {
                printError("operator-= (1000 -= 400)", "Expected 600, got " + f.to_string());
                allPassed = false;
            } else {
                printSuccess("operator-= (1000 -= 400)");
            }
        }
        {
            BigInteger f("20");
            f *= BigInteger("30");
            if(f.to_string() != "600") {
                printError("operator*= (20 *= 30)", "Expected 600, got " + f.to_string());
                allPassed = false;
            } else {
                printSuccess("operator*= (20 *= 30)");
            }
        }
    }
    
    // ***** Comparison Operators *****
    cout << "\nTesting Comparison Operators..." << endl;
    {
        BigInteger g("100");
        BigInteger h("200");
        BigInteger i("100");
        if(!(g == i)) {
            printError("operator==", "Expected 100 == 100");
            allPassed = false;
        } else {
            printSuccess("operator== (100 == 100)");
        }
        if(!(g < h)) {
            printError("operator<", "Expected 100 < 200");
            allPassed = false;
        } else {
            printSuccess("operator< (100 < 200)");
        }
        if(!(g <= h)) {
            printError("operator<=", "Expected 100 <= 200");
            allPassed = false;
        } else {
            printSuccess("operator<= (100 <= 200)");
        }
        if(!(h > g)) {
            printError("operator>", "Expected 200 > 100");
            allPassed = false;
        } else {
            printSuccess("operator> (200 > 100)");
        }
        if(!(h >= g)) {
            printError("operator>=", "Expected 200 >= 100");
            allPassed = false;
        } else {
            printSuccess("operator>= (200 >= 100)");
        }
    }

    cout << "\nAll tests " << (allPassed ? "PASSED" : "did not pass") << endl;
    return 0;
}