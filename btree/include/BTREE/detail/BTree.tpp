#ifndef CS008_LABS_BTREE_CPP
#define CS008_LABS_BTREE_CPP
#include<BTREE/BTree.h>

template<typename KeyType, typename ValueType, int MAX>
int BTree<KeyType, ValueType, MAX>::ALLOCATIONS = 0;

template<typename KeyType, typename ValueType, int MAX>
int BTree<KeyType, ValueType, MAX>::DEALLOCATIONS = 0;


template<typename KeyType, typename ValueType, int MAX>
BTree<KeyType, ValueType, MAX>::BTree()
:
_root(nullptr),
_keyCount(0),
MAX_KEYS(MAX - 1),
MIN_KEYS(static_cast<int>((0.5) + (MAX / 2.0)) - 1)
{
    this->_outputMode = HIERARCHY;
}

template<typename KeyType, typename ValueType, int MAX>
BTree<KeyType, ValueType, MAX>::BTree(const BTree<KeyType, ValueType> &btree)
:
_root(nullptr),
_keyCount(0),
MAX_KEYS(MAX - 1),
MIN_KEYS(static_cast<int>((0.5) + (MAX / 2.0)) - 1)
{
    *this = btree;
}

template<typename KeyType, typename ValueType, int MAX>
BTree<KeyType, ValueType, MAX> &BTree<KeyType, ValueType, MAX>::operator = (const BTree<KeyType, ValueType> &btree)
{
    if (this != &btree)
    {
        this->~BTree();
        this->_root = (btree._root) ? copyTree(btree._root) : nullptr;
        this->_outputMode = btree._outputMode;
        this->_keyCount = btree._keyCount;
    }
}

template<typename KeyType, typename ValueType, int MAX>
BTree<KeyType, ValueType, MAX>::~BTree()
{
    clear();
}

template<typename KeyType, typename ValueType, int MAX>
void BTree<KeyType, ValueType, MAX>::setOutputMode(BTree::OutputMode mode)
{
    this->_outputMode = mode;
}

template<typename KeyType, typename ValueType, int MAX>
typename BTree<KeyType, ValueType, MAX>::OutputMode BTree<KeyType, ValueType, MAX>::getOutputMode() const
{
    return this->_outputMode;
}

template<typename KeyType, typename ValueType, int MAX>
std::ostream &operator << (std::ostream &out, const BTree<KeyType, ValueType, MAX> &btree)
{
    if (!btree._root) return out;

    switch(btree._outputMode)
    {
        case BTree<KeyType, ValueType, MAX>::SIDEWAYS:
            btree.printSideways(btree._root, 0, out);
            break;
        case BTree<KeyType, ValueType, MAX>::HIERARCHY:
        default:
            btree.printInHierarchy(btree._root, 0, out);
            break;
    }
    return out;
}

template<typename KeyType, typename ValueType, int MAX>
void BTree<KeyType, ValueType, MAX>::incrementKeyCount()
{
    ++this->_keyCount;
}

template<typename KeyType, typename ValueType, int MAX>
void BTree<KeyType, ValueType, MAX>::decrementKeyCount()
{
    --this->_keyCount;
}

template<typename KeyType, typename ValueType, int MAX>
BTreeNode<KeyType, ValueType> *BTree<KeyType, ValueType, MAX>::createNode(const std::vector<std::pair<KeyType, ValueType>> &keysValues)
{
    ++BTree<KeyType, ValueType>::ALLOCATIONS;
    return new BTreeNode<KeyType, ValueType>(keysValues);
}

template<typename KeyType, typename ValueType, int MAX>
void BTree<KeyType, ValueType, MAX>::deleteNode(BTreeNode<KeyType, ValueType> *node)
{
    ++BTree<KeyType, ValueType>::DEALLOCATIONS;
    delete node;
}

template<typename KeyType, typename ValueType, int MAX>
void BTree<KeyType, ValueType, MAX>::printSideways(BTreeNode<KeyType, ValueType> *root, int depthCount, std::ostream &out) const
{
    // Printing the right side.
    for (int i = (int) root->subsets.size() - 1; i >= (int) root->subsets.size() / 2; --i)
    {
        printSideways(root->subsets.at(i), depthCount + 1, out);
    }

    out << std::setw(4 * depthCount);
    for (int i = 0; i < root->keysValues.size(); ++i)
    {
        out << root->keysValues.at(i).first;
        if (i < root->keysValues.size() - 1)
        {
            out << ", ";
        }
    }
    out << '\n';

    // Printing the left side.
    for (int i = 0; i < (int) root->subsets.size() / 2; ++i)
    {
        printSideways(root->subsets.at(i), depthCount + 1, out);
    }
}

template<typename KeyType, typename ValueType, int MAX>
void BTree<KeyType, ValueType, MAX>::printInHierarchy(BTreeNode<KeyType, ValueType> *root, int depthCount, std::ostream &out) const
{
    // Print keys for this node
    out << std::string(4 * depthCount, ' ');
    for (int i = 0; i < root->keysValues.size(); ++i)
    {
        out << root->keysValues.at(i).first;
        out << "[" << root->keysValues.at(i).second << "]";
        if (i < root->keysValues.size() - 1)
        {
            out << ", ";
        }
    }
    out << '\n';

    // Recursively print subsets
    for (auto & i : root->subsets)
    {
        printInHierarchy(i, depthCount + 1, out);
    }
}

template<typename KeyType, typename ValueType, int MAX>
bool BTree<KeyType, ValueType, MAX>::empty() const
{
    return !this->_keyCount;
}

template<typename KeyType, typename ValueType, int MAX>
bool BTree<KeyType, ValueType, MAX>::remove(const KeyType &key)
{
    if (!this->_root) return false;

    bool removed = loose_remove(this->_root, key);
    if (removed)
    {
        decrementKeyCount();
        if (this->_root->keysValues.empty())
        {
            shrinkTree();
        }
    }
    return removed;
}

template<typename KeyType, typename ValueType, int MAX>
void BTree<KeyType, ValueType, MAX>::clear()
{
    if (!this->_root) return;

    clearSubset(this->_root);
    this->_keyCount = 0;
    this->_root = nullptr;
}

template<typename KeyType, typename ValueType, int MAX>
ValueType* BTree<KeyType, ValueType, MAX>::find(const KeyType &key) const
{
    if (!this->_root) return nullptr;
    return recursiveFind(this->_root, key);
}

template<typename KeyType, typename ValueType, int MAX>
void BTree<KeyType, ValueType, MAX>::insert(const std::pair<KeyType, ValueType> &keyValue)
{
    if (!this->_root)
    {
        createRoot(keyValue);
    }
    else
    {
        // Insert the keys (trickle down to the leaf nodes).
        loose_insert(this->_root, keyValue);
        // Once the keys is inserted, check if the root is overflowing with keys.
        if (this->_root->keysValues.size() > MAX_KEYS)
        {
            // If so, then grow the three by one level.
            growTree();
        }
    }
    incrementKeyCount();
}

template<typename KeyType, typename ValueType, int MAX>
int BTree<KeyType, ValueType, MAX>::keyCount() const
{
    return this->_keyCount;
}

template<typename KeyType, typename ValueType, int MAX>
void BTree<KeyType, ValueType, MAX>::createRoot(const std::pair<KeyType, ValueType> &keysValues)
{
    this->_root = createNode({keysValues});
}

template<typename KeyType, typename ValueType, int MAX>
void BTree<KeyType, ValueType, MAX>::loose_insert(BTreeNode<KeyType, ValueType> *root, const std::pair<KeyType, ValueType> &keyValue)
{
    // Find the index of where this keys should be.
    int index = find_ge(root, keyValue.first);
    // Only push keys to a leafNode (will usually be a child).
    if (root->subsets.empty())
    {
        root->keysValues.insert(root->keysValues.begin() + index, keyValue);
        // Return to the parent of the leafNode we just inserted to.
        return;
    }
    // Otherwise if root is not a leafNode, then insert the keys into the appropriate subsets.
    else
    {
        loose_insert(root->subsets.at(index), keyValue);
    }

    // Once back at the parent's call stack, check the child node for excess keys.
    if (root->subsets.at(index)->keysValues.size() > MAX_KEYS)
    {
        // If it is over the limit, split the node.
        splitNode(root, index);
    }
}

template<typename KeyType, typename ValueType, int MAX>
void BTree<KeyType, ValueType, MAX>::splitNode(BTreeNode<KeyType, ValueType> *parent, int subset_index)
{
    BTreeNode<KeyType, ValueType> *child = parent->subsets.at(subset_index);
    auto *newChild = createNode({});
    newChild->keysValues.reserve(MAX_KEYS);
    newChild->subsets.reserve(MAX);

    int midIndex = child->keysValues.size() / 2;
    std::pair<KeyType, ValueType> midKeyValue = child->keysValues.at(midIndex);

    newChild->keysValues.assign(child->keysValues.begin() + midIndex + 1, child->keysValues.end());
    child->keysValues.erase(child->keysValues.begin() + midIndex, child->keysValues.end());

    if (!child->subsets.empty())
    {
        newChild->subsets.assign(child->subsets.begin() + midIndex + 1, child->subsets.end());
        child->subsets.erase(child->subsets.begin() + midIndex + 1, child->subsets.end());
    }
    parent->keysValues.insert(parent->keysValues.begin() + subset_index, midKeyValue);
    parent->subsets.insert(parent->subsets.begin() + subset_index + 1, newChild);
}

template<typename KeyType, typename ValueType, int MAX>
void BTree<KeyType, ValueType, MAX>::growTree()
{
    auto *newRoot = createNode({});
    newRoot->subsets.push_back(this->_root);
    this->_root = newRoot;
    splitNode(this->_root, 0);
}

template<typename KeyType, typename ValueType, int MAX>
bool BTree<KeyType, ValueType, MAX>::loose_remove(BTreeNode<KeyType, ValueType> *root, const KeyType &key)
{
    int index = find_ge(root, key);
    bool found = index < root->keysValues.size() && root->keysValues.at(index).first == key;
    bool removed = false;

    // If the key was found at a leaf-node...
    if (found && root->subsets.empty())
    {
        // Remove it from the key vector.
        root->keysValues.erase(root->keysValues.begin() + index);
        return true;
    }
    // If the key was found in an internal node, swap values with its predecessor.
    // Call loose_insert again from that subset with the same key value as before.
    else if (found && !root->subsets.empty())
    {
        KeyType& predecessor = findPredecessor(root->subsets.at(index));
        root->keysValues.at(index).first = predecessor;
        predecessor = key;
        removed = loose_remove(root->subsets.at(index), key);
    }
    // If it wasn't found but there are still subsets to check, call loose_insert() on the required subset.
    else if(!found && !root->subsets.empty())
    {
        removed = loose_remove(root->subsets.at(index), key);
    }

    // If we are at a leaf-node, return to the parent's call stack.
    if (root->subsets.empty())
    {
        return false;
    }

    // Once back at the parent's call stack, attempt to fix any key shortage from the subset we just deleted from.
    fix_shortage(root, index);
    return removed;
}

template<typename KeyType, typename ValueType, int MAX>
void BTree<KeyType, ValueType, MAX>::fix_shortage(BTreeNode<KeyType, ValueType> *parent, int subsetIndex)
{
    // If the subset is short on keys...
    if (!parent->subsets.empty() && parent->subsets.at(subsetIndex)->keysValues.size() < MIN_KEYS)
    {
        // Attempt to rotate a key from the left sibling.
        if (rotate_left(parent, subsetIndex)) return;
        // Otherwise attempt to rotate a key from the right sibling.
        if (rotate_right(parent, subsetIndex)) return;
        // Otherwise merge the first key in the parent into the left subtree. Delete the right subtree.
        merge_left(parent, subsetIndex);
    }
}

template<typename KeyType, typename ValueType, int MAX>
bool BTree<KeyType, ValueType, MAX>::rotate_left(BTreeNode<KeyType, ValueType> *parent, int subsetIndex)
{
    BTreeNode<KeyType, ValueType> *thinSubset = parent->subsets.at(subsetIndex);
    // Ensure we won't go out of bounds on the left.
    if (subsetIndex <= 0)
    {
        return false;
    }
    BTreeNode<KeyType, ValueType> *leftSubset = parent->subsets.at(subsetIndex - 1);
    if (leftSubset->keysValues.size() > MIN_KEYS)
    {
        int index = subsetIndex - 1;

        std::pair<KeyType, ValueType> temp = parent->keysValues.at(index);
        // Perform the (left) rotation.
        parent->keysValues.at(index) = leftSubset->keysValues.back();
        // Erase the rightmost key from the left subtree.
        leftSubset->keysValues.pop_back();
        // Push to the front the rightmost key into the deficient node.
        thinSubset->keysValues.insert(thinSubset->keysValues.begin(), temp);

        // Also bring the rightmost key's children (if it has any).
        if (!leftSubset->subsets.empty())
        {
            thinSubset->subsets.insert(thinSubset->subsets.begin(), leftSubset->subsets.back());
            // Erase this subset from the left subtree.
            leftSubset->subsets.pop_back();
        }
        return true;
    }
    return false;
}

template<typename KeyType, typename ValueType, int MAX>
bool BTree<KeyType, ValueType, MAX>::rotate_right(BTreeNode<KeyType, ValueType> *parent, int subsetIndex)
{
    BTreeNode<KeyType, ValueType> *thinSubset = parent->subsets.at(subsetIndex);
    // Ensure we don't go out of bounds on the right.
    if (subsetIndex >= parent->keysValues.size())
    {
        return false;
    }
    BTreeNode<KeyType, ValueType> *rightSubset = parent->subsets.at(subsetIndex + 1);

    if (rightSubset->keysValues.size() > MIN_KEYS)
    {
        // We can safely pass subsetIndex as an index because subsetIndex will only reach this code
        // if it is less than parent->keys.size().
        std::pair<KeyType, ValueType> temp = parent->keysValues.at(subsetIndex);
        // Perform the (right) rotation.
        parent->keysValues.at(subsetIndex) = rightSubset->keysValues.front();
        // Erase the leftmost key from the right subtree.
        rightSubset->keysValues.erase(rightSubset->keysValues.begin());
        // Push to the back the leftmost key from the right subtree into the deficient node.
        thinSubset->keysValues.push_back(temp);

        // Also bring the leftmost key's children (if it has any).
        if (!rightSubset->subsets.empty())
        {
            thinSubset->subsets.push_back(rightSubset->subsets.front());
            // Erase this subset from the right subtree.
            rightSubset->subsets.erase(rightSubset->subsets.begin());
        }
        return true;
    }
    return false;
}

template<typename KeyType, typename ValueType, int MAX>
void BTree<KeyType, ValueType, MAX>::merge_left(BTreeNode<KeyType, ValueType> *parent, int subsetIndex)
{
    BTreeNode<KeyType, ValueType> *leftSubtree = (subsetIndex <= 0) ? parent->subsets.at(subsetIndex) :
                                parent->subsets.at(subsetIndex - 1);

    auto keyValueIter= (subsetIndex <= 0) ? parent->keysValues.begin() : parent->keysValues.begin() + (subsetIndex - 1);
    // Push the appropriate key from the parent into the key's left subtree.
    leftSubtree->keysValues.push_back(*keyValueIter);

    // Remove the key from the parent's collection of keys.
    parent->keysValues.erase(keyValueIter);

    if (subsetIndex > 0)
    {
        // Push the remaining keys
        for (auto &keyValue : parent->subsets.at(subsetIndex)->keysValues)
        {
            leftSubtree->keysValues.push_back(keyValue);
        }
        // Push the children of the remaining keys.
        for (BTreeNode<KeyType, ValueType> *subset: parent->subsets.at(subsetIndex)->subsets)
        {
            leftSubtree->subsets.push_back(subset);
        }

        // Delete the empty subset.
        deleteNode(parent->subsets.at(subsetIndex));
        parent->subsets.erase(parent->subsets.begin() + subsetIndex);
    }
    else
    {
        BTreeNode<KeyType, ValueType> *rightSubtree = parent->subsets.at(subsetIndex + 1);
        // Push the remaining keys from the right subset.
        for (auto &keyValue : rightSubtree->keysValues)
        {
            leftSubtree->keysValues.push_back(keyValue);
        }
        // Push the children of the remaining keys form the right subset.
        for (BTreeNode<KeyType, ValueType> *subset: rightSubtree->subsets)
        {
            leftSubtree->subsets.push_back(subset);
        }
        deleteNode(rightSubtree);
        parent->subsets.erase(parent->subsets.begin() + subsetIndex + 1);
    }
}

template<typename KeyType, typename ValueType, int MAX>
void BTree<KeyType, ValueType, MAX>::shrinkTree()
{
    if (empty())
    {
        deleteNode(this->_root);
        this->_root = nullptr;
    }
    else
    {
        BTreeNode<KeyType, ValueType> *oldRoot = this->_root;
        this->_root = this->_root->subsets.at(0);
        deleteNode(oldRoot);
    }
}

template<typename KeyType, typename ValueType, int MAX>
void BTree<KeyType, ValueType, MAX>::clearSubset(BTreeNode<KeyType, ValueType> *root)
{
    for (BTreeNode<KeyType, ValueType> *subset : root->subsets)
    {
        clearSubset(subset);
    }
    deleteNode(root);
}

template<typename KeyType, typename ValueType, int MAX>
KeyType &BTree<KeyType, ValueType, MAX>::findPredecessor(BTreeNode<KeyType, ValueType> *root)
{
    if (root->subsets.empty())
        return root->keysValues.back().first;

    return findPredecessor(root->subsets.back());
}

template<typename KeyType, typename ValueType, int MAX>
BTreeNode<KeyType, ValueType> *BTree<KeyType, ValueType, MAX>::copyTree(const BTreeNode<KeyType, ValueType> *root)
{
    auto *newNode = createNode(root->keysValues);
    for (const auto &subset : root->subsets)
    {
        newNode->subsets.push_back(copyTree(subset));
    }
    return newNode;
}

template<typename KeyType, typename ValueType, int MAX>
ValueType *BTree<KeyType, ValueType, MAX>::recursiveFind(BTreeNode<KeyType, ValueType> *root, const KeyType &key) const
{
    int index = find_ge(root, key);
    bool found = index < root->keysValues.size() && root->keysValues.at(index).first == key;

    if (found)
    {
        return &root->keysValues.at(index).second;
    }

    if (root->subsets.empty())
    {
        return nullptr;
    }
    else
    {
        return recursiveFind(root->subsets.at(index), key);
    }
}

template<typename KeyType, typename ValueType, int MAX>
int BTree<KeyType, ValueType, MAX>::find_ge(BTreeNode<KeyType, ValueType> *root, const KeyType &key) const
{
    int index = 0;
    while (index < root->keysValues.size() && key > root->keysValues.at(index).first)
    {
        ++index;
    }
    return index;
}

#endif //CS008_LABS_BTREE_CPP