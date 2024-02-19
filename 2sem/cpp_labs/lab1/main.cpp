#include <iostream>
#include <chrono>
#include <random>
#include <fstream>
#include <algorithm>
using namespace std;

#define MAX_N 10000
#define MIN_N 100
#define N_RANGE 1000
#define N_REPEATED_LINEAR 500'000
#define N_REPEATED_LOGARIFMIC 500'000
#define N_REPEATED_SQUARE 1
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

void linear_search(unsigned *arr, unsigned N, unsigned search_element)
{
  for (unsigned i = 0; i < N; i++)
  {
    if (arr[i] == search_element)
    {
      break;
    }
  }
}

void binary_search(unsigned *sorted_arr, unsigned N, unsigned search_element)
{
  unsigned low = 0;
  unsigned high = N - 1;

  while (low <= high)
  {
    unsigned mid = low + (high - low) / 2;
    if (search_element == sorted_arr[mid])
      break;
    else if (search_element > sorted_arr[mid])
      low = mid + 1;
    else
      high = mid - 1;
  }
}

void sum_search_long(unsigned *arr, unsigned N, unsigned sum)
{
  bool found = false;
  for (unsigned i = 0; i < N && !found; i++)
  {
    for (unsigned j = i + 1; j < N && !found; j++)
    {
      if (arr[i] + arr[j] == sum)
      {
        found = true;
      }
    }
  }
}

void sum_search_fast(unsigned *arr, unsigned N, unsigned sum)
{
  unsigned i = 0;
  unsigned j = N - 1;
  while (i < j)
  {
    if (arr[i] + arr[j] == sum)
      break;
    else if (arr[i] + arr[j] < sum)
      i++;
    else
      j--;
  }
}

void strategy_A(unsigned *arr, unsigned N, unsigned search_element)
{
  for (unsigned i = 0; i < N; i++)
  {
    if (arr[i] == search_element && i != 0)
    {
      unsigned tmp = arr[i];
      arr[i] = arr[0];
      arr[0] = tmp;
      break;
    }
  }
}

void strategy_B(unsigned *arr, unsigned N, unsigned search_element)
{
  for (unsigned i = 0; i < N; i++)
  {
    if (arr[i] == search_element && i != 0)
    {
      unsigned tmp = arr[i];
      arr[i] = arr[i - 1];
      arr[i - 1] = tmp;
      break;
    }
  }
}

void strategy_C(unsigned *arr, unsigned *arr_counts, unsigned N, unsigned search_element)
{
  for (unsigned i = 0; i < N; i++)
  {
    if (arr[i] == search_element)
    {
      arr_counts[i]++;
      if (i != 0)
      {
        if (arr_counts[i - 1] < arr_counts[i])
        {
          unsigned tmp = arr[i];
          arr[i] = arr[i - 1];
          arr[i - 1] = tmp;
        }
      }
      break;
    }
  }
}

unsigned check_time(unsigned int *arr, unsigned N, default_random_engine rng, unsigned exp_id)
{
  auto begin = chrono::steady_clock::now();
  switch (exp_id)
  {
  case 1:
  { 
    uniform_int_distribution<unsigned> dstr(0, N - 1);
    for (unsigned cnt = N_REPEATED_LINEAR; cnt != 0; --cnt)
    {
      linear_search(arr, N, arr[dstr(rng)]);
    }
    break;
  }
  case 2:
  { 
    uniform_int_distribution<unsigned> dstr(0, N - 1);
    for (unsigned cnt = N_REPEATED_LOGARIFMIC; cnt != 0; --cnt)
    {
      binary_search(arr, N, arr[dstr(rng)]);
    }
    break;
  }
  case 3:
  { 
    uniform_int_distribution<unsigned> dstr(0, N - 1);
    for (unsigned cnt = N_REPEATED_SQUARE; cnt != 0; --cnt)
    {
      sum_search_long(arr, N, arr[dstr(rng)] + arr[dstr(rng)]);
    }
    break;
  }
  case 4:
  { 
    uniform_int_distribution<unsigned> dstr(0, N - 1);
    for (unsigned cnt = N_REPEATED_LINEAR; cnt != 0; --cnt)
    {
      sum_search_fast(arr, N, arr[dstr(rng)] + arr[dstr(rng)]);
    }
  }
  case 5:
  {
    std::poisson_distribution<unsigned> dstr((N - 1) / 2);
    random_shuffle(&arr[0], &arr[N]);
    for (unsigned cnt = N_REPEATED_LINEAR; cnt != 0; --cnt)
    {
      strategy_A(arr, N, dstr(rng));
    }
    sort(arr, arr + N);
    break;
  }
  case 6:
  {
    std::poisson_distribution<unsigned> dstr((N - 1) / 2);
    random_shuffle(&arr[0], &arr[N]);
    for (unsigned cnt = N_REPEATED_LINEAR; cnt != 0; --cnt)
    {
      strategy_B(arr, N, dstr(rng));
    }
    sort(arr, arr + N);
    break;
  }
  case 7:
  {
    uniform_int_distribution<unsigned> dstr(0, N);
    random_shuffle(&arr[0], &arr[N]);
    for (unsigned cnt = N_REPEATED_LINEAR; cnt != 0; --cnt)
    {
      strategy_A(arr, N, dstr(rng));
    }
    sort(arr, arr + N);
    break;
  }
  case 8:
  {
    uniform_int_distribution<unsigned> dstr(0, N);
    random_shuffle(&arr[0], &arr[N]);
    for (unsigned cnt = N_REPEATED_LINEAR; cnt != 0; --cnt)
    {
      strategy_B(arr, N, dstr(rng));
    }
    sort(arr, arr + N);
    break;
  }
  default:
    break;
  }
  auto end = chrono::steady_clock::now();
  auto time_span = chrono::duration_cast<chrono::milliseconds>(end - begin);
  return time_span.count();
}

unsigned check_time(unsigned int *arr, unsigned N, default_random_engine rng, unsigned exp_id, unsigned int *arr_counts){
  auto begin = chrono::steady_clock::now();
  switch (exp_id)
  {
  case 9: 
  {
    std::poisson_distribution<unsigned> dstr((N - 1) / 2);
    random_shuffle(&arr[0], &arr[N]);
    for (unsigned cnt = N_REPEATED_LINEAR; cnt != 0; --cnt)
    {
      strategy_C(arr, arr_counts, N, dstr(rng));
    }
    sort(arr, arr + N);
    fill(arr_counts, arr_counts + N, 0);
    break;
  }
  case 10: 
  {
    uniform_int_distribution<unsigned> dstr(0, N);
    random_shuffle(&arr[0], &arr[N]);
    for (unsigned cnt = N_REPEATED_LINEAR; cnt != 0; --cnt)
    {
      strategy_C(arr, arr_counts, N, dstr(rng));
    }
    sort(arr, arr + N);
    fill(arr_counts, arr_counts + N, 0);
    break;
  }
  
  default:
    break;
}
  auto end = chrono::steady_clock::now();
  auto time_span = chrono::duration_cast<chrono::milliseconds>(end - begin);
  return time_span.count();
}


int main()
{
  const unsigned N_range[] = {MIN_N, MAX_N, N_RANGE};
  static unsigned arr[MAX_N] = {0};
  static unsigned sort_arr[MAX_N] = {0};
  static unsigned arr_counts[MAX_N] = {0};

  const unsigned seed = 42;
  default_random_engine rng(seed);
  uniform_int_distribution<unsigned> dstr(0, N_range[1] - 1);

  for (unsigned i = 0; i < N_range[1]; i++)
  {
    unsigned p = dstr(rng);
    arr[i] = p;
    sort_arr[i] = p;
  }
  sort(sort_arr, sort_arr + N_range[1]);

  // cout << "First test started" << endl;
  // ofstream output_file_1("data/linear_search.csv");
  // output_file_1 << "N" << "," << "time" << "\n";
  // for (unsigned N = N_range[0], i = 0; N <= N_range[1]; N += N_range[2], i++){
  //   output_file_1 << N << "," << check_time(arr, N, rng, 1) << "\n";
  //   print_progress(static_cast<double>(N) / static_cast<double>(N_range[1]));
  // }
  // cout << endl;

  // cout << "Second test started" << endl;
  // ofstream output_file_2("data/binary_search.csv");
  // output_file_2 << "N" << "," << "time" << "\n";
  // for (unsigned N = N_range[0], i = 0; N <= N_range[1]; N += N_range[2], i++){
  //   output_file_2 << N << "," << check_time(sort_arr, N, rng, 2) << "\n";
  //   print_progress(static_cast<double>(N) / static_cast<double>(N_range[1]));
  // }
  // cout << endl;

  // cout << "Third test start(sum search O(n^2))" << endl;
  // ofstream output_file_3("data/sum_search_1.csv");
  // output_file_3 << "N" << "," << "time" << "\n";
  // for (unsigned N = N_range[0], i = 0; N <= N_range[1]; N += N_range[2], i++){
  //   print_progress(static_cast<double>(N) / static_cast<double>(N_range[1]));
  //   output_file_3 << N << "," << check_time(arr, N, rng, 3) << "\n";
  // }
  // cout << endl;

  // cout << "Fourth test start(sum search O(n))" << endl;
  // ofstream output_file_4("data/sum_search_2.csv");
  // output_file_4 << "N" << "," << "time" << "\n";
  // for (unsigned N = N_range[0], i = 0; N <= N_range[1]; N += N_range[2], i++){
  //   print_progress(static_cast<double>(N) / static_cast<double>(N_range[1]));
  //   output_file_4 << N << "," << check_time(sort_arr, N, rng, 4) << "\n";
  // }
  // cout << endl;

  cout << "Fifth test start (Strategy A poisson dist)" << endl;
  ofstream output_file_5("data/strategy_a_1.csv");
  output_file_5 << "N"
                << ","
                << "time"
                << "\n";
  for (unsigned N = N_range[0], i = 0; N <= N_range[1]; N += N_range[2], i++)
  {
    print_progress(static_cast<double>(N) / static_cast<double>(N_range[1]));
    output_file_5 << N << "," << check_time(sort_arr, N, rng, 5) << "\n";
  }
  sort(sort_arr, sort_arr + N_range[1]);
  cout << endl;

  cout << "Sixth test start (Strategy B poisson dist)" << endl;
  ofstream output_file_6("data/strategy_b_1.csv");
  output_file_6 << "N"
                << ","
                << "time"
                << "\n";
  for (unsigned N = N_range[0], i = 0; N <= N_range[1]; N += N_range[2], i++)
  {
    print_progress(static_cast<double>(N) / static_cast<double>(N_range[1]));
    output_file_6 << N << "," << check_time(sort_arr, N, rng, 6) << "\n";
  }
  sort(sort_arr, sort_arr + N_range[1]);
  cout << endl;

  cout << "Seventh test start (Strategy A rect dist)" << endl;
  ofstream output_file_7("data/strategy_a_2.csv");
  output_file_7 << "N"
                << ","
                << "time"
                << "\n";
  for (unsigned N = N_range[0], i = 0; N <= N_range[1]; N += N_range[2], i++)
  {
    print_progress(static_cast<double>(N) / static_cast<double>(N_range[1]));
    output_file_7 << N << "," << check_time(sort_arr, N, rng, 7) << "\n";
  }
  sort(sort_arr, sort_arr + N_range[1]);
  cout << endl;

  cout << "Eighth test start (Strategy B rect dist)" << endl;
  ofstream output_file_8("data/strategy_b_2.csv");
  output_file_8 << "N"
                << ","
                << "time"
                << "\n";
  for (unsigned N = N_range[0], i = 0; N <= N_range[1]; N += N_range[2], i++)
  {
    print_progress(static_cast<double>(N) / static_cast<double>(N_range[1]));
    output_file_8 << N << "," << check_time(sort_arr, N, rng, 8) << "\n";
  }
  sort(sort_arr, sort_arr + N_range[1]);
  cout << endl;

  cout << "Ninth test start (Strategy C poisson dist)" << endl;
  ofstream output_file_9("data/strategy_c_1.csv");
  output_file_9 << "N"
                << ","
                << "time"
                << "\n";
  for (unsigned N = N_range[0], i = 0; N <= N_range[1]; N += N_range[2], i++)
  {
    print_progress(static_cast<double>(N) / static_cast<double>(N_range[1]));
    output_file_9 << N << "," << check_time(sort_arr, N, rng, 9, arr_counts) << "\n";
  }
  sort(sort_arr, sort_arr + N_range[1]);
  cout << endl;

  cout << "Tenth test start (Strategy C rect dist)" << endl;
  ofstream output_file_10("data/strategy_c_2.csv");
  output_file_10 << "N"
                << ","
                << "time"
                << "\n";
  for (unsigned N = N_range[0], i = 0; N <= N_range[1]; N += N_range[2], i++)
  {
    print_progress(static_cast<double>(N) / static_cast<double>(N_range[1]));
    output_file_10 << N << "," << check_time(sort_arr, N, rng, 10, arr_counts) << "\n";
  }
  
  cout << endl;
  return 0;
}
