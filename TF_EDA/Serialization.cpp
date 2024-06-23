#include "BTree.h"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/utility.hpp>
#include <fstream>

void SerializeBTree(const BTree& btree, const char* outputFilename) {
    std::ofstream outFile(outputFilename);
    if (!outFile.is_open()) {
        std::cerr << "\t\tError al abrir el archivo de salida: " << outputFilename << " - " << strerror(errno) << std::endl;
        return;
    }
    boost::archive::text_oarchive oa(outFile);
    oa << btree;
    outFile.close();
}

BTree* DeserializeBTree(const char* inputFilename, int minimumDegree) {
    std::ifstream inFile(inputFilename);
    if (!inFile.is_open()) {
        std::cerr << "\t\tError al abrir el archivo de entrada: " << inputFilename << " - " << strerror(errno) << std::endl;
        return nullptr;
    }
    BTree* btree = new BTree(minimumDegree);
    boost::archive::text_iarchive ia(inFile);
    ia >> *btree;
    inFile.close();
    return btree;
}

//void SerializeBTreeNode(BTreeNode* node, std::ofstream& outFile) {
//    if (node == nullptr) return;
//
//    // Escribir si el nodo es una hoja
//    outFile.write(reinterpret_cast<char*>(&node->leaf), sizeof(node->leaf));
//
//    // Escribir el numero de claves en el nodo
//    outFile.write(reinterpret_cast<char*>(&node->n), sizeof(node->n));
//
//    // Escribir las claves que contienen los DNI's y direcciones de memoria
//    for (int i = 0; i < node->n; ++i) {
//        outFile.write(node->keys[i], sizeof(std::pair<int, size_t>));
//    }
//
//    // Si el nodo no es una hoja, hay que escribir también sus hijos
//    if (!node->leaf) {
//        for (int i = 0; i <= node->n; ++i) {
//            SerializeBTreeNode(node->children[i], outFile); // Serializar los nodos hijos recursivamente
//        }
//    }
//}
//
//
//BTreeNode* DeserializeBTreeNode(std::ifstream& inFile, int minimunDegree) {
//    bool isLeaf;
//    int numberKeys;
//    inFile.read(reinterpret_cast<char*>(&isLeaf), sizeof(isLeaf));
//    inFile.read(reinterpret_cast<char*>(&numberKeys), sizeof(numberKeys));
//
//    BTreeNode* node = new BTreeNode(minimunDegree, isLeaf);
//    node->n = numberKeys;
//
//    for (int i = 0; i < numberKeys; ++i) {
//        inFile.read(node->keys[i], sizeof(std::pair<int, size_t>));
//    }
//
//    if (!isLeaf) {
//        for (int i = 0; i <= numberKeys; ++i) {
//            node->children[i] = DeserializeBTreeNode(inFile, minimunDegree);
//        }
//    }
//
//    return node;
//}
//
//void SerializeBTree(BTree& btree, const char* outputFilename) {
//    std::ofstream outFile(outputFilename, std::ios::binary);
//    if (!outFile.is_open()) {
//        std::cerr << "\t\tError al abrir el archivo de salida: " << outputFilename << " - " << strerror(errno) << std::endl;
//        return;
//    }
//    SerializeBTreeNode(btree.root, outFile);
//    outFile.close();
//}
//
//
//BTree* DeserializeBTree(const char* inputFilename, int minimunDegree) {
//    std::ifstream inFile(inputFilename, std::ios::binary);
//    if (!inFile.is_open()) {
//        std::cerr << "\t\tError al abrir el archivo de entrada: " << inputFilename << " - " << strerror(errno) << std::endl;
//        return;
//    }
//    BTreeNode* root = DeserializeBTreeNode(inFile, minimunDegree);
//    inFile.close();
//    return new BTree(root, minimunDegree);
//}