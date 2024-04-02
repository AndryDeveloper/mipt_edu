#include <fstream>
#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>

#include "config.h"
#include "structs.h"
#include "structs_methods.h"

using namespace std;

void print_progress(double percentage)
{
    int val = static_cast<int>(percentage * 100);
    int lpad = static_cast<int>(percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    printf("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
    fflush(stdout);
}

void inorder(BinaryTree *head)
{
    if (head != nullptr)
    {
        inorder(head->left);
        cout << head->key << " -> ";
        inorder(head->right);
    }
}

void check_tree()
{
    
}

void tree_time()
{
    default_random_engine rng(SEED);
    ofstream output_file("data/tree_A.csv");
    output_file << "N"
                << ","
                << "insert_time_A" << ','
                << "find_time_A" << ','
                << "remove_time_A" << ','
                << "insert_time_B" << ','
                << "find_time_B" << ','
                << "remove_time_B"
                << "\n";
    BinaryTree *arr[N_REPEATS_T];
    for (int N = MIN_N_T; N < MAX_N_T; N += STEP_T)
    {
        print_progress(static_cast<double>(N) / static_cast<double>(MAX_N_T));

        // Вставка
        auto begin = chrono::steady_clock::now();
        for (int j = 0; j < N_REPEATS_T; j++)
        {
            arr[j] = nullptr;
            for (int i = 0; i < N; i++)
            {
                arr[j] = insert(arr[j], i);
            }
        }
        auto end = chrono::steady_clock::now();
        unsigned insert_time_A = (chrono::duration_cast<chrono::milliseconds>(end - begin)).count();

        // Поиск
        begin = chrono::steady_clock::now();
        for (int j = 0; j < N_REPEATS_T; j++)
        {
            for (int i = 0; i < N; i++)
            {
                find(arr[j], i);
            }
        }
        end = chrono::steady_clock::now();
        unsigned find_time_A = (chrono::duration_cast<chrono::milliseconds>(end - begin)).count();

        // Удаление
        begin = chrono::steady_clock::now();
        for (int j = 0; j < N_REPEATS_T; j++)
        {
            for (int i = 0; i < N; i++)
            {
                arr[j] = remove(arr[j], i);
            }
        }
        end = chrono::steady_clock::now();
        unsigned remove_time_A = (chrono::duration_cast<chrono::milliseconds>(end - begin)).count();

        // Неравномерные запросы
        int *requests = new int[N];
        for (int i = 0; i < N; i++)
        {
            requests[i] = i;
        }

        // Вставка
        shuffle(&requests[0], &requests[N], rng);
        begin = chrono::steady_clock::now();
        for (int j = 0; j < N_REPEATS_T; j++)
        {
            arr[j] = nullptr;
            for (int i = 0; i < N; i++)
            {
                arr[j] = insert(arr[j], requests[i]);
            }
        }
        end = chrono::steady_clock::now();
        unsigned insert_time_B = (chrono::duration_cast<chrono::milliseconds>(end - begin)).count();

        // Поиск
        begin = chrono::steady_clock::now();
        for (int j = 0; j < N_REPEATS_T; j++)
        {
            for (int i = 0; i < N; i++)
            {
                find(arr[j], requests[i]);
            }
        }
        end = chrono::steady_clock::now();
        unsigned find_time_B = (chrono::duration_cast<chrono::milliseconds>(end - begin)).count();

        // Удаление
        shuffle(&requests[0], &requests[N], rng);
        begin = chrono::steady_clock::now();
        for (int j = 0; j < N_REPEATS_T; j++)
        {
            for (int i = 0; i < N; i++)
            {
                arr[j] = remove(arr[j], requests[i]);
            }
        }
        end = chrono::steady_clock::now();
        unsigned remove_time_B = (chrono::duration_cast<chrono::milliseconds>(end - begin)).count();

        delete[] requests;

        output_file << N
                    << ","
                    << insert_time_A << ','
                    << find_time_A << ','
                    << remove_time_A << ','
                    << insert_time_B << ','
                    << find_time_B << ','
                    << remove_time_B
                    << "\n";
    }
    cout << endl;
}

// void compare_dynamic_arr()
// {
//     ofstream output_file("data/dynamic.csv");
//     output_file << "N"
//                 << ","
//                 << "time_A" << ',' << "time_B" << ',' << "time_C"
//                 << "\n";
//     DynamicArray arr[N_REPEATS_DA];
//     for (int N = MIN_N_DA; N < MAX_N_DA; N += STEP_DA)
//     {
//         print_progress(static_cast<double>(N) / static_cast<double>(MAX_N_DA));
//         unsigned times[3] = {0};
//         for (int q = 0; q < 3; q++)
//         {
//             for (int i = 0; i < N_REPEATS_DA; i++)
//             {
//                 arr[i] = new_array(1, 0);
//             }
//             auto begin = chrono::steady_clock::now();

//             for (int i = 0; i < N_REPEATS_DA; i++)
//             {
//                 for (int j = 0; j < N; j++)
//                 {
//                     push_back(arr[i], 0, q);
//                 }
//             }

//             auto end = chrono::steady_clock::now();
//             times[q] = (chrono::duration_cast<chrono::milliseconds>(end - begin)).count();

//             for (int i = 0; i < N_REPEATS_DA; i++)
//             {
//                 erase(arr[i]);
//             }
//         }
//         output_file << N << "," << times[0] << ',' << times[1] << ',' << times[2] << "\n";
//     }
//     output_file.close();
//     cout << endl;
// }