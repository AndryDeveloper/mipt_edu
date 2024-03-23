#ifndef __STRUCTS_METODS_H__
#define __STRUCTS_METODS_H__

LinkedList* insert_after(LinkedList* const current=nullptr, int init_val=0);
LinkedList* remove_after(LinkedList* const current);
LinkedList* reverse_rec(LinkedList * const head);
void erase(LinkedList * const head);

#endif