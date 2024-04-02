#ifndef __STRUCTS_H__
#define __STRUCTS_H__

struct BinaryTree
{
    BinaryTree *left;
    BinaryTree *right;
    int key;
    int value;
};

struct DynamicArray
{
    int *elements = nullptr;
    unsigned capacity = 0;
    unsigned size = 0;
};

#endif