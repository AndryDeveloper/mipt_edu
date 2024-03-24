#ifndef __STRUCTS_METODS_H__
#define __STRUCTS_METODS_H__

LinkedList* insert_after(LinkedList* const current=nullptr, int init_val=0);
LinkedList* remove_after(LinkedList* const current);
LinkedList* reverse_rec(LinkedList * const head);
void erase(LinkedList * const head);

RingBuffer* remove_after_b(RingBuffer* const current);
RingBuffer* insert_after_b(RingBuffer* const current=nullptr, int init_val=0);
void erase(RingBuffer * const head);

DynamicArray new_array(unsigned size, int init_val = 0);
void erase(DynamicArray &da);
DynamicArray& reserve(DynamicArray &da, unsigned capacity);
DynamicArray clone(DynamicArray &da);
DynamicArray& assign(DynamicArray const &src, DynamicArray &dst);
DynamicArray& push_back(DynamicArray &da, int val, int strategy);

#endif