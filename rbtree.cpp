#include "rbtree.h"

template<class V, class K>		//Конструктор
RBTree<V, K>::RBTree() :root(nullptr) {
    root = nullptr;
}
template<class V, class K>		// Деструктор
RBTree<V, K>::~RBTree() {
    destory(root);
}
template<class V, class K>		//Оставил
void RBTree<V, K>::leftRotate(RBTNode<V, K>* &root, RBTNode<V, K>* x) {
    RBTNode<V, K>*y = x->right;
    x->right = y->left;
    if (y->left != nullptr)
        y->left->parent = x;

    y->parent = x->parent;
    if (x->parent == nullptr)
        root = y;
    else {
        if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
};
template<class V, class K>		//Правая рука
void RBTree<V, K>::rightRotate(RBTNode<V, K>*&root, RBTNode<V, K>*y) {
    RBTNode<V, K>*x = y->left;
    y->left = x->right;
    if (x->right != nullptr)
        x->right->parent = y;

    x->parent = y->parent;
    if (y->parent == nullptr)
        root = x;
    else {
        if  (y == y->parent->right)
            y->parent->right = x;
        else
            y->parent->left = x;
    }
    x->right = y;
    y->parent = x;
};
template<class V, class K>		// вставка
void RBTree<V, K>::insert(K key, V value)
{
    RBTNode<V, K>*z = new RBTNode<V, K>(key, value, true, nullptr, nullptr, nullptr);
    insert(root, z);
};
template<class V, class K>
void RBTree<V, K>::insert(RBTNode<V, K>* &root, RBTNode<V, K>* node)
{
    RBTNode<V, K> *x = root;
    RBTNode<V, K> *y = nullptr;
    while (x != nullptr)
    {
        y = x;
        if (node->key > x->key)
            x = x->right;
        else
            x = x->left;
    }
    node->parent = y;
    if(y!=nullptr)
    {
        if (node->key > y->key)
            y->right = node;
        else
            y->left = node;
    }
    else
        root = node;
    node->color = true;
    insertRestoration(root, node);
};

template<class V, class K>
void RBTree<V, K>::insertRestoration(RBTNode<V, K>* &root, RBTNode<V, K>* node)
{
    RBTNode<V, K>*parent;
    parent = node->parent;
    while (node != RBTree::root  && parent->color == true)
    {
        RBTNode<V, K>*gparent = parent->parent;
        if (gparent->left == parent)
        {
            RBTNode<V, K>*uncle = gparent->right;
            if (uncle != nullptr && uncle->color == true)
            {
                parent->color = false;
                uncle->color = false;
                gparent->color = true;
                node = gparent;
                parent = node->parent;
            }
            else
            {
                if (parent->right == node)
                {
                    leftRotate(root, parent);
                    swap(node, parent);
                }
                rightRotate(root, gparent);
                gparent->color = true;
                parent->color = false;
                break;
            }
        }
        else
        {
            RBTNode<V, K>*uncle = gparent->left;
            if (uncle != nullptr && uncle->color == true)
            {
                gparent->color = true;
                parent->color = false;
                uncle->color = false;

                node = gparent;
                parent = node->parent;
            }
            else
            {
                if (parent->left == node)
                {
                    rightRotate(root, parent);
                    swap(parent, node);
                }
                leftRotate(root, gparent);
                parent->color = false;
                gparent->color = true;
                break;
            }
        }
    }
    root->color = false;
}

template<class V, class K>
// Уничтожить красные и черные деревья
void RBTree<V, K>::destory(RBTNode<V, K>* &node)
{
    if (node == nullptr)
        return;
    destory(node->left);
    destory(node->right);
    delete node;
    node = nullptr;
}

template<class V, class K>
void RBTree<V, K>::remove(K key)
{
    RBTNode<V, K>*deletenode = search(root,key);
    if (deletenode != nullptr)
        remove(root, deletenode);
}
template<class V, class K>
void RBTree<V, K>::remove(RBTNode<V, K>*&root, RBTNode<V, K>*node)
{
    RBTNode<V, K> *child, *parent;
    bool color;
    // Левый и правый узлы удаленного узла не пусты (не конечные узлы)
    if (node->left != nullptr && node->right != nullptr)
    {
        RBTNode<V, K> *replace = node;
        // Найти узел-преемник (самый нижний левый узел правого поддерева текущего узла)
        replace = node->right;
        while (replace->left != nullptr)
        {
            replace = replace->left;
        }
        // Случай, когда удаленный узел не является корневым узлом
        if (node->parent != nullptr)
        {
            if (node->parent->left == node)
                node->parent->left = replace;
            else
                node->parent->right = replace;
        }
        // Ситуация с корневым узлом
        else
            root = replace;
        // child - это правильный узел, который заменяет узел и является узлом, который требует последующей корректировки
        // Поскольку замена является преемником, он не может иметь левого дочернего узла
        // Аналогично, у узла-предшественника не может быть правого дочернего узла
        child = replace->right;
        parent = replace->parent;
        color = replace->color;

        // Удаленный узел является родительским узлом замещающего узла (repalce)
        if (parent == node)
            parent = replace;
        else
        {
            // Существование дочернего узла
            if (child != nullptr)
                child->parent = parent;
            parent->left = child;

            replace->right = node->right;
            node->right->parent = replace;
        }
        replace->parent = node->parent;
        replace->color = node->color;
        replace->left = node->left;
        node->left->parent = replace;
        if (color == false)
            removeRestoration(root, child, parent);

        delete node;
        return;
    }
    // Когда в удаленном узле только левый (правый) узел пуст, найдите дочерний узел удаленного узла
    if (node->left != nullptr)
        child = node->left;
    else
        child = node->right;

    parent = node->parent;
    color = node->color;
    if (child)
    {
        child->parent = parent;
    }
    // Удаленный узел не является корневым узлом
    if (parent)
    {
        if (node == parent->left)
            parent->left = child;
        else
            parent->right = child;
    }
    // Удаленный узел является корневым узлом
    else
        RBTree::root = child;

    if (color == false)
    {
        removeRestoration(root, child, parent);
    }
    delete node;

}

template<class V, class K>
void RBTree<V, K>::removeRestoration(RBTNode<V, K>* &root, RBTNode<V, K>* node,RBTNode<V, K>*parent)
{
    RBTNode<V, K>*othernode;
    while ((!node) || (node->color == false && node != RBTree::root))
    {
        if (parent->left == node)
        {
            othernode = parent->right;
            if (othernode->color == true)
            {
                othernode->color = false;
                parent->color = true;
                leftRotate(root, parent);
                othernode = parent->right;
            }
            else
            {
                if (!(othernode->right) || othernode->right->color == false)
                {
                    othernode->left->color=false;
                    othernode->color = true;
                    rightRotate(root, othernode);
                    othernode = parent->right;
                }
                othernode->color = parent->color;
                parent->color = false;
                othernode->right->color = false;
                leftRotate(root, parent);
                node = root;
                break;
            }
        }
        else
        {
            othernode = parent->left;
            if (othernode->color == true)
            {
                othernode->color = false;
                parent->color = true;
                rightRotate(root, parent);
                othernode = parent->left;
            }
            if ((!othernode->left || othernode->left->color == false) && (!othernode->right || othernode->right->color == false))
            {
                othernode->color = true;
                node = parent;
                parent = node->parent;
            }
            else
            {
                if (!(othernode->left) || othernode->left->color == false)
                {
                    othernode->right->color = false;
                    othernode->color = true;
                    leftRotate(root, othernode);
                    othernode = parent->left;
                }
                othernode->color = parent->color;
                parent->color = false;
                othernode->left->color = false;
                rightRotate(root, parent);
                node = root;
                break;
            }
        }
    }
    if (node)
        node->color = false;
}

template<class V, class K>
RBTNode<V, K>* RBTree<V, K>::search(K key)
{
    return search(root, key);
}
template<class V, class K>
RBTNode<V, K>* RBTree<V, K>::search(RBTNode<V, K>*node, K key)
{
    if (node == nullptr || node->key == key)
        return node;
    else
        if (key > node->key)
            return search(node->right, key);
        else
            return search(node->left, key);
}

template<class V, class K>
void RBTree<V, K>::preOrder(RBTNode<V, K>* tree){
        if (tree != nullptr) {
            //cout << tree->key << " ";
            preOrder(tree->left);
            preOrder(tree->right);
        }
    }
