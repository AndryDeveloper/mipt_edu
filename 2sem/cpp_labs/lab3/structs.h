#ifndef __STRUCTS_H__
#define __STRUCTS_H__

struct LinkedList
{
    int data;
    LinkedList *next;
};

struct RingBuffer
{
    int data;
    RingBuffer *next;
};

// struct DynamicArray;
// DynamicArray new_array(unsigned size, int init_val = 0);
// void erase(DynamicArray &da);
// DynamicArray& reserve(DynamicArray &da, unsigned capacity);
// DynamicArray clone(DynamicArray &da);
// DynamicArray& assign(DynamicArray const &src, DynamicArray &dst);
// DynamicArray& push_back(DynamicArray &da, int val);

#endif