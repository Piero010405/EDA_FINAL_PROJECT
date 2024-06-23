#pragma once
#include "BTreeNode.h"
class BTree
{
public:
    BTreeNode* root;
    int t;              // Grado mínimo
    
    BTree(int _t);
    BTree(BTreeNode* root, int _t);
    ~BTree();


    void traverse();
    std::pair<int, size_t> search(int key);
    bool isExisting(int key);
    void insert(int k, size_t t);
    void Delete(int key);

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar& root& t;
    }
};