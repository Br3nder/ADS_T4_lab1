#ifndef RBNODE_H
#define RBNODE_H

/*
  K - Key, V - Value
  color - black = 0, red = 1
*/

template<class K, class V>
struct  RBTNode
{
    K key;
    V value;
    bool color;
    RBTNode<K, V> * left;
    RBTNode<K, V> * right;
    RBTNode<K, V> * parent;
    RBTNode(K k, V v, bool c, RBTNode* p, RBTNode*l, RBTNode*r) :
        key(k), value(v), color(c), parent(p), left(l), right(r) { };
};

#endif // RBNODE_H
