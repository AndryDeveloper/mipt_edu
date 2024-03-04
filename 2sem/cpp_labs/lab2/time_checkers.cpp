#include <algorithm>
#include <chrono>
#include <random>
#include "sorts.h"
#include "config.h"
using namespace std;

void check_shaker_time(unsigned *arr, 
unsigned N, 
default_random_engine& rng, 
float& time_full, 
float& time_forward, 
float& time_backward)
{   
    shuffle(&arr[0], &arr[ARR_LENGTH], rng);
    auto begin = chrono::steady_clock::now();
    unsigned iter = 0;
    for (unsigned i = 0; i < ARR_LENGTH - N; i += N){
        shaker_sort(arr, i, i + N - 1);
        iter++;
    }
    auto end = chrono::steady_clock::now();
    auto time_span = chrono::duration_cast<chrono::milliseconds>(end - begin);
    sort(arr, arr + MAX_N);
    time_full = static_cast<float>(time_span.count()) / static_cast<float>(iter);
    time_forward = 0.;
    time_backward = 0.;
}

void check_comb_time(unsigned *arr, 
unsigned N, 
default_random_engine& rng, 
float& time, 
float& iterations)
{   
    shuffle(&arr[0], &arr[ARR_LENGTH], rng);
    auto begin = chrono::steady_clock::now();
    unsigned iter = 0;
    unsigned permutations = 0;
    for (unsigned i = 0; i < ARR_LENGTH - N; i += N){
        permutations += comb_sort(arr, i, i + N - 1);
        iter++;
    }
    auto end = chrono::steady_clock::now();
    auto time_span = chrono::duration_cast<chrono::milliseconds>(end - begin);

    sort(arr, arr + MAX_N);
    time = static_cast<float>(time_span.count()) / static_cast<float>(iter);
    iterations = static_cast<float>(permutations) / static_cast<float>(iter);
}

void next_step_a(unsigned& s, unsigned& q){
    s /= 2;
    q = 0;
}

void next_step_b(unsigned& s, unsigned& q){
    if (q == 0 && s != 0){
        while ((1 << q) - 1 <= s) {
            q++;
        }
        if ((1 << q) - 1 > s){
            q--;
        }
        s = (1 << q) - 1;
    }
    else if (q != 0){
        s = (1 << (q - 1)) - 1;
        q -= 1;
    }
}

void next_step_c(unsigned& s, unsigned& q){
    if (q == 0){
        unsigned size = s;
        s = 1;
        q = 1;
        while (s + q <= size) {
            unsigned tmp = s;
            s += q;
            q = tmp;
        }
    }
    else if (q != 0){
        unsigned tmp = q;
        q = s - q;
        s = tmp;
        if (q == 0) {
            s = 0;
        }
    }
}

void check_shell_time(unsigned *arr, 
unsigned N, 
default_random_engine& rng, 
float& time1,
float& time2,
float& time3, 
float& iterations1,
float& iterations2,
float& iterations3)
{   
    // strategy 1
    shuffle(&arr[0], &arr[ARR_LENGTH], rng);
    auto begin = chrono::steady_clock::now();
    unsigned iter = 0;
    unsigned permutations = 0;
    for (unsigned i = 0; i < ARR_LENGTH - N; i += N){
        permutations += shell_sort(arr, i, i + N - 1, next_step_a);
        iter++;
    }
    auto end = chrono::steady_clock::now();
    auto time_span = chrono::duration_cast<chrono::milliseconds>(end - begin);

    sort(arr, arr + MAX_N);
    time1 = static_cast<float>(time_span.count()) / static_cast<float>(iter);
    iterations1 = static_cast<float>(permutations) / static_cast<float>(iter);

    // strategy 2
    shuffle(&arr[0], &arr[ARR_LENGTH], rng);
    begin = chrono::steady_clock::now();
    iter = 0;
    permutations = 0;
    for (unsigned i = 0; i < ARR_LENGTH - N; i += N){
        permutations += shell_sort(arr, i, i + N - 1, next_step_b);
        iter++;
    }
    end = chrono::steady_clock::now();
    time_span = chrono::duration_cast<chrono::milliseconds>(end - begin);

    sort(arr, arr + MAX_N);
    time2 = static_cast<float>(time_span.count()) / static_cast<float>(iter);
    iterations2 = static_cast<float>(permutations) / static_cast<float>(iter);

    // strategy 3
    shuffle(&arr[0], &arr[ARR_LENGTH], rng);
    begin = chrono::steady_clock::now();
    iter = 0;
    permutations = 0;
    for (unsigned i = 0; i < ARR_LENGTH - N; i += N){
        permutations += shell_sort(arr, i, i + N - 1, next_step_c);
        iter++;
    }
    end = chrono::steady_clock::now();
    time_span = chrono::duration_cast<chrono::milliseconds>(end - begin);

    sort(arr, arr + MAX_N);
    time3 = static_cast<float>(time_span.count()) / static_cast<float>(iter);
    iterations3 = static_cast<float>(permutations) / static_cast<float>(iter);
}