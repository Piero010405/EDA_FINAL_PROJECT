#pragma once
#include "BTreeNode.h"
class BTree
{
public:
    BTreeNode* root;
    int t;              // Grado mínimo
    BTree(int _t);

    BTree(BTreeNode* root, int _t);

    void traverse();

    void insert(int k, size_t t);

    std::pair<int, size_t> search(int key, BTreeNode* node);

    void Delete(BTreeNode* node, int key);

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar& root& t;
    }
};