/*
note:
1. sedgewick inrement sequence is fastest, shell inrement sequence is slowest
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define ElementType int

// double Log2( double n )  
// {  
//     // log(n)/log(2) is log2.  
//     return log( n ) / log( 2 );  
// }

double Log3( double n )  
{  
    // log(n)/log(3) is log3.  
    return log( n ) / log( 3 );  
}

int create_array(int *array, int len, int max_value)
{
    if (array != NULL && len <= 0)
    {
        return -1;
    }
    for (int i = 0; i < len; i++)
        array[i] = rand() % max_value;
    return 0;
}

int print_array(int *array, int len)
{
    if (array != NULL && len <= 0)
    {
        return -1;
    }
    for (int i = 0; i < len; i++)
        printf("%d ", array[i]);
    printf("\n");
    return 0;
}

void shell_sort(ElementType array[], int n)
{
    int i, j, increment;
    ElementType tmp;
    for (increment = n / 2; increment > 0; increment /= 2)
    {
        for (i = increment; i < n; i++)
        {
            tmp = array[i];
            for (j = i; j >= increment; j -= increment)
            {
                if (tmp < array[j - increment])
                    array[j] = array[j - increment];
                else
                    break;
            }
            array[j] = tmp;
        }
    }
}

void shell_sort_hibbard(ElementType array[], int n)
{
    int k = (int)log2(n);
    int i, j, increment;
    ElementType tmp;

    for (; k > 0; k--)
    {
        increment = (1 << k) - 1;
        for (i = increment; i < n; i++)
        {
            tmp = array[i];
            for (j = i; j >= increment; j -= increment)
            {
                if (tmp < array[j - increment])
                    array[j] = array[j - increment];
                else
                    break;
            }
            array[j] = tmp;
        }
        // printf("\nincrement: %d\n", increment);
        // print_array(array, n);
    }
}

void shell_sort_knuth(ElementType array[], int n)
{
    int k = (int)Log3(2 * n - 1);
    int i, j, increment;
    ElementType tmp;

    for (; k >= 0; k--)
    {
        increment = (pow(3, k) + 1) / 2;
        for (i = increment; i < n; i++)
        {
            tmp = array[i];
            for (j = i; j >= increment; j -= increment)
            {
                if (tmp < array[j - increment])
                    array[j] = array[j - increment];
                else
                    break;
            }
            array[j] = tmp;
        }
        // printf("\nincrement: %d\n", increment);
        // print_array(array, n);
    }
}

void shell_sort_gonnet(ElementType array[], int n)
{
    int i, j;
    ElementType tmp;
    int increment = n / 2.2;

    while (increment >= 1)
    {
        for (i = increment; i < n; i++)
        {
            tmp = array[i];
            for (j = i; j >= increment; j -= increment)
            {
                if (tmp < array[j - increment])
                    array[j] = array[j - increment];
                else
                    break;
            }
            array[j] = tmp;
        }
        // printf("\nincrement: %d\n", increment);
        // print_array(array, n);
        if (increment == 2)
            increment = 1;
        else
            increment = increment / 2.2;
    }
}

void shell_sort_sedgewick(ElementType array[], int n)
{
    int i, j;
    ElementType tmp;

    // only support n < 16002
    if (n < 16002)
    {
        // sedgewick increment sequence
        int increments[12] = {1, 5, 19, 41, 109, 209, 505, 929, 2161, 3905, 8929, 16001};
        int end = -1;
        while (increments[++end] <= n);

        int increment = increments[--end];
        while (end >= 0)
        {
            for (i = increment; i < n; i++)
            {
                tmp = array[i];
                for (j = i; j >= increment; j -= increment)
                {
                    if (tmp < array[j - increment])
                        array[j] = array[j - increment];
                    else
                        break;
                }
                array[j] = tmp;
            }
            // printf("\nincrement: %d\n", increment);
            // print_array(array, n);
            increment = increments[--end];
        }
    }
}

void test_shell_sort(int size, int repeate_num, int max_value, int mode)
{
    ElementType array[size];
    create_array(array, size, max_value);

    ElementType array_copy[size];

    clock_t start_t, finish_t;
    double total_t = 0;
    printf("\nsize: %d, repreat_num: %d\n", size, repeate_num);
    
    // for (int i = 0; i < 5; i++)
    // {
    
    int i = mode;
    memcpy(array_copy, array, size * sizeof(ElementType));
    if (i == 0)
    {
        for(int j = 0; j < repeate_num; j++)
        {
            memcpy(array_copy, array, size * sizeof(ElementType));

            start_t = clock();
            shell_sort(array_copy, size);
            finish_t = clock();
            total_t += (double)(finish_t - start_t);
        }
        finish_t = clock();
        total_t = total_t / CLOCKS_PER_SEC;
        printf("shell sort, time: %f \n", total_t);
    }
    else if (i == 1)
    {
        for(int j = 0; j < repeate_num; j++)
        {
            memcpy(array_copy, array, size * sizeof(ElementType));

            start_t = clock();
            shell_sort_hibbard(array_copy, size);
            finish_t = clock();
            total_t += (double)(finish_t - start_t);
        }
        finish_t = clock();
        total_t = total_t / CLOCKS_PER_SEC;
        printf("shell sort hibbard, time: %f \n", total_t);
    }
    else if (i == 2)
    {
        for(int j = 0; j < repeate_num; j++)
        {
            memcpy(array_copy, array, size * sizeof(ElementType));

            start_t = clock();
            shell_sort_knuth(array_copy, size);
            finish_t = clock();
            total_t += (double)(finish_t - start_t);
        }
        finish_t = clock();
        total_t = total_t / CLOCKS_PER_SEC;
        printf("shell sort knuth, time: %f \n", total_t);
    }
    else if (i == 3)
    {
        for(int j = 0; j < repeate_num; j++)
        {
            memcpy(array_copy, array, size * sizeof(ElementType));

            start_t = clock();
            shell_sort_gonnet(array_copy, size);
            finish_t = clock();
            total_t += (double)(finish_t - start_t);
        }
        finish_t = clock();
        total_t = total_t / CLOCKS_PER_SEC;
        printf("shell sort gonnet, time: %f \n", total_t);
    }
    else if (i == 4)
    {
        for(int j = 0; j < repeate_num; j++)
        {
            memcpy(array_copy, array, size * sizeof(ElementType));

            start_t = clock();
            shell_sort_sedgewick(array_copy, size);
            finish_t = clock();
            total_t += (double)(finish_t - start_t);
        }
        finish_t = clock();
        total_t = total_t / CLOCKS_PER_SEC;
        printf("shell sort sedgewick, time: %f \n", total_t);
    }
    // }
}


int main()
{
    // srand(time(NULL)); // Initialization, should only be called once.
    int size = 15000;
    int max_value = size;
    int repeat_num = 1000;
    test_shell_sort(size, repeat_num, max_value, 0);

    printf("\n");
    return 0;
}