#include "Btree.h"
#include "BTreeNode.h"

BTree::BTree(int _t)
{
    root = nullptr;
    t = _t;
}

BTree::BTree(BTreeNode* root, int _t) {
    root = root;
    t = _t;
}



BTree::~BTree()
{
    delete root;
}

void BTree::traverse()
{
    if (root != nullptr)
        root->traverse();
}

std::pair<int, size_t> BTree::search(int id)
{
    if (root == nullptr)
    {
        return { -1, 0 };
    }
    // Llama al método search de la raíz
    return root->search(id);
}

bool BTree::isExisting(int id)
{
    return root == nullptr ? false : root->isExisting(id);
}

void BTree::insert(int k, size_t t)
{
    if (root == nullptr)
    {
        root = new BTreeNode(this->t, true);
        root->keys[0] = { k, t };
        root->n = 1;
    }
    else
    {
        if (root->n == 2 * this->t - 1)
        {
            BTreeNode* s = new BTreeNode(this->t, false);
            s->children[0] = root;
            s->splitChild(0, root);
            int i = 0;
            if (s->keys[0].first < k)
                i++;
            s->children[i]->insertNonFull(k, t);
            root = s;
        }
        else
            root->insertNonFull(k, t);
    }
}

void BTree::Delete(BTreeNode* node, int key)
{
    if (node == nullptr)
    {
        node = root;
    }

    int i = 0;
    while (i < node->n && key > node->keys[i].first)
    {
        i++;
    }

    if (node->leaf)
    {
        if (i < node->n && key == node->keys[i].first)
        {
            for (int j = i; j < node->n - 1; j++)
            {
                node->keys[j] = node->keys[j + 1];
            }
            node->n -= 1;
            std::cout << "\n\t\tSe eliminó el registro " << key << " con éxito." << std::endl;
            return;
        }
        else
        {
            std::cout << "\n\t\tNo se encontró el nodo" << std::endl;
            return;
        }
    }

    // Si la clave está en este nodo
    if (i < node->n && node->keys[i].first == key)
    {
        node->delete_internal_node(node, key, i, root);
        return;
    }

    // Si la clave no está en este nodo y este nodo no es una hoja, descendemos al hijo apropiado
    if (node->children[i]->n >= t)
    {
        Delete(node->children[i], key);
    }
    else
    {
        // Manejar casos de fusionar o dividir nodos hermanos
        if (i != 0 && i + 2 < node->children.size())
        {
            if (node->children[i - 1]->n >= t)
            {
                node->delete_sibling(node, i, i - 1);
            }
            else if (node->children[i + 1]->n >= t)
            {
                node->delete_sibling(node, i, i + 1);
            }
            else
            {
                node->delete_merge(node, i, i + 1, root);
            }
        }
        else if (i == 0)
        {
            if (node->children[i + 1]->n >= t)
            {
                node->delete_sibling(node, i, i + 1);
            }
            else
            {
                node->delete_merge(node, i, i + 1, root);
            }
        }
        else if (i + 1 == node->children.size())
        {
            if (node->children[i - 1]->n >= t)
            {
                node->delete_sibling(node, i, i - 1);
            }
            else
            {
                node->delete_merge(node, i - 1, i, root);
            }
        }
        // Después de fusionar o dividir nodos hermanos, descendemos al hijo apropiado para continuar la eliminación
        Delete(node->children[i], key);
    }
}