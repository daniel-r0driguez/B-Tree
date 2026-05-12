#ifndef CS008_LABS_BTREENODE_H
#define CS008_LABS_BTREENODE_H
#include <vector>
#include <utility>

template<typename KeyType, typename ValueType>
struct BTreeNode {
    std::vector<std::pair<KeyType, ValueType>> keysValues;
    std::vector<BTreeNode<KeyType, ValueType>*> subsets;
    BTreeNode();
    explicit BTreeNode(const std::vector<std::pair<KeyType, ValueType>> &keysValues);
};

#include <BTREE/detail/BTreeNode.tpp>

#endif //CS008_LABS_BTREENODE_H