#pragma once
#include <iostream>
#include <vector>

class BTreeNode
{
    int t;                        // Grado m�nimo (define el rango de n�mero de claves)
    std::vector<int> keys;             // Vector de claves
    std::vector<BTreeNode*> children; // Vector de punteros a los hijos
    int n;                        // N�mero actual de claves
    bool leaf;                    // Es verdadero cuando el nodo es hoja

public:
    BTreeNode(int _t, bool _leaf);

    // Funci�n para recorrer todos los nodos en un sub�rbol enraizado con este nodo
    void traverse();

    // Funci�n para insertar una nueva clave en el sub�rbol enraizado con este nodo
    void insertNonFull(int k);

    // Funci�n para dividir el hijo y de este nodo. i es el �ndice de y en el vector children
    void splitChild(int i, BTreeNode* y);

    void delete_internal_node(BTreeNode* node, int key, int index, BTreeNode*& root);

    int delete_predecessor(BTreeNode* node, BTreeNode*& root);

    int delete_successor(BTreeNode* node, BTreeNode*& root);

    void delete_sibling(BTreeNode* node, int i, int j);

    void delete_merge(BTreeNode* node, int i, int j, BTreeNode*& root);

    friend class BTree;
};
