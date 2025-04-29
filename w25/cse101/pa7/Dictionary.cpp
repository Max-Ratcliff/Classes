/***
* Max Ratcliff
* mwratcli
* 2025 Winter CSE101 PA7
* Dictionary.cpp
* Implementation of Dictionary function with manipulation function
***/ 

#include "Dictionary.h"
#include <sstream>
#include <cassert>
#include <stdexcept>   // Needed for std::logic_error

// Node constructor
Dictionary::Node::Node(keyType k, valType v){
        key = k;
        val = v;
        parent = nullptr;
        left = nullptr;
        right = nullptr;
    }

// Dictionary constructor
Dictionary::Dictionary(){
        nil = nullptr;
        root = nullptr;
        current = nullptr;
        num_pairs = 0;
    }

// Copy constructor
Dictionary::Dictionary(const Dictionary& D){
        nil = nullptr;
        root = nullptr;
        current = nullptr;
        num_pairs = 0;
        preOrderCopy(D.root, D.nil);
}

// Destructor
Dictionary::~Dictionary() {
    clear();
}

// inOrderString()
// Appends a string representation of the tree rooted at R to string s. The
// string appended consists of: "key : value \n" for each key-value pair in
 // tree R, arranged in order by keys.
void Dictionary::inOrderString(std::string& s, Node* R) const {
    if (R != nullptr) {
        inOrderString(s, R->left);
        s += R->key + " : " + std::to_string(R->val) + "\n";
        inOrderString(s, R->right);
    }
}

// preOrderString()
// Appends a string representation of keys (pre-order) to s.
void Dictionary::preOrderString(std::string& s, Node* R) const {
    if (R != nullptr) {
        s += R->key + "\n";
        preOrderString(s, R->left);
        preOrderString(s, R->right);
    }
}

// preOrderCopy()
// Recursively inserts a deep copy of the subtree rooted at R into this 
// Dictionary. Recursion terminates at N.
void Dictionary::preOrderCopy(Node* R, Node* N) {
    if (R != N) {
        setValue(R->key, R->val);
        preOrderCopy(R->left, N);
        preOrderCopy(R->right, N);
    }
}

// postOrderDelete()
// Deletes all nodes in the subtree rooted at R.
void Dictionary::postOrderDelete(Node* R) {
    if (R != nullptr) {
        postOrderDelete(R->left);
        postOrderDelete(R->right);
        delete R;
    }
}

// search()
// Searches for a node with key k in the subtree rooted at R.
// Returns a pointer to the node if found, or nullptr otherwise.
Dictionary::Node* Dictionary::search(Node* R, keyType k) const {
    while (R != nullptr) {
        if (k == R->key)
            return R;
        else if (k < R->key)
            R = R->left;
        else
            R = R->right;
    }
    return nullptr;
}

// findMin()
// Returns a pointer to the node with the smallest key in the subtree rooted at R.
Dictionary::Node* Dictionary::findMin(Node* R) {
    if (R == nullptr)
        return nullptr;
    while (R->left != nullptr)
        R = R->left;
    return R;
}

// findMax()
// Returns a pointer to the node with the largest key in the subtree rooted at R.
Dictionary::Node* Dictionary::findMax(Node* R) {
    if (R == nullptr)
        return nullptr;
    while (R->right != nullptr)
        R = R->right;
    return R;
}

// findNext()
// Returns the in-order successor of node N in the tree.
Dictionary::Node* Dictionary::findNext(Node* N) {
    if (N == nullptr)
        return nullptr;
    if (N->right != nullptr)
        return findMin(N->right);
    Node* y = N->parent;
    while (y != nullptr && N == y->right) {
        N = y;
        y = y->parent;
    }
    return y;
}

// findPrev()
// Returns the in-order predecessor of node N in the tree.
Dictionary::Node* Dictionary::findPrev(Node* N) {
    if (N == nullptr)
        return nullptr;
    if (N->left != nullptr)
        return findMax(N->left);
    Node* y = N->parent;
    while (y != nullptr && N == y->left) {
        N = y;
        y = y->parent;
    }
    return y;
}

// size()
// Returns the number of key-value pairs in the Dictionary.
int Dictionary::size() const {
    return num_pairs;
}

// contains()
// Returns true if the Dictionary contains a key-value pair with key k.
bool Dictionary::contains(keyType k) const {
    return (search(root, k) != nullptr);
}

// getValue()
// Returns a reference to the value associated with key k.
// Pre: contains(k) is true.
// Throws a logic_error if the key does not exist.
valType& Dictionary::getValue(keyType k) const {
    Node* n = search(root, k);
    if(n == nullptr)
        throw std::logic_error("Dictionary: getValue(): key \"" + k + "\" does not exist");
    return n->val;
}

// hasCurrent()
// Returns true if the current iterator is defined.
bool Dictionary::hasCurrent() const {
    return (current != nullptr);
}

// currentKey()
// Returns the key at the current iterator position.
// Pre: hasCurrent() is true.
// Throws a logic_error if current is undefined.
keyType Dictionary::currentKey() const {
    if (!hasCurrent())
        throw std::logic_error("Dictionary: currentKey(): current undefined");
    return current->key;
}

// currentVal()
// Returns a reference to the value at the current iterator position.
// Pre: hasCurrent() is true.
// Throws a logic_error if current is undefined.
valType& Dictionary::currentVal() const {
    if (!hasCurrent())
        throw std::logic_error("Dictionary: currentVal(): current undefined");
    return current->val;
}

// clear()
// Resets the Dictionary to an empty state by deleting all nodes.
void Dictionary::clear() {
    postOrderDelete(root);
    root = nullptr;
    current = nullptr;
    num_pairs = 0;
}

// setValue()
// If a pair with key k exists, updates its value to v.
// Otherwise, inserts a new (k, v) pair into the Dictionary.
void Dictionary::setValue(keyType k, valType v) {
    Node* n = search(root, k);
    if (n != nullptr) {
        n->val = v;
        return;
    }
    // Create a new node
    Node* newNode = new Node(k, v);
    Node* y = nullptr;
    Node* x = root;
    while (x != nullptr) {
        y = x;
        if (k < x->key)
            x = x->left;
        else
            x = x->right;
    }
    newNode->parent = y;
    if (y == nullptr) {
        root = newNode;
    } else if (k < y->key) {
        y->left = newNode;
    } else {
        y->right = newNode;
    }
    num_pairs++;
}

// remove()
// Deletes the pair for which key==k. If that pair is current, current becomes undefined.
// Pre: contains(k) is true.
// Throws a logic_error if the key does not exist.
void Dictionary::remove(keyType k) {
    Node* z = search(root, k);
    if(z == nullptr)
        throw std::logic_error("Dictionary: remove(): key \"" + k + "\" does not exist");
    if (current == z)
        current = nullptr;
    
    // Case 1: z has at most one child
    if (z->left == nullptr || z->right == nullptr) {
        Node* y = z;
        Node* x = (z->left != nullptr) ? z->left : z->right;
        
        // Transplant y with x
        if (y->parent == nullptr) {
            root = x;
        } else if (y == y->parent->left) {
            y->parent->left = x;
        } else {
            y->parent->right = x;
        }
        if (x != nullptr)
            x->parent = y->parent;
        delete y;
    } else {
        // Case 2: z has two children.
        // Find y, the minimum in z->right, then copy y's data to z and remove y.
        Node* y = findMin(z->right);
        z->key = y->key;
        z->val = y->val;
        Node* x = y->right; // y has no left child since it is the minimum of z->right.
        if (y->parent->left == y)
            y->parent->left = x;
        else
            y->parent->right = x;
        if (x != nullptr)
            x->parent = y->parent;
        delete y;
    }
    num_pairs--;
}

// begin()
// Sets the current iterator to the smallest (first) key in the Dictionary.
void Dictionary::begin() {
    if (root != nullptr)
        current = findMin(root);
}

// end()
// Sets the current iterator to the largest (last) key in the Dictionary.
void Dictionary::end() {
    if (root != nullptr)
        current = findMax(root);
}

// next()
// Advances the current iterator to the next key (in in-order sequence).
// Pre: hasCurrent() is true.
// Throws a logic_error if current is undefined.
void Dictionary::next() {
    if (!hasCurrent())
        throw std::logic_error("Dictionary: next(): current undefined");
    current = findNext(current);
}

// prev()
// Moves the current iterator to the previous key (in in-order sequence).
// Pre: hasCurrent() is true.
// Throws a logic_error if current is undefined.
void Dictionary::prev() {
    if (!hasCurrent())
        throw std::logic_error("Dictionary: prev(): current undefined");
    current = findPrev(current);
}

// to_string()
// Returns a string representation of the Dictionary (in-order traversal).
std::string Dictionary::to_string() const {
    std::string s;
    inOrderString(s, root);
    return s;
}

// pre_string()
// Returns a string consisting of keys from a pre-order traversal.
std::string Dictionary::pre_string() const {
    std::string s;
    preOrderString(s, root);
    return s;
}

// equals()
// Returns true if and only if this Dictionary contains the same key-value pairs as D.
bool Dictionary::equals(const Dictionary& D) const {
    if (num_pairs != D.num_pairs)
        return false;
    return (to_string() == D.to_string());
}

// Overloaded operator<<()
// Inserts the string representation of Dictionary D into the stream.
std::ostream& operator<<(std::ostream& stream, Dictionary& D) {
    stream << D.to_string();
    return stream;
}

// Overloaded operator==()
// Returns true if and only if Dictionary A equals Dictionary B.
bool operator==(const Dictionary& A, const Dictionary& B) {
    return A.equals(B);
}

// Overloaded operator=()
// Assigns Dictionary D to this Dictionary.
Dictionary& Dictionary::operator=(const Dictionary& D) {
    if (this != &D) {
        clear();
        preOrderCopy(D.root, D.nil);
    }
    return *this;
}