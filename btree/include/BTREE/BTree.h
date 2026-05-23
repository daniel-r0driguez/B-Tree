#ifndef BTREE_H
#define BTREE_H
#include <BTREE/BTreeNode.h>
#include <iostream>
#include <iomanip>

/**
 * BTree Class
 * @tparam KeyType key data type
 * @tparam ValueType value data type
 * @tparam MAX_CHILDREN max amount of children for each node
 */
template<typename KeyType, typename ValueType, int MAX_CHILDREN = 3>
class BTree {
public:
    enum OutputMode{HIERARCHY, SIDEWAYS, OUTPUT_COUNT};
    static int ALLOCATIONS;
    static int DEALLOCATIONS;

private:
    const int MAX_KEYS;
    const int MIN_KEYS;
    BTreeNode<KeyType, ValueType> *_root;
    OutputMode _outputMode;
    int _keyCount;

public:
    //////////////////////////////////////////////////////////////
    //   Constructors / Assignment Operator / Deconstructor     //
    //////////////////////////////////////////////////////////////
    /**
     * Default constructor
     */
    BTree();

    /**
     * Copy constructor
     * Copies the content of another BTree.
     */
    BTree(const BTree<KeyType, ValueType> &btree);

    /**
     * Assignment operator
     * Copies the content of another BTree.
     */
    BTree& operator = (const BTree<KeyType, ValueType> &btree);

    /**
     * Deconstructor
     * Empties the content of this BTree.
     */
    ~BTree();

    /////////////////
    //   Getters   //
    /////////////////
    /**
     * Checks if the BTree is empty.
     * @return true if the BTree is empty, false otherwise
     */
    bool empty() const;
    
    /**
     * Finds and returns the data associated with passed in key.
     * @param key the key to search for
     * @return a const pointer to the data
     */
    const ValueType* find(const KeyType &key) const;

    /**
     * Returns the key count
     * @return the key count of the BTree
     */
    int keyCount() const;


    ///////////////////
    //   Modifiers   //
    ///////////////////
    /**
     * Removes the key/value pair with the specific key.
     * @param key the key to search for
     * @return true if the removal was successful, false otherwise
     */
    bool remove(const KeyType &key);

    /**
     * Empties the content of the BTree.
     */
    void clear();

    /**
     * Inserts a new key/value pair into the BTree for storage.
     * @param keyValue the key/value pair to insert
     */
    void insert(const std::pair<KeyType, ValueType> &keyValue);

    ///////////////////////////////////////
    //   Printing and Output functions   //
    ///////////////////////////////////////
    /**
     * Returns the output mode of the BTree.
     * @return the output mode of the BTree when fed into a std::ostream object
     */
    OutputMode getOutputMode() const;

    /**
     * Sets the output mode of the BTree.
     * @param mode the desired output mode when fed into a std::ostream object
     */
    void setOutputMode(BTree::OutputMode mode);

    /**
     * Overloaded << operator for the BTree class.
     * @param out the std::ostream derived object to feed the BTree into
     * @tparam T the key data type
     * @tparam Q the value data type
     * @tparam max the max amount of children per node
     */
    template<typename T, typename Q, int max>
    friend std::ostream& operator << (std::ostream &out, const BTree<T, Q, max> &btree);

private:
    /**
     * Increments the key count of the BTree.
     */
    void incrementKeyCount();

    /**
     * Decreases the key count of the BTree.
     */
    void decrementKeyCount();

    /**
     * Creates a brand new node.
     * @param keysValues the key/value pairs (children) to give to the newly created BTreeNode
     * @return returns a pointer to the newly created BTreeNode
     */
    BTreeNode<KeyType, ValueType>* createNode(const std::vector<std::pair<KeyType, ValueType>> &keysValues);

    /**
     * Deletes the node.
     * @param node a pointer to the BTreeNode to delete
     */
    void deleteNode(BTreeNode<KeyType, ValueType> *node);

    ////////////////////////
    // Printing Functions //
    ////////////////////////
    /**
     * Prints the BTree in Hierarchy mode.
     * @param root a BTreeNode which represents the root
     * @param depthCount the depth count of the BTree
     * @param out the std::ostream derived object to feed the BTree data into
     */
    void printInHierarchy(BTreeNode<KeyType, ValueType> *root, int depthCount, std::ostream &out) const;

    /**
     * Prints the BTree in Sideways mode.
     * @param root a BTreeNode which represents the root
     * @param depthCount the depth count of the BTree
     * @param out the std::ostream derived object to feed the BTree data into
     */
    void printSideways(BTreeNode<KeyType, ValueType> *root, int depthCount, std::ostream &out) const;

    ///////////////////////
    // Finding Functions //
    ///////////////////////
    /**
     * Recursively looks for the key/value pair using the passed key.
     * @param root a BTreeNode which represents the current root
     * @param key the key being searched for
     * @return returns a pointer to the found data, if nothing was found it returns nullptr
     */
    ValueType* recursiveFind(BTreeNode<KeyType, ValueType> *root, const KeyType &key) const;

    /**
     * Traverses the children of the root node to search for the specific passed in key.
     * @param root the current root node whose children will be traversed through
     * @param key the key to search for
     * @return the index of the key/value pair in the children array
     */
    int find_ge(BTreeNode<KeyType, ValueType> *root, const KeyType &key) const;

    /////////////////////////////////////
    // Insertion (and helper) functions
    /////////////////////////////////////
    /**
     * Creates a new root node.
     * @param keysValues the key/value pair to store in the root node
     */
    void createRoot(const std::pair<KeyType, ValueType> &keysValues);

    /**
     * Recursive function that looks for the correct location to store the new key/value pair.
     * @param root the current root node
     * @param keyValue the key/value pair to loosely insert
     * 
     */
    void loose_insert(BTreeNode<KeyType, ValueType> *root, const std::pair<KeyType, ValueType> &keyValue);

    /**
     * Splits a child node of the parent node.
     * @param parent the parent node of the child node
     * @param subset_index the index of the child node to split 
     */
    void splitNode(BTreeNode<KeyType, ValueType> *parent, int subset_index);

    /**
     * Grows the tree when the root node has excess children.
     */
    void growTree();

    /**
     * Recursive function that trickles down to the leaf nodes in order to delete a node with the specific key.
     * @param root the current root
     * @param key the key to search for and delete
     * @return true if the removal was successful, false otherwise
     */
    bool loose_remove(BTreeNode<KeyType, ValueType> *root, const KeyType &key);

    /**
     * Fixes the shortage of children nodes (when < MIN_KEYS).
     * @param parent the parent node to fix
     * @param subsetIndex the index of the child that was deleted
     */
    void fix_shortage(BTreeNode<KeyType, ValueType> *parent, int subsetIndex);

    /**
     * Attempts to rotate the key/value from the left sibling.
     * @param parent the parent to attempt the rotation on
     * @param subsetIndex the index to rotate from
     * @return true if the rotation was successful, false otherwise
     */
    bool rotate_left(BTreeNode<KeyType, ValueType> *parent, int subsetIndex);

    /**
     * Attempts to rotate the key/value from the right sibling.
     * @param parent the parent to attempt the rotation on
     * @param subsetIndex the index to rotate from
     * @return true if the rotation was successful, false otherwise
     */
    bool rotate_right(BTreeNode<KeyType, ValueType> *parent, int subsetIndex);

    /**
     * Merge the parent node into it's left subtree.
     * @param parent the parent node to merge
     * @param subsetIndex the index of the subset
     */
    void merge_left(BTreeNode<KeyType, ValueType> *parent, int subsetIndex);

    /**
     * Shrinks the BTree.
     */
    void shrinkTree();

    /**
     * Recursively clears the contents of each BTreeNode.
     * @param root the current BTreeNode whose content will be deleted
     */
    void clearSubset(BTreeNode<KeyType, ValueType> *root);

    /**
     * Finds the predecessor of the passed in root node.
     * @param root the current BTreeNode root
     * @return the predecessor key/value pair (largest number less than the root's key)
     */
    std::pair<KeyType, ValueType>& findPredecessor(BTreeNode<KeyType, ValueType> *root);

    /**
     * Recursively copies the content of another BTree using only BTreeNodes.
     * @param root the current BTreeNode root to copy from
     */
    BTreeNode<KeyType, ValueType>* copyTree(const BTreeNode<KeyType, ValueType> *root);

};

#include <BTREE/detail/BTree.tpp>

#endif // BTREE_H