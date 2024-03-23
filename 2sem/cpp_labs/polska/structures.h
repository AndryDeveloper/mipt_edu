#ifndef __STRUCTURES_H__
#define __STRUCTURES_H__


template <typename T>
struct DynamicArray
{
    T *elements = nullptr;
    unsigned capacity = 0;
    unsigned size = 0;
};

template <typename T>
DynamicArray<T> new_array(unsigned size, int init_val = 0){
    DynamicArray<T> result;
    result.size = size;
    result.capacity = size;
    result.elements = new T[result.capacity];
    for (unsigned i = 0; i < result.size; i++){
        result.elements[i] = 0;
    }
    return result;
}

template <typename T>
void erase(DynamicArray<T> &da){
    delete [] da.elements;
}

template <typename T>
DynamicArray<T>& reserve(DynamicArray<T> &da, unsigned capacity){
    if (capacity <= da.capacity){
        return da;
    }
    T* elements = new T[capacity];
    for (unsigned i = 0; i < da.size; i++){
        elements[i] = da.elements[i];
    }
    erase(da);
    da.elements = elements;
    return da;
}

template <typename T>
DynamicArray<T> clone(DynamicArray<T> &da){
    DynamicArray<T> result;
    result.size = da.size;
    result.capacity = da.size;
    result.elements = new T[result.capacity];

    for (unsigned i = 0; i < result.size; i++){
        result.elements[i] = da.elements[i];
    }
    return result;
}

template <typename T>
DynamicArray<T>& assign(DynamicArray<T> const &src, DynamicArray<T> &dst){
    dst = reserve(dst, src.size);
    for (unsigned i = 0; i < src.size; i++){
        dst[i] = src[i];
    }
    return dst;
}

template <typename T>
DynamicArray<T>& push_back(DynamicArray<T> &da, T val){
    if (da.size + 1 > da.capacity){
        da = reserve(da, da.size + 5);
    }
    da[da.size + 1] = val;
    return da;
}

#endif