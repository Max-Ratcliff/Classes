/***
* Max Ratcliff
* mwratcli
* 2025 Winter CSE101 PA6
* BigInteger.cpp
* Implementation of Big Integer ADT with manipulation functions
***/ 

#include"BigInteger.h"
#include <iostream>
#include <string>
#include <stdexcept>
#include <cmath>

// Consts
const int P = 9;
const long BASE = static_cast<long>(pow(10, P));

// Class Constructors & Destructors ----------------------------------------

// BigInteger()
// Constructor that creates a new BigInteger in the zero state: 
// signum=0, digits=().
BigInteger::BigInteger(){
    signum = 0;
    digits = List();
}
// BigInteger()
// Constructor that creates a new BigInteger from the long value x.
BigInteger::BigInteger(long x){
    // if the long is 0 the list and signum should be 0
    if (x == 0) {
        signum = 0;
        // digits.insertAfter(0);
        return;
    }

    // if x < 0 set the signum and make x positive
    if (x < 0) {
        signum = -1;
        x = abs(x);
    } else {
        signum = 1;
    }

    while (x > 0) {
        digits.insertBefore(x % BASE);
        x /= BASE;
    }
}

// helper
void trim(List* L) {
    L->moveFront();
    while (L->length() > 0 && L->peekNext() == 0) { // as long as the first element is 0 delete it
        L->eraseAfter();
    }
}

// BigInteger()
// Constructor that creates a new BigInteger from the string s.
// Pre: s is a non-empty string consisting of (at least one) base 10 digit
// {0,1,2,3,4,5,6,7,8,9}, and an optional sign {+,-} prefix.
BigInteger::BigInteger(std::string s){
    int length = s.length(); // cache to avoid unecissary length calls
    if(length == 0){ // the string is empty
        throw std::invalid_argument("BigInteger: Constructor: empty string");
    }else if(length == 1 && s[0] == 0){
        signum = 0;
        digits = List();
        return;
    }

    int i = 0; // start of the numeric part of the string

    // deal with signum
    if(s[0] == '-'){
        signum = -1;
        i = 1; // skip the sign prefix
    }else{
        signum = 1;
        if(s[0] == '+'){
            i = 1; // skip the sign prefix
        }
    }

    if (i != 0){
        s = s.substr(i); // adjust to exclude the sign prefix
        length = s.length(); // update length based on new substring

        if(length == 0){ // if there are no digits in the number after extracting the sign
            throw std::invalid_argument("BigInteger: Constructor: non-numeric string");
        }
    }
    
    // process string
    size_t remaining = length;
    std::string chunk = "";
    size_t pos;
    for(i = 0; i < length / P; i++){
        chunk = s.substr((remaining - P), P); // grab the last two digits
        try{
            digits.insertAfter(std::stol(chunk, &pos, 10)); // try to convert them to long
            if(pos != chunk.size()){
                throw std::invalid_argument("BigInteger: Constructor: non-numeric string");
            }
        } catch (const std::invalid_argument& e){
            throw std::invalid_argument("BigInteger: Constructor: non-numeric string"); // throw an error if It is non numeric
        }
        remaining -= P; // decrement remainder
    }
    if(remaining > 0){ // remainder should be in the range (0, P)
        chunk = s.substr(0, remaining);
        try{
            digits.insertAfter(std::stol(chunk, &pos, 10)); // try to convert them to long
            if(pos != chunk.size()){
                throw std::invalid_argument("BigInteger: Constructor: non-numeric string");
            }
        } catch (const std::invalid_argument& e){
            throw std::invalid_argument("BigInteger: Constructor: non-numeric string"); // throw an error if It is non numeric
        }
    }
    trim(&digits);
}

// BigInteger()
// Constructor that creates a copy of N.
BigInteger::BigInteger(const BigInteger& N){
    signum = N.sign();
    digits = List(N.digits);
}

// Optional Destuctor
// ~BigInteger()
// ~BigInteger(){}


// Access functions --------------------------------------------------------

// sign()
// Returns -1, 1 or 0 according to whether this BigInteger is positive, 
// negative or 0, respectively.
int BigInteger::sign() const{
    return signum;
}

// compare()
// Returns -1, 1 or 0 according to whether this BigInteger is less than N,
// greater than N or equal to N, respectively.
int BigInteger::compare(const BigInteger& N) const{
    if(signum > N.signum){ // if the signum is bigger than the number has to be bigger
        return 1;
    }else if(signum < N.signum){
        return -1;
    }else if(signum == 0 && N.signum == 0){
        return 0; // both numbers == 0
    }//else

    // copy digit list to avoid accidental modifications
    List A = digits;
    List B = N.digits;

    // we can be sure signum == N.signum
    if(A.length() > B.length()){ // if A has more digits it's bigger
        return 1;
    }else if(A.length() < B.length()){
        return -1;
    }

    // we can be sure A.length() == B.length()
    A.moveFront();
    B.moveFront();
    while(A.position() < A.length()){
        // start at the most significant digit
        if(A.peekNext() > B.peekNext()){
            return 1;
        }else if(A.peekNext() < B.peekNext()){
            return -1;
        }
        A.moveNext();
        B.moveNext();
    }
    return 0; // if they stay equal throughout the traversal they are equal
}


// Manipulation procedures -------------------------------------------------

// makeZero()
// Re-sets this BigInteger to the zero state.
void BigInteger::makeZero(){
    digits.clear();
    signum = 0;
}

// negate()
// If this BigInteger is zero, does nothing, otherwise reverses the sign of 
// this BigInteger positive <--> negative. 
void BigInteger::negate(){
    signum *= -1;
}


// BigInteger Arithmetic operations ----------------------------------------
// BigInteger BigInteger::add(const BigInteger& N) const{
//     BigInteger lhs = *this

// }

// add()
// Returns a BigInteger representing the sum of this and N.
BigInteger BigInteger::add(const BigInteger& N) const{
    // Copy the current BigInteger and the one being added
    BigInteger lhs = *this;
    BigInteger rhs = N;
    BigInteger result;

    // If lhs is positive and rhs is negative, negate rhs and perform subtraction
    if (lhs.sign() > 0 && rhs.sign() < 0) {
        rhs.negate();  // Negate rhs
        return lhs.sub(rhs);  // Subtract the negated rhs from lhs
    }
    // If lhs is negative and rhs is positive, negate lhs and perform subtraction
    else if (lhs.sign() < 0 && rhs.sign() > 0) {
        lhs.negate();  // Negate lhs
        return rhs.sub(lhs);  // Subtract the negated lhs from rhs
    }
    // If both lhs and rhs are negative, negate both and add them
    else if (lhs.sign() < 0 && rhs.sign() < 0) {
        lhs.negate();  
        rhs.negate();  
        result = lhs.add(rhs);  
        result.negate();  // Negate the result to return a negative value
        return result;
    }

    // Normalize the smaller of the two operands by putting the larger operand first
    if (lhs > rhs) {
        return rhs.add(lhs);  // Ensure lhs is always smaller
    }

    // Convert both operands into Lists of digits for base 100 (as described)
    List x = lhs.digits;
    List y = rhs.digits;
    List ans = result.digits;

    long carry = 0;  
    long temp = 0;   

    // begin addition at the least significant digit
    x.moveBack();  
    y.moveBack();  

    // Add digits from right to left
    while (x.position() > 0 && y.position() > 0) {
        // Sum the corresponding digits from both operands and the carry
        temp = carry + x.movePrev() + y.movePrev();
        
        // Compute the carry for the next step
        carry = temp / BASE; // will truncate to an int 
        temp %= BASE;  // Update temp to hold only the current digit
        
        ans.insertAfter(temp);
    }

    // If there are remaining digits in rhs, add them to the result
    while (y.position() > 0) {
        temp = carry + y.movePrev();

        carry = temp / BASE;
        temp %= BASE;

        ans.insertAfter(temp);
    }

    // If there's any carry left after processing all digits, insert it
    if (carry > 0) {
        ans.insertAfter(carry);
    }

    // Set the result's sign to positive (since both operands are positive)
    result.signum = 1;
    result.digits = ans;

    return result;  // Return the sum as a BigInteger
}    

// sub()
// Returns a BigInteger representing the difference of this and N.
BigInteger BigInteger::sub(const BigInteger& N) const {
    BigInteger B = *this;
    BigInteger A = N;
    BigInteger C;
    List a = A.digits;
    List b = B.digits;
    List c = C.digits;

    if (A == B) {
        return C; 
    }
    if (A.sign() && !B.sign()) {
        A.negate();
        return A;
    }
    if (!A.sign() && B.sign()) {
        return B;
    }
    if (A.sign() < 0 && B.sign() > 0) {
        B.negate();
        C = A.add(B);
        C.negate();
        return C;
    }
    if (A.sign() > 0 && B.sign() < 0) {
        A.negate();
        C = A.add(B);
        return C;
    }
    if (A.sign() < 0 && B.sign() < 0) {
        A.negate();
        B.negate();
        C = B.sub(A);
        C.negate();
        return C;
    }
    if (A < B) {
        C = A.sub(B);
        C.negate();
        return C;
    }
    a.moveBack();
    b.moveBack();
    long dist = 0;
    long temp = 0;
    int i = b.position();
    while (!(i <= 0)) {
        if (a.peekPrev() - dist < b.peekPrev()) {
            temp = a.peekPrev() + BASE - b.peekPrev() - dist;
            dist = 1;
        } else {
            temp = a.peekPrev() - dist - b.peekPrev();
            if (!(a.peekPrev() <= 0)) {
                dist = 0;
            }
        }
        c.insertAfter(temp);
        a.movePrev();
        b.movePrev();
        i--;
    }
    while (a.position() > 0) {
        if (a.peekPrev() - dist < 0) {
            temp = a.peekPrev() + BASE - 0 - dist;
            dist = 1;
        } else {
            temp = a.peekPrev() - dist - 0;
            if (!(a.peekPrev() <= 0)) {
                dist = 0;
            }
        }
        c.insertAfter(temp);
        a.movePrev();
    }
    C.digits = c;
    trim(&(C.digits));
    C.signum = -1;
    return C;
}


List tempMult(long s, List *b, int* ctr) {
    List L;
    long carry = 0;
    long temp = 0;
    for (b->moveBack(); b->position() > 0; b->movePrev()) {
        temp = (b->peekPrev() * s) + carry;
        carry = temp / BASE;
        temp %= BASE;
        L.insertAfter(temp);
    }
    if (carry > 0) {
        L.insertAfter(carry);
    }
    L.moveBack();
    for (int i = 0; i < *ctr; i++) {
        L.insertAfter(0);
    }
    return L;
}


// mult()
// Returns a BigInteger representing the product of this and N. 
BigInteger BigInteger::mult(const BigInteger& N) const{
    BigInteger lhs = *this;
    BigInteger rhs = N;
    BigInteger result;
    List x = rhs.digits;
    List y = lhs.digits;
    int ctr = 0;
    x.moveBack();
    y.moveBack();
    int p = x.position();
    for (int i = p; i > 0; i--) {
        List temp = tempMult(x.peekPrev(), &y, &ctr);
        BigInteger T;
        T.signum = 1;
        T.digits = temp;
        result = result.add(T);
        x.movePrev();
        ctr++;
    }
    result.signum = rhs.signum * lhs.signum;
    return result;
}


// Other Functions ---------------------------------------------------------

// to_string()
// Returns a string representation of this BigInteger consisting of its
// base 10 digits. If this BigInteger is negative, the returned string 
// will begin with a negative sign '-'. If this BigInteger is zero, the
// returned string will consist of the character '0' only.
std::string BigInteger::to_string(){
    std::string out = "";
    if (signum == 0 || digits.length() == 0) { // if there are no digits then just return "0"
        return "0";
    }
    
    if (signum == -1) { // if its a negative number prepend the sign
        out = "-";
    }

    digits.moveFront();
    while (digits.front() == 0 && digits.length() > 1) { // clear leading zeros
        digits.eraseAfter();
    }

    digits.moveFront(); 
    while(digits.position() < digits.length()) {
        std::string A = std::to_string(digits.peekNext());
        std::string B = "";
        while ((int)(B.length() + A.length()) < P && digits.position() != 0) {
            B += '0';
        }
        out += (B + A);
        digits.moveNext();
    }
    return out;
}


// Overriden Operators -----------------------------------------------------

// operator<<()
// Inserts string representation of N into stream.
std::ostream& operator<<( std::ostream& stream, BigInteger N ){
    return stream<<N.BigInteger::to_string();
}

// operator==()
// Returns true if and only if A equals B. 
bool operator==( const BigInteger& A, const BigInteger& B ){
    return (A.compare(B) == 0);
}

// operator<()
// Returns true if and only if A is less than B. 
bool operator<( const BigInteger& A, const BigInteger& B ){
    return (A.compare(B) == -1);
}

// operator<=()
// Returns true if and only if A is less than or equal to B. 
bool operator<=( const BigInteger& A, const BigInteger& B ){
    return (A < B || A == B);
}

// operator>()
// Returns true if and only if A is greater than B. 
bool operator>( const BigInteger& A, const BigInteger& B ){
    return (A.compare(B) == 1);
}

// operator>=()
// Returns true if and only if A is greater than or equal to B. 
bool operator>=( const BigInteger& A, const BigInteger& B ){
    return (A > B || A == B);
}

// operator+()
// Returns the sum A+B. 
BigInteger operator+( const BigInteger& A, const BigInteger& B ){
    return A.add(B);
}

// operator+=()
// Overwrites A with the sum A+B. 
BigInteger operator+=( BigInteger& A, const BigInteger& B ){
    A = A + B;
    return A;
}

// operator-()
// Returns the difference A-B. 
BigInteger operator-( const BigInteger& A, const BigInteger& B ){
    return A.sub(B);
}

// operator-=()
// Overwrites A with the difference A-B. 
BigInteger operator-=( BigInteger& A, const BigInteger& B ){
    A = A - B;
    return A;
}

// operator*()
// Returns the product A*B. 
BigInteger operator*( const BigInteger& A, const BigInteger& B ){
    return A.mult(B);
}

// operator*=()
// Overwrites A with the product A*B. 
BigInteger operator*=( BigInteger& A, const BigInteger& B ){
    A = A*B;
    return A;
}
