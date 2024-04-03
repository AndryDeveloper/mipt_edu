#ifndef __STRUCTS_METODS_H__
#define __STRUCTS_METODS_H__

Table new_table(unsigned size);
void erase_table(Table *t);
void add(Table* t, int p);
bool contains(Table* t, int p);
void remove(Table* t, int p);

BinaryTree* find(BinaryTree* head, int key);
BinaryTree* insert(BinaryTree* head, int key, int value=0);
BinaryTree* remove(BinaryTree* head, int key);

AVLTree *insert(AVLTree *p, int k);
AVLTree *remove(AVLTree *p, int k);
AVLTree* find(AVLTree *p, int k);

#endif