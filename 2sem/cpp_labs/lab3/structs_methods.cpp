#include "structs.h"
#include "config.h"

LinkedList *insert_after(LinkedList *const current = nullptr, int init_val = 0)
{
    if (current == nullptr)
    {
        return new LinkedList{init_val, nullptr};
    }
    current->next = new LinkedList{init_val, nullptr};
    return current->next;
}

LinkedList *remove_after(LinkedList *const current)
{
    if (nullptr == current->next)
        return nullptr;
    LinkedList *new_follower = current->next->next;
    delete current->next;
    current->next = new_follower;
    return new_follower;
}

void erase(LinkedList *const head)
{
    LinkedList *follower = head->next;
    do
    {
        follower = remove_after(head);
    } while (follower != nullptr);
    delete head;
}

LinkedList *reverse_rec(LinkedList *const head)
{
    if (head->next == nullptr)
    {
        return head;
    }
    LinkedList *current = head;
    LinkedList *next = current->next;
    head->next = nullptr;
    while (next != nullptr)
    {
        LinkedList *fnext = next->next;
        next->next = current;
        current = next;
        next = fnext;
    }
    return current;
}

RingBuffer *insert_after_b(RingBuffer *const current = nullptr, int init_val = 0)
{
    if (current == nullptr)
    {
        RingBuffer *result = new RingBuffer;
        result->data = init_val;
        result->next = result;
        return result;
    }
    RingBuffer *previous_tail = current->next;
    current->next = new RingBuffer{init_val, previous_tail};
    return current->next;
}

RingBuffer *remove_after_b(RingBuffer *const current)
{
    if (current->next == current)
    {
        delete current;
        return nullptr;
    }
    RingBuffer *new_follower = current->next->next;
    delete current->next;
    current->next = new_follower;
    return new_follower;
}

void erase(RingBuffer *const head)
{   
    RingBuffer *follower = head->next;
    while (follower != head){
        follower = remove_after_b(head);
    }
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

DynamicArray &push_back(DynamicArray &da, int val, int strategy)
{
    if (da.size + 1 > da.capacity)
    {
        if (strategy == 0)
        {
            da = reserve(da, da.size + 1);
        }
        else if (strategy == 1)
        {
            da = reserve(da, da.size + ADD_K);
        }
        else
        {
            da = reserve(da, da.size * MUL_K);
        }
    }
    da.elements[da.size] = val;
    da.size += 1;
    return da;
}