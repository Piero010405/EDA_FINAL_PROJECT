#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <iostream>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/utility.hpp>

namespace boost {
    namespace serialization {
        template<class Archive>
        void serialize(Archive& ar, std::pair<int, size_t>& p, const unsigned int version) {
            ar& p.first;
            ar& p.second;
        }
    }
}

class BTreeNode
{
public:
    int t;                                          // Grado mínimo (define el rango de número de claves)
    std::vector<std::pair<int, size_t>> keys;       // Vector de claves
    std::vector<BTreeNode*> children;               // Vector de punteros a los hijos
    int n;                                          // Número actual de claves
    bool leaf;                                      // Es verdadero cuando el nodo es hoja
    
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar& t& keys& children& n& leaf;
    }

    BTreeNode() = default;
    BTreeNode(int _t, bool _leaf);
    ~BTreeNode(); // Destructor para liberar memoria

    void traverse();
    void insertNonFull(int k, size_t t);
    void splitChild(int i, BTreeNode* y);
    void delete_internal_node(BTreeNode* node, int key, int index, BTreeNode*& root);
    std::pair<int, size_t> delete_predecessor(BTreeNode* node, BTreeNode*& root);
    std::pair<int, size_t> delete_successor(BTreeNode* node, BTreeNode*& root);
    void delete_sibling(BTreeNode* node, int i, int j);
    void delete_merge(BTreeNode* node, int i, int j, BTreeNode*& root);
    std::pair<int, size_t> search(int id);
    bool isExisting(int id);
    void Delete(int id, BTreeNode* root);

    friend class BTree;
};
