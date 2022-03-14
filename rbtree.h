#ifndef RBTREE_H
#define RBTREE_H
#include "rbnode.h"

/*
    insert - insert new item using a key<K> and a new value<V>
    remove - remove an item using a key<K>
    public search - adapter for user, which only requires a key<K>. (~ private remove(root, key<K>))
    print - traversal and print the tree using preOrder
    root - main node of the rbtree
*/

template<class V, class K>
class  RBTree
{
public:
    RBTree();
    ~RBTree();

    void insert(K key, V value);
    void remove(K key);
    RBTNode<V, K>* search(K key);
    void print();
    RBTNode<V, K>*root;

private:
    void leftRotate(RBTNode<V, K>* &root, RBTNode<V, K>* x);// левая рука
    void rightRotate(RBTNode<V, K>* &root, RBTNode<V, K>* y);// правая рука
    void insert(RBTNode<V, K>* &root, RBTNode<V, K>* node);// Вставляем узел, внутренний интерфейс
    void remove(RBTNode<V, K>*& root, RBTNode<V, K>*node);
    void insertRestoration(RBTNode<V, K>* &root, RBTNode<V, K>* node);
    void removeRestoration(RBTNode<V, K>* &root, RBTNode<V, K>* node, RBTNode<V, K>*parent);
    RBTNode<V, K>* search(RBTNode<V, K>*node, K key);
    void destory(RBTNode<V, K>* &node);
    void preOrder(RBTNode<V, K>* tree);
    void print(RBTNode<V, K>* node); // сделать перегрузку оператора
};

#endif // RBTREE_H
