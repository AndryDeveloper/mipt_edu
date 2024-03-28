#include "structs.h"
#include "structs_methods.h"

int linked_list_solver(RingBuffer* const head, int k){
    RingBuffer* current = head;
    while (current->next != current){
        remove_after_b(current);
        for (int i = 0; i < k; i++){
            current = current->next;
        }
    }
    int data = current->data;
    delete current;
    return data;
}

int remove_after_arr(int* arr, int k, int n){
    if (k + 2 == n){
        return n - 1;
    }
    if (k + 1 == n){
        arr[0] = arr[1];
        return remove_after_arr(arr, 0, n);
    }
    arr[k + 1] = arr[k + 2];
    return remove_after_arr(arr, k + 1, n);
}

int arr_solver(int* arr, int n, int k){
    int current = 0;
    k %= n;
    while (n != 1){
        n = remove_after_arr(arr, current, n);
        current += k;
        current %= n;
    }
    return arr[0];
}