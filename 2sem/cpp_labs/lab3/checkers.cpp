#include <fstream>
#include <iostream>
#include "config.h"
#include "structs.h"
#include "structs_methods.h"
#include "solvers.h"
#include <chrono>
#include <random>

using namespace std;

void print_progress(double percentage)
{
    int val = static_cast<int>(percentage * 100);
    int lpad = static_cast<int>(percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    printf("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
    fflush(stdout);
}

void compare_io_flav()
{
    ofstream output_file("data/io_flav.csv");
    output_file << "N"
                << ","
                << "time_A" << ',' << "time_B"
                << "\n";
    int arr[N_REPEATS_IO][MAX_N_IO] = {0};
    RingBuffer *buf_arr[N_REPEATS_IO];
    for (int N = MIN_N_IO; N < MAX_N_IO; N += STEP_IO)
    {
        print_progress(static_cast<double>(N) / static_cast<double>(MAX_N_IO));

        unsigned time_A = 0;
        auto begin = chrono::steady_clock::now();

        for (int i = 0; i < N_REPEATS_IO; i++)
        {
            arr_solver(arr[i], N, i + 1);
        }

        auto end = chrono::steady_clock::now();
        time_A = (chrono::duration_cast<chrono::milliseconds>(end - begin)).count();
        
        unsigned time_B = 0;

        for (int i = 0; i < N_REPEATS_IO; i++)
        {   
            buf_arr[i] = insert_after_b();
            RingBuffer *current = buf_arr[i];
            for (int i = 0; i < N; i++){
                current = insert_after_b(current, i);
            }
        }

        begin = chrono::steady_clock::now();

        for (int i = 0; i < N_REPEATS_IO; i++)
        {
            linked_list_solver(buf_arr[i], i + 1);
        }

        end = chrono::steady_clock::now();
        time_B = (chrono::duration_cast<chrono::milliseconds>(end - begin)).count();

        output_file << N << "," << time_A << ',' << time_B << "\n";
    }
    output_file.close();
    cout << endl;
}

void compare_dynamic_arr()
{
    ofstream output_file("data/dynamic.csv");
    output_file << "N"
                << ","
                << "time_A" << ',' << "time_B" << ',' << "time_C"
                << "\n";
    DynamicArray arr[N_REPEATS_DA];
    for (int N = MIN_N_DA; N < MAX_N_DA; N += STEP_DA)
    {
        print_progress(static_cast<double>(N) / static_cast<double>(MAX_N_DA));
        unsigned times[3] = {0};
        for (int q = 0; q < 3; q++)
        {
            for (int i = 0; i < N_REPEATS_DA; i++)
            {
                arr[i] = new_array(1, 0);
            }
            auto begin = chrono::steady_clock::now();

            for (int i = 0; i < N_REPEATS_DA; i++)
            {
                for (int j = 0; j < N; j++)
                {
                    push_back(arr[i], 0, q);
                }
            }

            auto end = chrono::steady_clock::now();
            times[q] = (chrono::duration_cast<chrono::milliseconds>(end - begin)).count();

            for (int i = 0; i < N_REPEATS_DA; i++)
            {
                erase(arr[i]);
            }
        }
        output_file << N << "," << times[0] << ',' << times[1] << ',' << times[2] << "\n";
    }
    output_file.close();
    cout << endl;
}