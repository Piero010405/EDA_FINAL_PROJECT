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

void BTree::Delete(int k)
{
    if (!root)
        return;
    root->Delete(k, root);
    if (root->n == 0)
    {
        BTreeNode* tmp = root;
        if (root->leaf)
            root = nullptr;
        else
            root = root->children[0];
        delete tmp;
    }
}