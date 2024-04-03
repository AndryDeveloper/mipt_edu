#include "structs.h"
#include "config.h"

union Hash_int
{
    int i;
    unsigned u;
};

unsigned get_hash(int a)
{
    Hash_int d;
    d.i = a;
    return d.u;
}

void delete_after(ListPair *head)
{
    if (head != nullptr)
    {
        ListPair *tail = head->tail;
        head->tail = head->tail->tail;
        delete tail;
    }
}

void erase_list(ListPair *head)
{
    if (head != nullptr)
    {
        while (head->tail != nullptr)
        {
            ListPair *tail = head->tail;
            delete head;
            head = tail;
        }
        delete head;
    }
}

ListPair *add_before_list(ListPair *head, int p)
{
    return new ListPair{head, p};
}

Table new_table(unsigned size)
{
    ListPair **table = new ListPair *[size];
    for (unsigned i = 0; i < size; i++)
    {
        table[i] = nullptr;
    }
    return Table{table, 0.f, size};
}

bool contains(Table *t, int p)
{
    unsigned idx = get_hash(p) % t->size;
    ListPair *head = t->table[idx];
    bool found = false;
    while (nullptr != head && !found)
    {
        found = head->p == p;
        head = head->tail;
    }
    return found;
}

void remove(Table *t, int p)
{
    unsigned idx = get_hash(p) % t->size;
    ListPair *head = t->table[idx];
    if (head != nullptr)
    {
        if (head->p == p)
        {
            delete head;
            t->table[idx] = nullptr;
        }
        else
        {
            bool found = false;
            while (nullptr != head->tail && !found)
            {
                found = head->tail->p == p;
                head = head->tail;
            }
            if (found)
            {
                delete_after(head);
            }
        }
    }
}

void erase_table(Table *t)
{
    for (unsigned i = 0; i < t->size; i++)
    {
        erase_list((t->table)[i]);
    }
    delete[] t->table;
}

void add_element(Table *t, int p)
{
    unsigned idx = get_hash(p) % t->size;
    ListPair *head = t->table[idx];
    bool found = false;
    while (nullptr != head && !found)
    {
        found = head->p == p;
        head = head->tail;
    }
    if (!found)
    {
        t->table[idx] = add_before_list(t->table[idx], p);
        t->load_factor += 1.f / t->size;
    }
}

void expand_table(Table *t, unsigned new_size)
{
    Table empty_table = new_table(new_size);
    for (unsigned i = 0; i < t->size; i++)
    {
        ListPair *head = t->table[i];
        while (nullptr != head)
        {
            add_element(&empty_table, head->p);
            head = head->tail;
        }
    }
    erase_table(t);
    *t = empty_table;
}

void add(Table *t, int p)
{
    add_element(t, p);
    if (t->load_factor > 1)
    {
        expand_table(t, t->size * 2);
    }
}

BinaryTree *find(BinaryTree *head, int key)
{
    if (head == nullptr)
        return nullptr;
    else if (head->key == key)
        return head;
    else if (head->key > key)
        return find(head->right, key);
    else
        return find(head->left, key);
}

BinaryTree *insert(BinaryTree *head, int key, int value = 0)
{
    if (head == nullptr)
        return new BinaryTree{nullptr, nullptr, key, value};
    else if (head->key == key)
        head->key = value;
    else if (head->key > key)
        head->right = insert(head->right, key, value);
    else
        head->left = insert(head->left, key, value);
    return head;
}

BinaryTree *remove(BinaryTree *head, int key)
{
    if (head == nullptr)
        return nullptr;
    else if (head->key > key)
        head->right = remove(head->right, key);
    else if (head->key < key)
        head->left = remove(head->left, key);
    else
    {
        if (head->left == nullptr && head->right == nullptr)
        {
            delete head;
            return nullptr;
        }
        else if (head->right == nullptr)
        {
            BinaryTree *left = head->left;
            delete head;
            return left;
        }
        else if (head->left == nullptr)
        {
            BinaryTree *right = head->right;
            delete head;
            return right;
        }
        else
        {
            if (head->right->left == nullptr)
            {
                head->value = head->right->value;
                head->key = head->right->key;
                BinaryTree *right = head->right;
                head->right = head->right->right;
                delete right;
            }
            else
            {
                BinaryTree *node = head->right;
                while (node->left->left != nullptr)
                {
                    node = node->left;
                }
                head->value = node->left->value;
                head->key = node->left->key;
                node->left = remove(node->left, node->left->key);
            }
        }
    }
    return head;
}

unsigned char height(AVLTree *p)
{
    return (p != nullptr) ? p->height : 0;
}

int bfactor(AVLTree *p)
{
    return height(p->right) - height(p->left);
}

void fixheight(AVLTree *p)
{
    unsigned char hl = height(p->left);
    unsigned char hr = height(p->right);
    p->height = (hl > hr ? hl : hr) + 1;
}

AVLTree *rotateright(AVLTree *p)
{
    AVLTree *q = p->left;
    p->left = q->right;
    q->right = p;
    fixheight(p);
    fixheight(q);
    return q;
}

AVLTree *rotateleft(AVLTree *q)
{
    AVLTree *p = q->right;
    q->right = p->left;
    p->left = q;
    fixheight(q);
    fixheight(p);
    return p;
}

AVLTree *balance(AVLTree *p)
{
    fixheight(p);
    if (bfactor(p) == 2)
    {
        if (bfactor(p->right) < 0)
            p->right = rotateright(p->right);
        return rotateleft(p);
    }
    if (bfactor(p) == -2)
    {
        if (bfactor(p->left) > 0)
            p->left = rotateleft(p->left);
        return rotateright(p);
    }
    return p;
}

AVLTree *insert(AVLTree *p, int k)
{
    if (p == nullptr)
        return new AVLTree(k);
    if (k < p->key)
        p->left = insert(p->left, k);
    else
        p->right = insert(p->right, k);
    return balance(p);
}

AVLTree* find(AVLTree *p, int k)
{
    if (p == nullptr) return nullptr;
    else if (p->key == k) return p;
    else if (k < p->key) return find(p->left, k);
    else return find(p->right, k);
}

AVLTree *findmin(AVLTree *p)
{
    return (p->left != nullptr) ? findmin(p->left) : p;
}

AVLTree *removemin(AVLTree *p)
{
    if (p->left == nullptr)
        return p->right;
    p->left = removemin(p->left);
    return balance(p);
}

AVLTree *remove(AVLTree *p, int k)
{
    if (p == nullptr)
        return nullptr;
    if (k < p->key)
        p->left = remove(p->left, k);
    else if (k > p->key)
        p->right = remove(p->right, k);
    else //  k == p->key
    {
        AVLTree *q = p->left;
        AVLTree *r = p->right;
        delete p;
        if (r == nullptr)
            return q;
        AVLTree *min = findmin(r);
        min->right = removemin(r);
        min->left = q;
        return balance(min);
    }
    return balance(p);
}