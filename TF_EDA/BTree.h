#pragma once
#include "BTreeNode.h"
class BTree
{
    BTreeNode* root;
    int t; // Grado mínimo
public:
    BTree(int _t)
    {
        root = nullptr;
        t = _t;
    }

    void traverse()
    {
        if (root != nullptr)
            root->traverse();
    }

    void insert(int k);

    void search(int key, BTreeNode* node);

    void Delete(BTreeNode* node, int key);
};