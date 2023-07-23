#pragma once

#include <functional> // std::less
#include <iostream>
#include <queue>   // std::queue
#include <utility> // std::pair
#define TREE_ASSERT_PRINT
#define TREE_ASSERT_PRINT_SZ_LIMIT 50

template <typename K, typename V, typename Comparator = std::less<K>>
class BinarySearchTree
{
public:
    using key_type = K;
    using value_type = V;
    using key_compare = Comparator;
    using pair = std::pair<key_type, value_type>;
    using pointer = pair *;
    using const_pointer = const pair *;
    using reference = pair &;
    using const_reference = const pair &;
    using difference_type = ptrdiff_t;
    using size_type = size_t;

private:
    struct BinaryNode
    {
        pair element;
        BinaryNode *left;
        BinaryNode *right;

        BinaryNode(const_reference theElement, BinaryNode *lt, BinaryNode *rt)
            : element{theElement}, left{lt}, right{rt} {}

        BinaryNode(pair &&theElement, BinaryNode *lt, BinaryNode *rt)
            : element{std::move(theElement)}, left{lt}, right{rt} {}
    };

    using node = BinaryNode;
    using node_ptr = node *;
    using const_node_ptr = const node *;

    node_ptr _root;
    size_type _size;
    key_compare comp;

public:
    BinarySearchTree() : _root{nullptr}, _size(0), comp{}
    {
        // _root->left = nullptr;
        // _root->right = nullptr;
    }
    BinarySearchTree(const BinarySearchTree &rhs)
    {

        _root = clone(const_node_ptr(rhs._root));
        _size = rhs._size;
    }
    BinarySearchTree(BinarySearchTree &&rhs)
    {
        _size = rhs._size;
        _root = std::move(rhs._root);
        rhs._root = nullptr;
    }
    ~BinarySearchTree()
    {
        clear();
    }

    const_reference min() const { return min(_root)->element; }
    const_reference max() const { return max(_root)->element; }
    const_reference root() const
    {
        return const_reference(_root->element);
    }
    bool contains(const key_type &x) const { return contains(x, _root); }
    value_type &find(const key_type &key) { return find(key, _root)->element.second; }
    const value_type &find(const key_type &key) const { return find(key, _root)->element.second; }
    bool empty() const
    {
        return _size == 0;
    }
    size_type size() const
    {
        return _size;
    }

    void clear()
    {
        clear(_root);
        _size = 0;
    }
    void insert(const_reference x) { insert(x, _root); }
    void insert(pair &&x) { insert(std::move(x), _root); }
    void erase(const key_type &x) { erase(x, _root); }

    BinarySearchTree &operator=(const BinarySearchTree &rhs)
    {
        if (_root == rhs._root)
            return *this;
        clear();
        _root = clone(const_node_ptr(rhs._root));
        _size = rhs._size;
        return *this;
    }
    BinarySearchTree &operator=(BinarySearchTree &&rhs)
    {
        if (_root == rhs._root)
            return *this;
        clear();
        _size = rhs._size;
        _root = std::move(rhs._root);
        rhs._root = nullptr;

        return *this;
    }

private:
    void insert(const_reference x, node_ptr &t)
    {
        if (t == nullptr)
        {
            // node *newNode = new node(x, nullptr, nullptr);
            t = new node(x, nullptr, nullptr);
            _size++;
            // t->element.second = x.second;
            // return;
        }

        // recursively call the funciton again until t == nullptr (i.e. leaf node of the tree)
        else if (comp(x.first, t->element.first))
            insert(x, t->left);
        else if (comp(t->element.first, x.first))
            insert(x, t->right);
        else
        {
            t->element.second = x.second;
            //_size++;
        }
    }
    void insert(pair &&x, node_ptr &t)
    {
        if (t == nullptr)
        {
            t = new node(std::move(x), nullptr, nullptr);
            _size++;
        }

        // recursively call the insert function again using move semantics until t == nullptr (i.e will be leaf node
        // at the tree)

        else if (comp(x.first, t->element.first))
            insert(std::move(x), t->left);
        else if (comp(t->element.first, x.first))
            insert(std::move(x), t->right);
        else
        {
            t->element.second = std::move(x.second);
        }
    }

    void erase(const key_type &x, node_ptr &t)
    {
        if (!t)
            return;
        if (comp(x, t->element.first))
            erase(x, t->left);
        else if (comp(t->element.first, x))
            erase(x, t->right);
        else if ((t->left == nullptr) && (t->right == nullptr))
        {
            delete t;
            t = nullptr;
            _size--;
        }
        else if (t->right == nullptr && (t->left != nullptr))
        {
            node_ptr temp = t;
            t = t->left;
            delete temp;
            temp = nullptr;
            _size--;
        }
        else if ((t->left == nullptr) && (t->right != nullptr))
        {
            node_ptr temp = t;
            t = t->right;
            delete temp;
            temp = nullptr;
            _size--;
        }
        else
        {
            t->element.first = min(t->right)->element.first;
            t->element.second = min(t->right)->element.second;
            erase(min(t->right)->element.first, t->right);
        }
    }

    const_node_ptr min(const_node_ptr t) const
    {
        // if (t->left != nullptr) {
        //     min(t->left);
        // }

        // return t;

        if (t->left == nullptr)
            return t;
        min(t->left);
    }
    const_node_ptr max(const_node_ptr t) const
    {
        if (t->right == nullptr)
            return t;
        max(t->right);
    }

    bool contains(const key_type &x, const_node_ptr t) const
    {
        return find(x, t);
        // TODO
    }
    node_ptr find(const key_type &key, node_ptr t)
    {
        if (!t)
            return nullptr;
        if (comp(key, t->element.first))
            find(key, t->left);
        else if (comp(t->element.first, key))
            find(key, t->right);
        else
            return t;
    }
    const_node_ptr find(const key_type &key, const_node_ptr t) const
    {
        if (!t)
            return nullptr;
        if (comp(key, t->element.first))
            find(key, t->left);
        else if (comp(t->element.first, key))
            find(key, t->right);
        else
            return t;
    }

    void clear(node_ptr &t)
    {
        if (t != nullptr)
        {
            clear(t->left);
            clear(t->right);
            delete t;
        }
        t = nullptr;
        _size = 0;
    }

    node_ptr clone(const_node_ptr t) const
    {
        if (!t)
            return nullptr;
        else
        {
            node *newNode = new node(t->element, nullptr, nullptr);
            if (t->left != nullptr)
                newNode->left = clone(t->left);
            if (t->right != nullptr)
                newNode->right = clone(t->right);

            return newNode;
        }
    }

public:
    template <typename KK, typename VV, typename CC>
    friend void printLevelByLevel(const BinarySearchTree<KK, VV, CC> &bst, std::ostream &out);

    template <typename KK, typename VV, typename CC>
    friend std::ostream &printNode(std::ostream &o, const typename BinarySearchTree<KK, VV, CC>::node &bn);

    template <typename KK, typename VV, typename CC>
    friend void printTree(const BinarySearchTree<KK, VV, CC> &bst, std::ostream &out);

    template <typename KK, typename VV, typename CC>
    friend void printTree(typename BinarySearchTree<KK, VV, CC>::const_node_ptr t, std::ostream &out, unsigned depth);

    template <typename KK, typename VV, typename CC>
    friend void vizTree(
        typename BinarySearchTree<KK, VV, CC>::const_node_ptr node,
        std::ostream &out,
        typename BinarySearchTree<KK, VV, CC>::const_node_ptr prev);

    template <typename KK, typename VV, typename CC>
    friend void vizTree(
        const BinarySearchTree<KK, VV, CC> &bst,
        std::ostream &out);
};

template <typename KK, typename VV, typename CC>
std::ostream &printNode(std::ostream &o, const typename BinarySearchTree<KK, VV, CC>::node &bn)
{
    return o << '(' << bn.element.first << ", " << bn.element.second << ')';
}

template <typename KK, typename VV, typename CC>
void printLevelByLevel(const BinarySearchTree<KK, VV, CC> &bst, std::ostream &out = std::cout)
{
    using node = typename BinarySearchTree<KK, VV, CC>::node;
    using node_ptr = typename BinarySearchTree<KK, VV, CC>::node_ptr;
    using const_node_ptr = typename BinarySearchTree<KK, VV, CC>::const_node_ptr;

    if (bst._root == nullptr)
        return;
    std::queue<node_ptr> q;
    q.push(bst._root);
    int elementsInLevel = 1;
    bool nonNullChild = false;
    while (elementsInLevel > 0)
    {
        node_ptr temp = q.front();
        q.pop();
        elementsInLevel--;

        // temp node isn't nullptr enquee the child nodes and check if either are not null
        if (temp != nullptr)
        {
            // out << temp->element;
            printNode<KK, VV, CC>(out, *temp);
            q.push(temp->left);
            q.push(temp->right);
            if (temp->left != nullptr || temp->right != nullptr)
                nonNullChild = true;
        }
        // temp node is nullptr so the child nodes will be null as well
        else
        {
            out << "null";
            q.push(nullptr);
            q.push(nullptr);
        }

        if (elementsInLevel == 0)
        {
            out << '\n';
            if (nonNullChild)
            {
                nonNullChild = false;
                elementsInLevel = q.size();
            }
        }
    }
}

template <typename KK, typename VV, typename CC>
void printTree(const BinarySearchTree<KK, VV, CC> &bst, std::ostream &out = std::cout) { printTree<KK, VV, CC>(bst._root, out); }

template <typename KK, typename VV, typename CC>
void printTree(typename BinarySearchTree<KK, VV, CC>::const_node_ptr t, std::ostream &out, unsigned depth = 0)
{
    if (t != nullptr)
    {
        printTree<KK, VV, CC>(t->right, out, depth + 1);
        for (unsigned i = 0; i < depth; ++i)
            out << '\t';
        printNode<KK, VV, CC>(out, *t) << '\n';
        printTree<KK, VV, CC>(t->left, out, depth + 1);
    }
}

template <typename KK, typename VV, typename CC>
void vizTree(
    typename BinarySearchTree<KK, VV, CC>::const_node_ptr node,
    std::ostream &out,
    typename BinarySearchTree<KK, VV, CC>::const_node_ptr prev = nullptr)
{
    if (node)
    {
        std::hash<KK> khash{};

        out << "\t"
               "node_"
            << (uint32_t)khash(node->element.first)
            << "[label=\"" << node->element.first
            << " [" << node->element.second << "]\"];" << std::endl;

        if (prev)
            out << "\tnode_" << (uint32_t)khash(prev->element.first) << " -> ";
        else
            out << "\t";

        out << "node_" << (uint32_t)khash(node->element.first) << ";" << std::endl;

        vizTree<KK, VV, CC>(node->left, out, node);
        vizTree<KK, VV, CC>(node->right, out, node);
    }
}

template <typename KK, typename VV, typename CC>
void vizTree(
    const BinarySearchTree<KK, VV, CC> &bst,
    std::ostream &out = std::cout)
{
    out << "digraph Tree {" << std::endl;
    vizTree<KK, VV, CC>(bst._root, out);
    out << "}" << std::endl;
}
