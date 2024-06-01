//#include <iostream>
//#include <vector>
//#include <fstream>
//#include <memory>
//#include "Ciudadano.h"
//
//using namespace std;
//
//class BTreeNode
//{
//
//    int t, n;
//    bool leaf;
//    vector<int> keys;
//    // vector<Ciudadano> keys;
//    vector<shared_ptr<BTreeNode>> children;
//
//public:
//    BTreeNode(int t, bool leaf)
//    {
//        this->t = t;
//        this->leaf = leaf;
//        this->n = 0;
//        this->keys.resize(2 * (t - 1));
//        this->children.resize(2 * t);
//    }
//
//    void traverse()
//    {
//        int i = 0;
//        while (i < n)
//        {
//            if (!leaf)
//            {
//                children[i]->traverse();
//            }
//
//            cout << " " << keys[i];
//            i++;
//        }
//
//        if (!leaf)
//        {
//            children[i]->traverse();
//        }
//    }
//
//    void InsertNonFull(int k)
//    {
//        int i = n - 1;
//        if (leaf)
//        {
//            while (i >= 0 && keys[i] > k)
//            {
//                keys[i + 1] = keys[i];
//                i--;
//            }
//
//            keys[i + 1] = k;
//            n++;
//        }
//        else
//        {
//            while (i >= 0 && keys[i] > k)
//            {
//                i--;
//            }
//
//            if (children[i + 1]->n == 2 * (t - 1))
//            {
//                SplitChild(i + 1, children[i + 1]);
//                if (keys[i + 1] < k)
//                {
//                    i++;
//                }
//            }
//            children[i + 1]->InsertNonFull(k);
//        }
//    }
//
//    void SplitChild(int i, const shared_ptr<BTreeNode>& y)
//    {
//        shared_ptr<BTreeNode> z = make_shared<BTreeNode>(y->t, y->leaf);
//        z->n = t - 1;
//
//        for (int j = 0; j < t - 1; j++)
//        {
//            z->keys[j] = y->keys[j + t];
//        }
//
//        if (!y->leaf)
//        {
//            for (int j = 0; j < t; j++)
//            {
//                z->children[j] = y->children[j + t];
//            }
//        }
//        y->n = t - 1;
//
//        for (int j = n; j >= i + 1; j--)
//        {
//            children[j + 1] = children[j];
//        }
//        children[i + 1] = z;
//
//        for (int j = n - 1; j >= i; j--)
//        {
//            keys[j + 1] = keys[j];
//        }
//        keys[i] = y->keys[t - 1];
//        n++;
//    }
//
//    friend class BTree;
//};
//
//class BTree
//{
//    shared_ptr<BTreeNode> root;
//    int t;
//
//public: 
//    BTree(int _t)
//    {
//        root = nullptr;
//        this->t = _t;
//    }
//
//    void traverse()
//    {
//        if (root != nullptr)
//        {
//            root->traverse();
//        }
//    }
//
//    void insert(int k)
//    {
//        if (root == nullptr)
//        {
//            root = make_shared<BTreeNode>(t, true);;
//            root->keys[0] = k;
//            root->n = 1;
//        }
//        else
//        {
//            // En caso de que la raiz este llena
//            if (root->n == 2 * (t - 1))
//            {
//                // Crear una raiz
//                shared_ptr<BTreeNode> s = make_shared<BTreeNode>(t, false);
//                s->children[0] = root;
//                s->SplitChild(0, root);
//                int i = 0;
//                if (s->keys[0] < k)
//                {
//                    i++;
//                }
//
//                s->children[i]->InsertNonFull(k);
//                root = s;
//            }
//            else
//            {
//                root->InsertNonFull(k);
//            }
//        }
//    }
//};
