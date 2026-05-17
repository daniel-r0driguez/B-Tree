#ifndef BTREENODE_TPP
#define BTREENODE_TPP
#include <BTREE/BTreeNode.h>

template<typename KeyType, typename ValueType>
BTreeNode<KeyType, ValueType>::BTreeNode()
: BTreeNode(std::vector<KeyType>(), std::vector<ValueType>())
{}

template<typename KeyType, typename ValueType>
BTreeNode<KeyType, ValueType>::BTreeNode(const std::vector<std::pair<KeyType, ValueType>> &keysValues)
{
    this->keysValues = keysValues;
    this->subsets.reserve(keysValues.size() + 1);
}
#endif // BTREENODE_TPP