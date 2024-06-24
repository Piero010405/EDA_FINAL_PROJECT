#include "BTreeNode.h"

BTreeNode::BTreeNode(int t1, bool leaf1)
    : t(t1), leaf(leaf1), n(0)
{
    keys.resize(2 * t - 1);
    children.resize(2 * t, nullptr);
}

BTreeNode::~BTreeNode()
{
    for (auto child : children)
    {
        delete child;
    }
}

void BTreeNode::traverse()
{
    int i;
    for (i = 0; i < n; i++)
    {
        if (!leaf)
            children[i]->traverse();
        std::cout << " " << keys[i].first;
    }
    if (!leaf)
        children[i]->traverse();
}

void BTreeNode::insertNonFull(int k, size_t t)
{
    int i = n - 1;
    if (leaf)
    {
        while (i >= 0 && keys[i].first > k)
        {
            keys[i + 1] = keys[i];
            i--;
        }
        keys[i + 1] = { k, t };
        n = n + 1;
    }
    else
    {
        while (i >= 0 && keys[i].first > k)
            i--;
        if (children[i + 1]->n == 2 * this->t - 1)
        {
            splitChild(i + 1, children[i + 1]);
            if (keys[i + 1].first < k)
                i++;
        }
        children[i + 1]->insertNonFull(k, t);
    }
}

void BTreeNode::splitChild(int i, BTreeNode* y)
{
    BTreeNode* z = new BTreeNode(y->t, y->leaf);
    z->n = t - 1;
    for (int j = 0; j < t - 1; j++)
        z->keys[j] = y->keys[j + t];
    if (!y->leaf)
    {
        for (int j = 0; j < t; j++)
            z->children[j] = y->children[j + t];
    }
    y->n = t - 1;
    for (int j = n; j >= i + 1; j--)
        children[j + 1] = children[j];
    children[i + 1] = z;
    for (int j = n - 1; j >= i; j--)
        keys[j + 1] = keys[j];
    keys[i] = y->keys[t - 1];
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
            return;
        }
        return;
    }

    if (node->children[index]->n >= t)
    {
        node->keys[index] = delete_predecessor(node->children[index], root);
    }
    else if (node->children[index + 1]->n >= t)
    {
        node->keys[index] = delete_successor(node->children[index + 1], root);
    }
    else
    {
        delete_merge(node, index, index + 1, root);
        delete_internal_node(node->children[index], key, t - 1, root);
    }
}

std::pair<int, size_t> BTreeNode::search(int dni)
{
    int i = 0;
    while (i < n && keys[i].first < dni)
    {
        i++;
    }
    if (i < n && keys[i].first == dni)
    {
        return keys[i];
    }
    if (leaf)
    {
        return { -1, 0 }; // Indicador de que la clave no se encontr�
    }
    return children[i]->search(dni);
}

bool BTreeNode::isExisting(int dni)
{
    int i = 0;
    while (i < n && keys[i].first < dni)
    {
        i++;
    }
    if (i < n && keys[i].first == dni)
    {
        return true;
    }
    if (leaf)
    {
        return false;
    }
    return children[i]->isExisting(dni);
}