#include "structs.h"
#include "config.h"

#include <iostream>

BinaryTree *find(BinaryTree *head, int key)
{
    if (head == nullptr) return nullptr;
    else if (head->key == key) return head;
    else if (head->key > key) return find(head->right, key);
    else return find(head->left, key);
}

BinaryTree *insert(BinaryTree *head, int key, int value = 0)
{
    if (head == nullptr) return new BinaryTree{nullptr, nullptr, key, value};
    else if (head->key == key) head->key = value;
    else if (head->key > key) head->right = insert(head->right, key, value);
    else head->left = insert(head->left, key, value);
    return head;
}

BinaryTree *remove(BinaryTree *head, int key)
{
    if (head == nullptr) return nullptr;
    else if (head->key > key) head->right = remove(head->right, key);
    else if (head->key < key) head->left = remove(head->left, key);
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

DynamicArray new_array(unsigned size, int init_val = 0)
{
    DynamicArray result;
    result.size = size;
    result.capacity = size;
    result.elements = new int[result.capacity];
    for (unsigned i = 0; i < result.size; i++)
    {
        result.elements[i] = init_val;
    }
    return result;
}

void erase(DynamicArray &da)
{
    delete[] da.elements;
    da.elements = nullptr;
}

DynamicArray &reserve(DynamicArray &da, unsigned capacity)
{
    if (capacity <= da.capacity)
    {
        return da;
    }
    int *elements = new int[capacity];
    for (unsigned i = 0; i < da.size; i++)
    {
        elements[i] = da.elements[i];
    }

    erase(da);
    da.elements = elements;
    da.capacity = capacity;

    return da;
}

DynamicArray clone(DynamicArray &da)
{
    DynamicArray result;
    result.size = da.size;
    result.capacity = da.size;
    result.elements = new int[result.capacity];

    for (unsigned i = 0; i < result.size; i++)
    {
        result.elements[i] = da.elements[i];
    }
    return result;
}

DynamicArray &assign(DynamicArray const &src, DynamicArray &dst)
{
    dst = reserve(dst, src.size);
    for (unsigned i = 0; i < src.size; i++)
    {
        dst.elements[i] = src.elements[i];
    }
    return dst;
}

DynamicArray &push_back(DynamicArray &da, int val)
{
    if (da.size + 1 > da.capacity)
    {
        da = reserve(da, da.size * 2);
    }
    da.elements[da.size] = val;
    da.size += 1;
    return da;
}