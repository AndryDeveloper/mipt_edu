#ifndef __STRUCTS_METODS_H__
#define __STRUCTS_METODS_H__

DynamicArray new_array(unsigned size, int init_val = 0);
void erase(DynamicArray &da);
DynamicArray& reserve(DynamicArray &da, unsigned capacity);
DynamicArray clone(DynamicArray &da);
DynamicArray& assign(DynamicArray const &src, DynamicArray &dst);
DynamicArray& push_back(DynamicArray &da, int val);

BinaryTree* find(BinaryTree* head, int key);
BinaryTree* insert(BinaryTree* head, int key, int value=0);
BinaryTree* remove(BinaryTree* head, int key);

#endif