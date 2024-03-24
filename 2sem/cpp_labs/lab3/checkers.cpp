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

void compare_io_flav(){
    // auto begin = chrono::steady_clock::now();

    // auto end = chrono::steady_clock::now();
    // auto time_span = chrono::duration_cast<chrono::milliseconds>(end - begin);
}

void compare_dynamic_arr(){
    ofstream output_file("data/dynamic.csv");
    output_file << "N" << "," << "time_A" << ',' << "time_B" << ',' << "time_C" << "\n";
    DynamicArray arr[N_REPEATS_DA];
    for (int N = MIN_N_DA; N < MAX_N_DA; N += STEP_DA){
        print_progress(static_cast<double>(N) / static_cast<double>(MAX_N_DA));
        unsigned times[3] = {0};
        for (int q = 0; q < 3; q++){
            for (int i = 0; i < N_REPEATS_DA; i++){
                arr[i] = new_array(1, 0);
            }
            auto begin = chrono::steady_clock::now();

            for (int i = 0; i < N_REPEATS_DA; i++){
                for (int j = 0; j < N; j++){
                    push_back(arr[i], 0, q);
                }
            }

            auto end = chrono::steady_clock::now();
            times[q] = (chrono::duration_cast<chrono::milliseconds>(end - begin)).count();

            for (int i = 0; i < N_REPEATS_DA; i++){
                erase(arr[i]);
            }
        }
        output_file << N << "," << times[0] << ',' << times[1] << ',' << times[2] << "\n";
    }
    output_file.close();
    cout << endl;
}