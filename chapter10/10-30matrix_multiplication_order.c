#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <limits.h>

#define MaxValue LONG_MAX

void opt_matrix(const long data[], int size, long matrix[][size + 1], int last_change[][size + 1])
{
    int i, k, left, right;
    long this_m;

    for (left = 1; left <= size; left++)
        matrix[left][left] = 0;
    
    for (k = 1; k < size; k++)
        for (left = 1; left <= size - k; left++)
        {
            right = left + k;
            matrix[left][right] = MaxValue;
            for (i = left; i < right; i++)
            {
                this_m = matrix[left][i] + matrix[i + 1][right] + data[left - 1] * data[i] * data[right];
                if (this_m < matrix[left][right])
                {
                    // update min
                    matrix[left][right] = this_m;
                    last_change[left][right]= i;
                }
            }
        }
}

void print_order(int left, int right, int size, long matrix[][size + 1], int last_change[][size + 1])
{

    if (left == right)
        return;
    int i = last_change[left][right];
    print_order(left, i, size, matrix, last_change);
    print_order(i + 1, right, size, matrix, last_change);
    printf("\nleft: %d, right: %d, i: %d, value: %ld", left, right, i, matrix[left][right]);

}

int create_array(long *array, int len, int max)
{
    for (int i = 0; i < len; i++)
    {
        int r = rand() % max;
        if (r == 0)
            r = 10;
        array[i] = r;
    }
    return 0;
}

int print_array(long *array, int len)
{
    if (array != NULL && len <= 0)
    {
        return -1;
    }
    for (int i = 0; i < len; i++)
        printf("%ld ", array[i]);
    printf("\n");
    return 0;
}

int main()
{
    srand(time(NULL)); // Initialization, should only be called once.
    int data_size = 10;
    long data[data_size + 1];
    create_array(data, data_size + 1, 20);
    printf("data: \n");
    print_array(data, data_size + 1);
    long matrix[data_size + 1][data_size + 1];
    int last_change[data_size + 1][data_size + 1];
    opt_matrix(data, data_size, matrix, last_change);
    print_order(1, data_size, data_size, matrix, last_change);
    printf("\n");
    return 0;
}