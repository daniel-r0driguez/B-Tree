#ifndef BTREENODE_H
#define BTREENODE_H
#include <vector>
#include <utility>

/**
 * Node class for BTrees.
 * @tparam KeyType key data type
 * @tparam ValueType value data type
 */
template<typename KeyType, typename ValueType>
struct BTreeNode {
    std::vector<std::pair<KeyType, ValueType>> keysValues;
    std::vector<BTreeNode<KeyType, ValueType>*> subsets;

    /**
     * Default constructor.
     */
    BTreeNode();

    /**
     * Constructor that takes in a collection of key/value pairs.
     * @param keyValues the std::vector of key/value pairs to construct the BTree
     * node with
     */
    explicit BTreeNode(const std::vector<std::pair<KeyType, ValueType>> &keysValues);
};

#include <BTREE/detail/BTreeNode.tpp>

#endif // BTREENODE_H