void shaker_sort(unsigned *arr, unsigned begin_idx, unsigned end_idx);
void bubble_sort_forward(unsigned *arr, unsigned begin_idx, unsigned end_idx);
void bubble_sort_backward(unsigned *arr, unsigned begin_idx, unsigned end_idx);
unsigned comb_sort(unsigned *arr, unsigned begin_idx, unsigned end_idx);
unsigned shell_sort(unsigned *arr, unsigned begin_idx, unsigned end_idx, void (*next_step)(unsigned&, unsigned&));