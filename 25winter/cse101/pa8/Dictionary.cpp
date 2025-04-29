/***
* Max Ratcliff
* mwratcli
* 2025 Winter CSE101 PA8
* Dictionary.cpp
* Implementation of Dictionary function with manipulation function
***/ 

#include "Dictionary.h"
#include <sstream>
#include <cassert>
#include <stdexcept>   // Needed for std::logic_error

#define RED 1
#define BLACK 0

// Node constructor
Dictionary::Node::Node(keyType k, valType v){
        key = k;
        val = v;
        parent = nullptr;
        left = nullptr;
        right = nullptr;
        color = BLACK;
    }

// Dictionary constructor
Dictionary::Dictionary(){
        //init nil node
        nil = new Node("nil", 0);
        nil->color = BLACK;
        nil->parent = nil;
        nil->left = nil;
        nil->right = nil;

        root = nil;
        current = nil;
        num_pairs = 0;
    }

// Copy constructor
Dictionary::Dictionary(const Dictionary& D){
        //init nil node
        nil = new Node("nil", 0);
        nil->color = BLACK;
        nil->parent = nil;
        nil->left = nil;
        nil->right = nil;

        root = nil;
        current = nil;
        num_pairs = 0;
        preOrderCopy(D.root, D.nil);
}

// Destructor
Dictionary::~Dictionary() {
    clear();
    delete nil;
}

// inOrderString()
// Appends a string representation of the tree rooted at R to string s. The
// string appended consists of: "key : value \n" for each key-value pair in
// tree R, arranged in order by keys.
void Dictionary::inOrderString(std::string& s, Node* R) const {
    if (R != nil) {
        inOrderString(s, R->left);
        s += R->key + " : " + std::to_string(R->val) + "\n";
        inOrderString(s, R->right);
    }
}

// preOrderString()
// Appends a string representation of the tree rooted at R to s. The appended
// string will consist of keys only, with the order determined by a pre-order
// tree walk. The keys stored in black Nodes will be appended as "key\n", and
// the keys stored in red Nodes will be appended as "key (RED)\n". The output 
// of this function is demonstrated in /Examples/pa8/DictionaryClient-out.  
void Dictionary::preOrderString(std::string& s, Node* R) const {
    if (R != nil) {
        s += R->key;
        if(R->color == RED){
            s += " (RED)\n";
        }else{
            s += "\n";
        }
        preOrderString(s, R->left);
        preOrderString(s, R->right);
    }
}


// BST_insert()
// Inserts a copy of the Node *M into this Dictionary. Used by preOrderCopy().
void Dictionary::BST_insert(Node* z){
    Node* y = nil;
    Node* x = root;
    while (x != nil){
        y = x;
        if (z->key < x->key){
            x = x->left;
        }else{
            x = x->right;
        }
    }

    z->parent = y;

    if (y == nil){
        root = z;
    }else if (z->key < y->key){
        y->left = z;
    }else{
        y->right = z;
    }
    z->left = nil;
    z->right = nil;
    z->color = RED;
    RB_InsertFixUp(z);
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
    if (R != nil) {
        postOrderDelete(R->left);
        postOrderDelete(R->right);
        delete R;
    }
}

// search()
// Searches the subtree rooted at R for a Node with key==k. Returns
// the address of the Node if it exists, returns nil otherwise.
Dictionary::Node* Dictionary::search(Node* R, keyType k) const {
    while (R != nil) {
        if (k == R->key)
            return R;
        else if (k < R->key)
            R = R->left;
        else
            R = R->right;
    }
    return nil;
}

// findMin()
// If the subtree rooted at R is not empty, returns a pointer to the 
// leftmost Node in that subtree, otherwise returns nil.
Dictionary::Node* Dictionary::findMin(Node* R) {
    if (R == nil)
        return nil;
    while (R->left != nil)
        R = R->left;
    return R;
}

// findMax()
// If the subtree rooted at R is not empty, returns a pointer to the 
// rightmost Node in that subtree, otherwise returns nil.
Dictionary::Node* Dictionary::findMax(Node* R) {
    if (R == nil)
        return nil;
    while (R->right != nil)
        R = R->right;
    return R;
}

// findNext()
// If N does not point to the rightmost Node, returns a pointer to the
// Node after N in an in-order tree walk.  If N points to the rightmost 
// Node, or is nil, returns nil. 
Dictionary::Node* Dictionary::findNext(Node* N) {
    if (N == nil)
        return nil;
    if (N->right != nil)
        return findMin(N->right);
    Node* y = N->parent;
    while (y != nil && N == y->right) {
        N = y;
        y = y->parent;
    }
    return y;
}

// findPrev()
// If N does not point to the leftmost Node, returns a pointer to the
// Node before N in an in-order tree walk.  If N points to the leftmost 
// Node, or is nil, returns nil.
Dictionary::Node* Dictionary::findPrev(Node* N) {
    if (N == nil)
        return nil;
    if (N->left != nil)
        return findMax(N->left);
    Node* y = N->parent;
    while (y != nil && N == y->left) {
        N = y;
        y = y->parent;
    }
    return y;
}

// RBT Helper Functions (Optional) -----------------------------------------

// LeftRotate()
void Dictionary::LeftRotate(Node* x){
    //set y
    Node* y = x->right;

    // turn y's left subtree into x's right subtree
    x->right = y->left; 
    if (y->left != nil)  // not necessary if using sentinel nil node
        y->left->parent = x;
    
    // link y's parent to x
    y->parent = x->parent;
    if (x->parent == nil){
        root = y;
    }else if (x == x->parent->left){
        x->parent->left = y;
    }else{ 
        x->parent->right = y;
    }
    
    // put x on y's left
    y->left = x;
    x->parent = y;

}

// RightRotate()
void Dictionary::RightRotate(Node* x){;
    // set y;
    Node* y = x->left ;

    // turn y's right subtree into x's left subtree;
    x->left = y->right ;
    if (y->right != nil){  // not necessary if using sentinel nil node
        y->right->parent = x;
    }

    // link y's parent to x;
    y->parent = x->parent ;
    if (x->parent == nil){
        root = y;
    }else if (x == x->parent->right){
        x->parent->right = y;
    }else{ 
        x->parent->left = y;
    }
    // put x on y's right;
    y->right = x;
    x->parent = y;
};

// RB_InsertFixUP()
// Ensures that the Red-Black coloring is consistent
void Dictionary::RB_InsertFixUp(Node* z){
    Node* y = nil;
    while(z->parent->color == RED){
        if(z->parent == z->parent->parent->left){
            y = z->parent->parent->right;
            if(y->color == RED){
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }else{
                if(z == z->parent->right){
                    z = z->parent;
                    LeftRotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                RightRotate(z->parent->parent);
            }
        }else{
            y = z->parent->parent->left;
            if (y->color == RED){
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left){
                    z = z->parent;
                    RightRotate(z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                LeftRotate(z->parent->parent);
            }
        }
    }
    root->color = BLACK;
}

// RB_Transplant()
void Dictionary::RB_Transplant(Node* u, Node* v){
    if (u->parent == nil){
        root = v;
    }else if (u == u->parent->left){
        u->parent->left = v;
    }else{
        u->parent->right = v;
    }
    v->parent = u->parent;
}

// RB_DeleteFixUp()
void Dictionary::RB_DeleteFixUp(Node* x){
    Node* w = nil;
    while(x != root && x->color == BLACK){
        if(x == x->parent->left){
            w = x->parent->right;
            if(w->color==RED){
                // case 1:
                w->color = BLACK;
                x->parent->color = RED;
                LeftRotate(x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK){
                // case 2:
                w->color = RED;
                x = x->parent;
            }else{
                if (w->right->color == BLACK){
                    // case 3:
                    w->left->color = BLACK;
                    w->color = RED;
                    RightRotate(w);
                    w = x->parent->right;
                }
                // case 4:
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                LeftRotate(x->parent);
                x = root;
            }
        }else{
            w = x->parent->left;
            if (w->color == RED){
                // case 5:
                w->color = BLACK;
                x->parent->color = RED;
                RightRotate(x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK){
                // case 6:
                w->color = RED;
                x = x->parent;
            }else{
                if (w->left->color == BLACK){
                    // case 7:
                    w->right->color = BLACK;
                    w->color = RED;
                    LeftRotate(w);
                    w = x->parent->left;
                }
                // case 8:
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                RightRotate(x->parent);
                x = root;
            }
        }
    }
    x->color = BLACK;
}

// RB_Delete()
void Dictionary::RB_Delete(Node* z){
    Node* y = z;
    Node* x = nil;
    int y_og_color = y->color;
    if(z->left == nil){
        x = z->right;
        RB_Transplant(z, z->right);
    }else if(z->right == nil){
        x = z->left;
        RB_Transplant(z, z->left);
    }else{
        y = findMin(z->right);
        y_og_color = y->color;
        x = y->right;
        if(y->parent == z){
            x->parent = y;
        }else{
            RB_Transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        RB_Transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    if(y_og_color == BLACK){
        RB_DeleteFixUp(x);
    }
}

// size()
// Returns the number of key-value pairs in the Dictionary.
int Dictionary::size() const {
    return num_pairs;
}

// contains()
// Returns true if the Dictionary contains a key-value pair with key k.
bool Dictionary::contains(keyType k) const {
    return (search(root, k) != nil);
}

// getValue()
// Returns a reference to the value associated with key k.
// Pre: contains(k) is true.
// Throws a logic_error if the key does not exist.
valType& Dictionary::getValue(keyType k) const {
    Node* n = search(root, k);
    if(n == nil)
        throw std::logic_error("Dictionary: getValue(): key \"" + k + "\" does not exist");
    return n->val;
}

// hasCurrent()
// Returns true if the current iterator is defined.
bool Dictionary::hasCurrent() const {
    return (current != nil);
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
    root = nil;
    current = nil;
    num_pairs = 0;
}

// setValue()
// If a pair with key==k exists, overwrites the corresponding value with v, 
// otherwise inserts the new pair (k, v).
void Dictionary::setValue(keyType k, valType v) {
    Node* n = search(root, k);
    if (n != nil) {
        n->val = v;
        return;
    }
    // Create a new node
    Node* newNode = new Node(k, v);
    newNode->color = RED;
    // insert with helper function
    BST_insert(newNode);
    num_pairs++;
}

// remove()
// Deletes the pair for which key==k. If that pair is current, then current
// becomes undefined.
// Pre: contains(k).
void Dictionary::remove(keyType k) {
    Node* z = search(root, k);
    if(z == nil)
        throw std::logic_error("Dictionary: remove(): key \"" + k + "\" does not exist");
    if (current == z)
        current = nil;
    // delete node with helper function
    RB_Delete(z);
    num_pairs--;
    delete z; // clean up memory
}

// begin()
// If non-empty, places current iterator at the first (key, value) pair
// (as defined by the order operator < on keys), otherwise does nothing. 
void Dictionary::begin() {
    if (root != nil)
        current = findMin(root);
}

// end()
// If non-empty, places current iterator at the last (key, value) pair
// (as defined by the order operator < on keys), otherwise does nothing. 
void Dictionary::end() {
    if (root != nil)
        current = findMax(root);
}

// next()
// If the current iterator is not at the last pair, advances current 
// to the next pair (as defined by the order operator < on keys). If 
// the current iterator is at the last pair, makes current undefined.
// Pre: hasCurrent()
void Dictionary::next() {
    if (!hasCurrent())
        throw std::logic_error("Dictionary: next(): current undefined");
    current = findNext(current);
}

// prev()
// If the current iterator is not at the first pair, moves current to  
// the previous pair (as defined by the order operator < on keys). If 
// the current iterator is at the first pair, makes current undefined.
// Pre: hasCurrent()
void Dictionary::prev() {
    if (!hasCurrent())
        throw std::logic_error("Dictionary: prev(): current undefined");
    current = findPrev(current);
}

// Other Functions ---------------------------------------------------------

// to_string()
// Returns a string representation of this Dictionary. Consecutive (key, value)
// pairs are separated by a newline "\n" character, and the items key and value 
// are separated by the sequence space-colon-space " : ". The pairs are arranged 
// in order, as defined by the order operator <.
std::string Dictionary::to_string() const {
    std::string s;
    inOrderString(s, root);
    return s;
}

// pre_string()
// Returns a string consisting of all keys in this Dictionary. The key order is 
// given by a pre-order tree walk. The keys stored in black Nodes will appear in
// the returned string as "key\n", and the keys stored in red Nodes will appear 
// as "key (RED)\n".  The output of this function is demonstrated in 
// /Examples/pa8/DictionaryClient-out.
std::string Dictionary::pre_string() const {
    std::string s;
    preOrderString(s, root);
    return s;
}

// equals()
// Returns true if and only if this Dictionary contains the same (key, value)
// pairs as Dictionary D.
bool Dictionary::equals(const Dictionary& D) const {
    if (num_pairs != D.num_pairs)
        return false;
    return (to_string() == D.to_string());
}

// Overloaded Operators ----------------------------------------------------

   // operator<<()
   // Inserts string representation of Dictionary D into stream, as defined by
   // member function to_string().
std::ostream& operator<<(std::ostream& stream, Dictionary& D) {
    stream << D.to_string();
    return stream;
}

// operator==()
// Returns true if and only if Dictionary A equals Dictionary B, as defined
// by member function equals(). 
bool operator==(const Dictionary& A, const Dictionary& B) {
    return A.equals(B);
}

// operator=()
// Overwrites the state of this Dictionary with state of D, and returns a
// reference to this Dictionary.
Dictionary& Dictionary::operator=(const Dictionary& D) {
    if (this != &D) {
        clear();
        preOrderCopy(D.root, D.nil);
    }
    return *this;
}