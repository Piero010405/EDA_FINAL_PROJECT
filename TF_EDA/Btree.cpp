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
    // Si el �rbol est� vac�o
    if (root == nullptr)
    {
        // Asigna memoria para la ra�z
        root = new BTreeNode(t, true);
        root->keys[0] = {k, t}; // Inserta la clave
        root->n = 1;            // Actualiza el n�mero de claves en la ra�z
    }
    else
    { // Si el �rbol no est� vac�o
        // Si la ra�z est� llena, entonces el �rbol crece en altura
        if (root->n == 2 * t - 1)
        {
            // Asigna memoria para la nueva ra�z
            BTreeNode* s = new BTreeNode(t, false);

            // Hace la antigua ra�z como hijo de la nueva ra�z
            s->children[0] = root;

            // Divide la antigua ra�z y mueve 1 clave a la nueva ra�z
            s->splitChild(0, root);

            // La nueva ra�z tiene dos hijos ahora. Decide cu�l de los
            // dos hijos va a tener la nueva clave
            int i = 0;
            if (s->keys[0].first < k)
                i++;
            s->children[i]->insertNonFull(k);

            // Cambia la ra�z
            root = s;
        }
        else
        { // Si la ra�z no est� llena, llama a insertNonFull para la ra�z
            root->insertNonFull(k);
        }
    }
}

std::pair<int, size_t> BTree::search(int key)
{
    if (root == nullptr) {
        return {-1, 0};
    }
    // Llama al m�todo search de la ra�z
    return root->search(key);

}

void BTree::Delete(int key)
{
    if (!root) {
        std::cout << "\n\t\tEl arbol esta vac�o. No se puede eliminar ninguna clave" << std::endl;
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
    // Llama al m�todo search de la ra�z
    return root->isExisting(key);

}