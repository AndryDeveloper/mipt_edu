#include <iostream>
#include <fstream>
#include <random>
#include "chekers.h"
#include "config.h"

using namespace std;

#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

void print_progress(double percentage)
{
  int val = static_cast<int>(percentage * 100);
  int lpad = static_cast<int>(percentage * PBWIDTH);
  int rpad = PBWIDTH - lpad;
  printf("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
  fflush(stdout);
}

template<typename T>
void print_first_elements (T *array, size_t length_to_print) {
    cout << "{";
    for (size_t i = 0; i < length_to_print; i++) {
        if (0 != i) {
            cout << ", ";
        }
        cout << array[i];
    }
    cout << "...}\n";
}


int main()
{
  default_random_engine rng(SEED);
  static unsigned arr[ARR_LENGTH] = {0};
  for (unsigned i = 0; i < ARR_LENGTH; i++){
    arr[i] = i;
  }
  cout << "Shaker sort" << endl;
  ofstream output_file_1("data/shaker.csv");
  output_file_1 << "N" << "," << "time_full" << ',' << "time_forward" << ',' << "time_backward" << "\n";;
  for (unsigned N = MIN_N; N <= MAX_N; N += STEP){
    float time_full, time_forward, time_backward = 0.;
    check_shaker_time(arr, N, rng, time_full, time_forward, time_backward);
    output_file_1 << N << "," << time_full << ',' << time_forward << ',' << time_backward << "\n";
    print_progress(static_cast<double>(N) / static_cast<double>(MAX_N));
  }
  cout << endl;

  cout << "Comb sort" << endl;
  ofstream output_file_2("data/comb.csv");
  output_file_2 << "N" << ',' << "time" << ',' << "iters" << "\n";;
  for (unsigned N = MIN_N; N <= MAX_N; N += STEP){
    float time, iterations = 0.;
    check_comb_time(arr, N, rng, time, iterations);
    output_file_2 << N << ',' << time << ',' << iterations << "\n";
    print_progress(static_cast<double>(N) / static_cast<double>(MAX_N));
  }
  cout << endl;

  cout << "Shell sort" << endl;
  ofstream output_file_3("data/shell.csv");
  output_file_3 << 'N' << ',' << "time1" << ',' << "time2" << ',' << "time3" << ',' << "iters1" << ',' << "iters2" << ',' << "iters3" << "\n";
  for (unsigned N = MIN_N; N <= MAX_N; N += STEP){
    float time1, time2, time3 = 0.;
    float iterations1, iterations2, iterations3 = 0.;

    check_shell_time(arr, N, rng, time1, time2, time3, iterations1, iterations2, iterations3);
    output_file_3 << N << ',' << time1 << ',' << time2 << ',' << time3 << ',' << iterations1 << ',' << iterations2 << ',' << iterations3 << "\n";
    print_progress(static_cast<double>(N) / static_cast<double>(MAX_N));
  }
  cout << endl;

  return 0;
}