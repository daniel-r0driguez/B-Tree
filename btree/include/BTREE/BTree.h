#ifndef CS008_LABS_BTREE_H
#define CS008_LABS_BTREE_H
#include <BTREE/BTreeNode.h>
#include <iostream>
#include <iomanip>

/**
 * Currently, the BTree can insert keys, delete itself, and copy from other BTrees.
 * @tparam T
 * @tparam MAX_CHILDREN
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
    BTree();
    BTree(const BTree<KeyType, ValueType> &btree);
    BTree& operator = (const BTree<KeyType, ValueType> &btree);
    ~BTree();

    bool empty() const;
    bool remove(const KeyType &key);
    void clear();
    ValueType* find(const KeyType &key) const;
    void insert(const std::pair<KeyType, ValueType> &keyValue);
    int keyCount() const;

    /////////////////////////////////
    // Printing and Output functions
    /////////////////////////////////
    OutputMode getOutputMode() const;
    void setOutputMode(BTree::OutputMode mode);
    template<typename T, typename Q, int max>
    friend std::ostream& operator << (std::ostream &out, const BTree<T, Q, max> &btree);

private:
    void incrementKeyCount();
    void decrementKeyCount();

    BTreeNode<KeyType, ValueType>* createNode(const std::vector<std::pair<KeyType, ValueType>> &keysValues);
    void deleteNode(BTreeNode<KeyType, ValueType> *node);

    ///////////////////////
    // Printing Functions
    ///////////////////////
    void printInHierarchy(BTreeNode<KeyType, ValueType> *root, int depthCount, std::ostream &out) const;
    void printSideways(BTreeNode<KeyType, ValueType> *root, int depthCount, std::ostream &out) const;

    //////////////////////
    // Finding Functions
    //////////////////////
    ValueType* recursiveFind(BTreeNode<KeyType, ValueType> *root, const KeyType &key) const;
    int find_ge(BTreeNode<KeyType, ValueType> *root, const KeyType &key) const;

    /////////////////////////////////////
    // Insertion (and helper) functions
    /////////////////////////////////////
    void createRoot(const std::pair<KeyType, ValueType> &keysValues);
    void loose_insert(BTreeNode<KeyType, ValueType> *root, const std::pair<KeyType, ValueType> &keyValue);
    void splitNode(BTreeNode<KeyType, ValueType> *parent, int subset_index);
    void growTree();

    // returns true if the key was successfully found and removed.
    bool loose_remove(BTreeNode<KeyType, ValueType> *root, const KeyType &key);
    void fix_shortage(BTreeNode<KeyType, ValueType> *parent, int subsetIndex);
    bool rotate_left(BTreeNode<KeyType, ValueType> *parent, int subsetIndex);
    bool rotate_right(BTreeNode<KeyType, ValueType> *parent, int subsetIndex);
    void merge_left(BTreeNode<KeyType, ValueType> *parent, int subsetIndex);
    void shrinkTree();
    void clearSubset(BTreeNode<KeyType, ValueType> *root);
    KeyType& findPredecessor(BTreeNode<KeyType, ValueType> *root);
    BTreeNode<KeyType, ValueType>* copyTree(const BTreeNode<KeyType, ValueType> *root);

};

#include <BTREE/detail/BTree.tpp>

#endif //CS008_LABS_BTREE_H