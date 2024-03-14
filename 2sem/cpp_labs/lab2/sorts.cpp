void forward_step(unsigned *arr, unsigned begin_idx, unsigned end_idx)
{
    for (unsigned i = begin_idx; i < end_idx; i++)
    {
        if (arr[i] > arr[i + 1])
        {
            unsigned tmp = arr[i];
            arr[i] = arr[i + 1];
            arr[i + 1] = tmp;
        }
    }
}

void backward_step(unsigned *arr, unsigned begin_idx, unsigned end_idx)
{
    for (unsigned i = begin_idx; i > end_idx; i--)
    {
        if (arr[i] < arr[i - 1])
        {
            unsigned tmp = arr[i];
            arr[i] = arr[i - 1];
            arr[i - 1] = tmp;
        }
    }
}

void shaker_sort(unsigned *arr, unsigned begin_idx, unsigned end_idx)
{
    while (begin_idx < end_idx)
    {
        forward_step(arr, begin_idx, end_idx);
        end_idx--;
        backward_step(arr, end_idx, begin_idx);
        begin_idx++;
    }
}

void bubble_sort_forward(unsigned *arr, unsigned begin_idx, unsigned end_idx)
{
    while (begin_idx < end_idx)
    {
        forward_step(arr, begin_idx, end_idx);
        end_idx--;
    }
}

void bubble_sort_backward(unsigned *arr, unsigned begin_idx, unsigned end_idx)
{
    while (begin_idx < end_idx)
    {
        backward_step(arr, end_idx, begin_idx);
        begin_idx++;
    }
}

unsigned comb_sort(unsigned *arr, unsigned begin_idx, unsigned end_idx)
{
    unsigned step = end_idx - begin_idx;
    double factor = 1.10;
    unsigned iters = 0;

    while (step >= 1)
    {
        for (int i = begin_idx; i + step <= end_idx; i++)
        {
            if (arr[i] > arr[i + step])
            {
                unsigned tmp = arr[i];
                arr[i] = arr[i + step];
                arr[i + step] = tmp;
                iters++;
            }
        }
        step /= factor;
    }
    return iters;
}

unsigned shell_sort(unsigned *arr, unsigned begin_idx, unsigned end_idx, void (*next_step)(unsigned &, unsigned &))
{
    unsigned size = end_idx - begin_idx + 1;
    unsigned iters = 0;

    unsigned s = size;
    unsigned q = 0;
    while (s != 0)
    {
        next_step(s, q);
        for (unsigned i = s + begin_idx; i < size + begin_idx; i++)
        {
            unsigned temp = arr[i];
            unsigned j;
            for (j = i; j >= s && arr[j - s] > temp; j -= s)
                arr[j] = arr[j - s];
            iters++;

            arr[j] = temp;
        }
    }
    return iters;
}
