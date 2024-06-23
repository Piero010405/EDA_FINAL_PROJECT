#include "Btree.h"
#include "BTreeNode.h"

BTree::BTree(int _t) {
    root = nullptr;
    t = _t;
}

BTree::BTree(BTreeNode root, int _t) {
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

std::pair<int, size_t> BTree::search(int key, BTreeNode* node)
{
    if (node == nullptr)
    {
        node = root;
    }
    int i = 0;
    while (i < node->n && key > node->keys[i].first)
    {
        i += 1;
    }
    if (i < node->n && key == node->keys[i].first)
    {
        //std::cout << "Se encontró el nodo" << std::endl;
        return node->keys[i];
    }
    else if (node->leaf)
    {
        //std::cout << "No se encontró el nodo" << std::endl;
        return {-1, 0};
    }
    else
    {
        return search(key, node->children[i]);
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
            std::cout << "Se eliminó el registro " << key << " con éxito." << std::endl;
            return;
        }
        else
        {
            std::cout << "No se encontró el nodo" << std::endl;
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
        if (i != 0 && i + 1 < node->children.size())
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