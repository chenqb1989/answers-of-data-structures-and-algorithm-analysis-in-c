/*
note: 
1. find should consider counts in line 178, so we add count in data_point_struct(line 14)
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

struct data_point_struct;
typedef struct data_point_struct *data_point;
struct data_point_struct
{
    int value;
    char count;
};

struct dist_set_struct;
typedef struct dist_set_struct *dist_set;

struct dist_set_struct
{
    data_point elements;
    int num;
    int size;
};

int cmpfunc_x (const void *a, const void *b)
{
   return (((data_point)a)->value - ((data_point)b)->value);
}

dist_set create_dist_set(int array[][2], int num)
{
    dist_set dists = malloc(sizeof(struct dist_set_struct));
    dists->num = 0;
    if (dists == NULL)
    {
        printf("out of space!");
        return NULL;
    }

    dists->elements = malloc(sizeof(struct data_point_struct) * num);
    if (dists->elements == NULL)
    {
        printf("out of space!");
        return NULL;
    }
    
    dists->num = num;
    dists->size = 0;
    for (int i = 0; i < num; i++)
    {
        dists->elements[i].value = array[i][0];
        dists->elements[i].count = array[i][1];
        dists->size += array[i][1];
    }
    
    qsort(dists->elements, num, sizeof(struct data_point_struct), cmpfunc_x);
    return dists;
}

int find_max(dist_set dists)
{
    if (dists->size <= 0)
    {
        printf("distset is empty");
        return -1;
    }
    for (int i = dists->num - 1; i >= 0; i--)
        if (dists->elements[i].count > 0)
            return dists->elements[i].value;
    printf("error");
    return -1;
}

int delete_max(dist_set dists)
{
    if (dists->size <= 0)
    {
        printf("distset is empty");
        return -1;
    }
    for (int i = dists->num - 1; i >= 0; i--)
        if (dists->elements[i].count > 0)
        {
            dists->elements[i].count--;
            dists->size--;
            return dists->elements[i].value;
        }
    
    printf("error");
    return -1;
}

void delete(int x, dist_set dists)
{
    for (int i = 0; i < dists->num; i++)
        if (dists->elements[i].count > 0 && dists->elements[i].value == x)
        {
            dists->elements[i].count--;
            dists->size--;
            break;
        }
}

void insert(int x, dist_set dists)
{
    for (int i = 0; i < dists->num; i++)
        if (dists->elements[i].value == x)
        {
            dists->elements[i].count++;
            dists->size++;
            break;
        }
}

int find(int x, dist_set dists, int count)
{
    for (int i = 0; i < dists->num; i++)
        if (dists->elements[i].count >= count && dists->elements[i].value == x)
        {
            return i;
        }
    return -1;
}

void initialize_data_points(data_point dp, int size)
{
    for (int i = 0; i < size; i++)
    {
        dp[i].count = 0;
        dp[i].value = -1;
    }
}

void insert_data_points(data_point dp, int size, int x)
{
    for (int i = 0; i < size; i++)
    {
        if (dp[i].count > 0)
        {
            if (dp[i].value == x)  // x is already in dp
            {
                dp[i].count++;
                break;
            }

        }
        else  // new element
        {
            dp[i].value = x;
            dp[i].count++;
            break;
        }
    }
}

int place(int coordinates[], dist_set dists, int size, int left, int right)
{
    int dmax, found = 0;
    if (dists->size <= 0)
        return 1;
    
    dmax = find_max(dists);

    /* check if setting coordinates[right] = dmax is feasible */
    char feasible = 1;
    int j;
    int temps_num = (left - 1) + (size - right);
    struct data_point_struct temps[temps_num];
    initialize_data_points(temps, temps_num);
    for (j = 1; j < left; j++)
        insert_data_points(temps, temps_num, abs(coordinates[j] - dmax));
    for (j = right + 1; j <= size; j++)
        insert_data_points(temps, temps_num, abs(coordinates[j] - dmax));
    
    for (int k = 0; k < temps_num && temps[k].count > 0; k++)
        if (find(temps[k].value, dists, temps[k].count) < 0)
        {
            feasible = 0;
            break;
        }
   
    if (feasible == 1)
    {
        coordinates[right] = dmax;
        for (j = 1; j < left; j++)
            delete(abs(coordinates[j] - dmax), dists);
        for (j = right + 1; j <= size; j++)
            delete(abs(coordinates[j] - dmax), dists);
        
        found = place(coordinates, dists, size, left, right - 1);
        if (found == 0) // backtrack
        {
            for (j = 1; j < left; j++)
                insert(abs(coordinates[j] - dmax), dists);
            for (j = right + 1; j <= size; j++)
                insert(abs(coordinates[j] - dmax), dists);
        }
    }

    /* if first attempt failed, try to see if setting coordinates[left] = coordinates[size] - dmax */
    if (found == 0)
    {
        feasible = 1;

        initialize_data_points(temps, temps_num);
        for (j = 1; j < left; j++)
            insert_data_points(temps, temps_num, abs(coordinates[size] - dmax - coordinates[j]));
        for (j = right + 1; j <= size; j++)
            insert_data_points(temps, temps_num, abs(coordinates[size] - dmax - coordinates[j]));
        
         for (int k = 0; k < temps_num && temps[k].count > 0; k++)
            if (find(temps[k].value, dists, temps[k].count) < 0)
            {
                feasible = 0;
                break;
            }
        
        if (feasible == 1)
        {
            coordinates[left] = coordinates[size] - dmax;
            for (j = 1; j < left; j++)
                delete(abs(coordinates[size] - dmax - coordinates[j]), dists);
            for (j = right + 1; j <= size; j++)
                delete(abs(coordinates[size] - dmax - coordinates[j]), dists);
            
            found = place(coordinates, dists, size, left + 1, right);
            if (found == 0) // backtrack
            {
                for (j = 1; j < left; j++)
                    insert(abs(coordinates[size] - dmax - coordinates[j]), dists);
                for (j = right + 1; j <= size; j++)
                    insert(abs(coordinates[size] - dmax - coordinates[j]), dists);
            }
        }
    }
    
    return found;
}

int trunpike(int x[], dist_set dists, int size)
{
    x[1] = 0;
    x[size] = delete_max(dists);
    x[size - 1] = delete_max(dists);
    if (find(x[size] - x[size - 1], dists, 1) >= 0)
    {
        delete(x[size] - x[size - 1], dists);
        return place(x, dists, size, 2, size - 2);
    }
    else
        return -1;
}

int main()
{
    srand(time(NULL));   // Initialization, should only be called once.
    int array_size = 9;
    int size = 6;
    int array[][2] = {{1, 1}, {2, 3}, {3, 3}, {4, 1}, {5, 3}, {6, 1}, {7, 1}, {8, 1}, {10, 1}};
    dist_set ds = create_dist_set(array, array_size);

    int coordinates[size + 1];
    if (trunpike(coordinates, ds, size) < 0)
        printf("no solution\n");

    printf("coordinates: ");
    for (int i = 1; i <= size; i++)
        printf("%d ", coordinates[i]);
    printf("\n");
}