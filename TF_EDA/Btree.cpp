#include "Btree.h"
#include "BTreeNode.h"

BTree::BTree(int _t) {
    root = nullptr;
    t = _t;
}

BTree::BTree(BTreeNode* root, int _t) {
    root = root;
    t = _t;
}

void BTree::traverse()
{
    if (root != nullptr)
        root->traverse();
}

void BTree::insert(int k, size_t t)
{
    // Si el árbol está vacío
    if (root == nullptr)
    {
        // Asigna memoria para la raíz
        root = new BTreeNode(t, true);
        root->keys[0] = {k, t}; // Inserta la clave
        root->n = 1;            // Actualiza el número de claves en la raíz
    }
    else
    { // Si el árbol no está vacío
        // Si la raíz está llena, entonces el árbol crece en altura
        if (root->n == 2 * t - 1)
        {
            // Asigna memoria para la nueva raíz
            BTreeNode* s = new BTreeNode(t, false);

            // Hace la antigua raíz como hijo de la nueva raíz
            s->children[0] = root;

            // Divide la antigua raíz y mueve 1 clave a la nueva raíz
            s->splitChild(0, root);

            // La nueva raíz tiene dos hijos ahora. Decide cuál de los
            // dos hijos va a tener la nueva clave
            int i = 0;
            if (s->keys[0].first < k)
                i++;
            s->children[i]->insertNonFull(k);

            // Cambia la raíz
            root = s;
        }
        else
        { // Si la raíz no está llena, llama a insertNonFull para la raíz
            root->insertNonFull(k);
        }
    }
}

std::pair<int, size_t> BTree::search(int key)
{
    if (root == nullptr) {
        return {-1, 0};
    }
    // Llama al método search de la raíz
    return root->search(key);

}

void BTree::Delete(int key)
{
    if (!root) {
        std::cout << "\n\t\tEl arbol esta vacío. No se puede eliminar ninguna clave" << std::endl;
        return;
    }

    root->Delete(key, root);

    if (root->n == 0) {
        BTreeNode* tmp = root;
        if (root->leaf) {
            root = nullptr;
        }
        else {
            root = root->children[0];
        }
        delete tmp;
    }
}

bool BTree::isExisting(int key)
{
    if (root == nullptr) {
        return false;
    }
    // Llama al método search de la raíz
    return root->isExisting(key);

}