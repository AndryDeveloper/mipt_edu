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

struct DynamicArray
{
    int *elements = nullptr;
    unsigned capacity = 0;
    unsigned size = 0;
};

#endif