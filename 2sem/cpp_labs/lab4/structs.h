#ifndef __STRUCTS_H__
#define __STRUCTS_H__

struct BinaryTree
{
    BinaryTree *left;
    BinaryTree *right;
    int key;
    int value;
};

struct AVLTree
{
	int key;
	unsigned char height;
	AVLTree* left;
	AVLTree* right;
	AVLTree(int k) { key = k; left = right = nullptr; height = 1; }
};

struct ListPair {
    ListPair *tail;
    int p;
};

struct Table
{
    ListPair **table;
    float load_factor;
    unsigned size;
};


#endif