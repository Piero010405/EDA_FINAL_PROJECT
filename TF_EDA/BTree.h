#pragma once
#include "BTreeNode.h"
class BTree
{
    BTreeNode* root;
    int t; // Grado mínimo
public:
    BTree(int _t);

    void traverse();

    void insert(int k, size_t t);

    std::pair<int, size_t> search(int key, BTreeNode* node);

    void Delete(BTreeNode* node, int key);
};