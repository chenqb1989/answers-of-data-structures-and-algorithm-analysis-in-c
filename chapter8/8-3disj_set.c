/*
note:
1. in this model, we can only union two root nodes (line: 142-144)
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NumSets 20

typedef int DisjSet[NumSets + 1];
typedef int SetType;
typedef int ElementType;

int print_union(DisjSet s)
{
    for (int i = 1; i <= NumSets; i++)
        printf("%d ", s[i]);
    printf("\n");
    return 0;
}

int print_array(ElementType *array, int size)
{
    for (int i = 0; i < size; i++)
        printf("%d ", array[i]);
    printf("\n");
    return 0;
}

int print_union_array(ElementType array[][2], int size)
{
    for (int i = 0; i < size; i++)
        printf("%d, %d\n", array[i][0], array[i][1]);
    printf("\n");
    return 0;
}

void initialize(DisjSet s)
{
    for (int i = NumSets; i > 0; i--)
        s[i] = 0;
}

// initialize disjset for union-by-size
void initialize_size(DisjSet s)
{
    for (int i = NumSets; i > 0; i--)
        s[i] = -1;
}

// assume root1 and root2 are roots
void set_union_simple(DisjSet s, SetType root1, SetType root2)
{
    s[root2] = root1;
}

SetType find_simple(ElementType x, DisjSet s)
{
    if (s[x] <= 0)
        return x;
    else
        return find_simple(s[x], s);
}

SetType find_simple_compression(ElementType x, DisjSet s)
{
    if (s[x] <= 0)
        return x;
    else
        return s[x] = find_simple_compression(s[x], s);
}

// assume root1 and root2 are roots
void set_union_size(DisjSet s, SetType root1, SetType root2)
{
    if(s[root2] < s[root1])
    {
        s[root2] += s[root1];
        s[root1] = root2;
    }
    else
    {
        s[root1] += s[root2];
        s[root2] = root1;
    }
}

SetType find_size(ElementType x, DisjSet s)
{
    if (s[x] < 0)
        return x;
    else
        return find_size(s[x], s);
}

SetType find_size_compression(ElementType x, DisjSet s)
{
    if (s[x] < 0)
        return x;
    else
        return s[x] = find_size_compression(s[x], s);
}

// assume root1 and root2 are roots
void set_union_height(DisjSet s, SetType root1, SetType root2)
{
    if(s[root2] < s[root1])
    {
        s[root1] = root2;
    }
    else
    {
        if (s[root1] == s[root2])
            s[root1]--;
        s[root2] = root1;
    }
}

SetType find_height(ElementType x, DisjSet s)
{
    if (s[x] <= 0)
        return x;
    else
        return find_height(s[x], s);
}

SetType find_height_compression(ElementType x, DisjSet s)
{
    if (s[x] <= 0)
        return x;
    else
        return s[x] = find_height_compression(s[x], s);
}

void test_union_find(DisjSet s,
                    ElementType union_array[][2], int union_num, ElementType find_array[], int find_num, 
                    void (*set_union)(DisjSet, SetType, SetType), SetType (*find)(ElementType, DisjSet))
{
    for (int i = 0; i < union_num; i++)
    {
        // only union two roots
        if (s[union_array[i][0]] <= 0 && s[union_array[i][1]] <= 0)
            set_union(s, union_array[i][0], union_array[i][1]);
    }
        
    for (int i = 0; i < find_num; i++)
        find(find_array[i], s);
}

void generate_union_array(ElementType union_array[][2], int union_num)
{
    for (int i = 0; i < union_num; i++)
    {
        int r1 = (rand() % NumSets) + 1;
        int r2 = (rand() % NumSets) + 1;
        while (r1 == r2)
            r2 = (rand() % NumSets) + 1;
        
        union_array[i][0] = r1;
        union_array[i][1] = r2;
    }
}

void generate_find_array(ElementType find_array[], int find_num)
{
    for (int i = 0; i < find_num; i++)
    {
        int r = (rand() % NumSets) + 1;
        find_array[i] = r;
    }
}

int main()
{
    // srand(time(NULL)); // Initialization, should only be called once.
    DisjSet s;
    initialize(s);
    DisjSet s_size;
    initialize_size(s_size);
    // ElementType union_array[5][2] = {{1, 2}, {1, 8}, {3, 6}, {1, 3}, {1, 14}};
    // ElementType find_array[3] = {1, 3, 6};
    int union_num = 20;
    int find_num = 10;
    ElementType union_array[union_num][2];
    ElementType find_array[find_num];

    generate_union_array(union_array, union_num);
    generate_find_array(find_array, find_num);
    printf("\nunion_array: \n");
    print_union_array(union_array, union_num);
    printf("\nfind_array: ");
    print_array(find_array, find_num);

    // test_union_find(s, union_array, union_num, find_array, find_num, set_union_simple, find_simple);
    // test_union_find(s, union_array, union_num, find_array, find_num, set_union_simple, find_simple_compression);
    // test_union_find(s, union_array, union_num, find_array, find_num, set_union_height, find_height);
    // test_union_find(s, union_array, union_num, find_array, find_num, set_union_height, find_height_compression);
    // test_union_find(s_size, union_array, union_num, find_array, find_num, set_union_size, find_size);
    test_union_find(s_size, union_array, union_num, find_array, find_num, set_union_size, find_size_compression);
    printf("\nresult: \n");
    print_union(s_size);
}
