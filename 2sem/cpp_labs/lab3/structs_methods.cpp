#include "structs.h"

LinkedList* insert_after(LinkedList* const current=nullptr, int init_val=0){
    if (current == nullptr){
        return new LinkedList {init_val, nullptr};
    }
    current->next = new LinkedList {init_val, nullptr};
    return current->next;
}

LinkedList* remove_after(LinkedList* const current){
    if (nullptr == current->next) return nullptr;
    LinkedList* new_follower = current->next->next;
    delete current->next;
    current->next = new_follower;
    return new_follower;
}

void erase(LinkedList * const head){
    LinkedList* follower = head->next;
    do{
        follower = remove_after(head);
    } while (follower != nullptr);
    delete head;
}

LinkedList* reverse_rec(LinkedList * const head){
    if (head->next == nullptr){
        return head;
    }
    LinkedList* current = head;
    LinkedList* next = current->next;
    head->next = nullptr;
    while (next != nullptr){
        LinkedList* fnext = next->next;
        next->next = current;
        current = next;
        next = fnext;
    }
    return current;
}

RingBuffer* insert_after(RingBuffer* const current=nullptr, int init_val=0){
    if (current == nullptr){
        RingBuffer* result = new RingBuffer;
        result->data = init_val;
        result->next = result;
        return result;
    }
    RingBuffer* previous_tail = current->next;
    current->next = new RingBuffer {init_val, previous_tail};
    return current->next;
}

RingBuffer* remove_after(RingBuffer* const current){
    if (current->next == current) {
        delete current;
        return nullptr;
    }
    RingBuffer* new_follower = current->next->next;
    delete current->next;
    current->next = new_follower;
    return new_follower;
}

void erase(RingBuffer * const head){
    RingBuffer* follower = head->next;
    do{
        follower = remove_after(head);
    } while (follower != nullptr);
}




// struct DynamicArray
// {
//     int *elements = nullptr;
//     unsigned capacity = 0;
//     unsigned size = 0;
// };

// DynamicArray new_array(unsigned size, int init_val = 0){
//     DynamicArray result;
//     result.size = size;
//     result.capacity = size;
//     result.elements = new int[result.capacity]{init_val};
//     return result;
// }

// void erase(DynamicArray &da){
//     delete [] da.elements;
// }

// DynamicArray& reserve(DynamicArray &da, unsigned capacity){
//     if (capacity <= da.capacity){
//         return da;
//     }
//     int* elements = new int[capacity];
//     for (unsigned i = 0; i < da.size; i++){
//         elements[i] = da.elements[i];
//     }
//     erase(da);
//     da.elements = elements;
//     return da;
// }

// DynamicArray clone(DynamicArray &da){
//     DynamicArray result;
//     result.size = da.size;
//     result.capacity = da.size;
//     result.elements = new int[result.capacity];

//     for (unsigned i = 0; i < result.size; i++){
//         result.elements[i] = da.elements[i];
//     }
//     return result;
// }

// DynamicArray& assign(DynamicArray const &src, DynamicArray &dst){
//     dst = reserve(dst, src.size);
//     for (unsigned i = 0; i < src.size; i++){
//         dst.elements[i] = src.elements[i];
//     }
//     return dst;
// }

// DynamicArray& push_back(DynamicArray &da, int val){
//     if (da.size + 1 > da.capacity){
//         da = reserve(da, da.size + 5);
//     }
//     da.elements[da.size + 1] = val;
//     return da;
// }