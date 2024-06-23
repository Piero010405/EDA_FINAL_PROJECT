#include "BTreeNode.h"

BTreeNode::BTreeNode(int t1, bool leaf1)
{
    t = t1;
    leaf = leaf1;
    n = 0;
    keys.resize(2 * t - 1);
    children.resize(2 * t);
}

void BTreeNode::traverse()
{
    int i;
    for (i = 0; i < n; i++)
    {
        // Si este nodo no es hoja, entonces antes de imprimir la clave[i],
        // recorre el subárbol enraizado con el hijo children[i].
        if (!leaf)
            children[i]->traverse();
        std::cout << " " << keys[i].first;
    }

    // Imprime el subárbol enraizado con el último hijo
    if (!leaf)
        children[i]->traverse();
}

void BTreeNode::insertNonFull(int k)
{
    // Inicializa i con el índice del elemento más a la derecha
    int i = n - 1;

    // Si este es un nodo hoja
    if (leaf)
    {
        // El siguiente bucle hace dos cosas:
        // a) Encuentra la ubicación de la nueva clave a insertar
        // b) Mueve todas las claves mayores un lugar hacia adelante
        while (i >= 0 && keys[i].first > k)
        {
            keys[i + 1] = keys[i];
            i--;
        }

        // Inserta la nueva clave en la ubicación encontrada
        keys[i + 1].first = k;
        n = n + 1;
    }
    else
    { // Si este nodo no es hoja
        // Encuentra el hijo que va a tener la nueva clave
        while (i >= 0 && keys[i].first > k)
            i--;

        // Ver si el hijo encontrado está lleno
        if (children[i + 1]->n == 2 * t - 1)
        {
            // Si el hijo está lleno, entonces divídelo
            splitChild(i + 1, children[i + 1]);

            // Después de dividir, la clave del medio de children[i] sube y children[i]
            // se divide en dos. Ver cuál de los dos va a tener la nueva clave
            if (keys[i + 1].first < k)
                i++;
        }
        children[i + 1]->insertNonFull(k);
    }
}

void BTreeNode::splitChild(int i, BTreeNode* y)
{
    // Crea un nuevo nodo que va a almacenar (t-1) claves de y
    BTreeNode* z = new BTreeNode(y->t, y->leaf);
    z->n = t - 1;

    // Copia las últimas (t-1) claves de y a z
    for (int j = 0; j < t - 1; j++)
        z->keys[j] = y->keys[j + t];

    // Copia los últimos t hijos de y a z
    if (!y->leaf)
    {
        for (int j = 0; j < t; j++)
            z->children[j] = y->children[j + t];
    }

    // Reduce el número de claves en y
    y->n = t - 1;

    // Dado que este nodo va a tener un nuevo hijo,
    // crea espacio para el nuevo hijo
    for (int j = n; j >= i + 1; j--)
        children[j + 1] = children[j];

    // Enlaza el nuevo hijo a este nodo
    children[i + 1] = z;

    // Una clave de y se moverá a este nodo. Mueve todas las claves mayores
    // un lugar hacia adelante
    for (int j = n - 1; j >= i; j--)
        keys[j + 1] = keys[j];

    // Copia la clave del medio de y a este nodo
    keys[i] = y->keys[t - 1];

    // Incrementa el conteo de claves en este nodo
    n = n + 1;
}

void BTreeNode::delete_merge(BTreeNode* node, int i, int j, BTreeNode*& root)
{
    BTreeNode* cnode = node->children[i];
    BTreeNode* sibling = node->children[j];

    if (j < i)
    {
        cnode->keys.insert(cnode->keys.begin(), node->keys[j]);
        node->keys[j] = sibling->keys[sibling->n - 1];
        if (!sibling->leaf)
        {
            cnode->children.insert(cnode->children.begin(), sibling->children[sibling->n]);
            sibling->children.pop_back();
        }
        sibling->keys.pop_back();
    }
    else
    {
        cnode->keys.push_back(node->keys[i]);
        node->keys[i] = sibling->keys[0];
        if (!sibling->leaf)
        {
            cnode->children.push_back(sibling->children[0]);
            sibling->children.erase(sibling->children.begin());
        }
        sibling->keys.erase(sibling->keys.begin());
    }
    cnode->n++;
    sibling->n--;

    if (node == root && node->n == 0)
    {
        root = cnode;
    }
}

std::pair<int, size_t> BTreeNode::delete_predecessor(BTreeNode* node, BTreeNode*& root)
{
    if (node->leaf)
    {
        std::pair<int, size_t> temp = node->keys[node->n - 1];
        node->keys.pop_back();
        node->n -= 1;
        return temp;
    }

    int n = node->n - 1;
    if (node->children[n]->n >= t)
    {
        delete_sibling(node, n + 1, n);
    }
    else
    {
        delete_merge(node, n, n + 1, root);
    }
    return delete_predecessor(node->children[n], root);
}

std::pair<int, size_t> BTreeNode::delete_successor(BTreeNode* node, BTreeNode*& root)
{
    if (node->leaf)
    {
        std::pair<int, size_t> temp = node->keys[0];
        node->keys.erase(node->keys.begin());
        node->n -= 1;
        return temp;
    }

    if (node->children[1]->n >= t)
    {
        delete_sibling(node, 0, 1);
    }
    else
    {
        delete_merge(node, 0, 1, root);
    }
    return delete_successor(node->children[0], root);
}

void BTreeNode::delete_sibling(BTreeNode* node, int i, int j)
{
    BTreeNode* cnode = node->children[i];
    BTreeNode* sibling = node->children[j];

    if (j < i)
    {
        cnode->keys.insert(cnode->keys.begin(), node->keys[j]);
        node->keys[j] = sibling->keys[sibling->n - 1];
        if (!sibling->leaf)
        {
            cnode->children.insert(cnode->children.begin(), sibling->children[sibling->n]);
            sibling->children.pop_back();
        }
        sibling->keys.pop_back();
    }
    else
    {
        cnode->keys.push_back(node->keys[i]);
        node->keys[i] = sibling->keys[0];
        if (!sibling->leaf)
        {
            cnode->children.push_back(sibling->children[0]);
            sibling->children.erase(sibling->children.begin());
        }
        sibling->keys.erase(sibling->keys.begin());
    }
    cnode->n++;
    sibling->n--;
}

void BTreeNode::delete_internal_node(BTreeNode* node, int key, int index, BTreeNode*& root)
{
    if (node->leaf)
    {
        if (node->keys[index].first == key)
        {
            for (int j = index; j < node->n - 1; j++)
            {
                node->keys[j] = node->keys[j + 1];
            }
            node->n -= 1;
            //std::cout << "Se eliminó el registro " << key << " con éxito." << std::endl;
            return;
        }
    }

    int i = 0;
    while (i < node->n && key > node->keys[i].first)
    {
        i++;
    }

    if (node->children[i]->n >= t)
    {
        node->keys[index] = delete_predecessor(node->children[i], root);
    }
    else if (i + 1 < node->children.size() && node->children[i + 1]->n >= t)
    {
        node->keys[index] = delete_successor(node->children[i + 1], root);
    }
    else
    {
        delete_merge(node, i, i + 1, root);
        delete_internal_node(node->children[i], key, t - 1, root);
    }
}

std::pair<int, size_t> BTreeNode::search(int dni) {
    // Encuentra la primera clave mayor o igual a dni
    int i = 0;
    while (i < n && keys[i].first < dni) {
        i++;
    }

    // Si la clave se encuentra en este nodo, devuelve su page ID
    if (i < n && keys[i].first == dni) {
        return keys[i];
    }

    // Si la clave no se encuentra aquí y este es un nodo hoja
    if (leaf) {
        return {-1, 0}; // Indicador de que la clave no se encontró
    }

    // Va al subárbol adecuado
    return children[i]->search(dni);
}

bool BTreeNode::isExisting(int dni) {
    // Encuentra la primera clave mayor o igual a dni
    int i = 0;
    while (i < n && keys[i].first < dni) {
        i++;
    }

    // Si la clave se encuentra en este nodo, devuelve su page ID
    if (i < n && keys[i].first == dni) {
        return true;
    }

    // Si la clave no se encuentra aquí y este es un nodo hoja
    if (leaf) {
        return false; // Indicador de que la clave no se encontró
    }

    // Va al subárbol adecuado
    return children[i]->isExisting(dni);
}

void BTreeNode::Delete(int key, BTreeNode* root) {
    int i = 0;
    while (i < n && key > keys[i].first)
    {
        i++;
    }

    if (leaf)
    {
        if (i < n && key ==keys[i].first)
        {
            for (int j = i; j < n - 1; j++)
            {
                keys[j] = keys[j + 1];
            }
            n -= 1;
            std::cout << "\t\tSe eliminó el registro " << key << " con éxito." << std::endl;
            return;
        }
        else
        {
            std::cout << "\t\tNo se encontró el nodo" << std::endl;
            return;
        }
    }

    // Si la clave está en este nodo
    if (i < n && keys[i].first == key)
    {
        delete_internal_node(this, key, i, root);
        return;
    }

    // Si la clave no está en este nodo y este nodo no es una hoja, descendemos al hijo apropiado
    if (children[i]->n >= t)
    {
        Delete(key, root);
    }
    else
    {
        // Manejar casos de fusionar o dividir nodos hermanos
        if (i != 0 && i + 1 < children.size())
        {
            if (children[i - 1]->n >= t)
            {
                delete_sibling(this, i, i - 1);
            }
            else if (children[i + 1]->n >= t)
            {
                delete_sibling(this, i, i + 1);
            }
            else
            {
                delete_merge(this, i, i + 1, root);
            }
        }
        else if (i == 0)
        {
            if (children[i + 1]->n >= t)
            {
                delete_sibling(this, i, i + 1);
            }
            else
            {
                delete_merge(this, i, i + 1, root);
            }
        }
        else if (i + 1 == children.size())
        {
            if (children[i - 1]->n >= t)
            {
                delete_sibling(this, i, i - 1);
            }
            else
            {
                delete_merge(this, i - 1, i, root);
            }
        }
        // Después de fusionar o dividir nodos hermanos, descendemos al hijo apropiado para continuar la eliminación
        Delete(key, root);
    }
}