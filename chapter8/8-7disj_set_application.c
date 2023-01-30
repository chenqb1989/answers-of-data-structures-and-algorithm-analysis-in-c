/*
note:
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

void test_union_find(DisjSet s, ElementType union_array[][2], int union_num)
{
    for (int i = 0; i < union_num; i++)
    {
        // only union two roots
        int root1 = find_height_compression(union_array[i][0], s);
        int root2 = find_height_compression(union_array[i][1], s);
        if (root1 != root2)
            set_union_height(s, root1, root2);
    }
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

int main()
{
    // srand(time(NULL)); // Initialization, should only be called once.
    DisjSet s;
    initialize(s);
   
    int union_num = 20;
    ElementType union_array[union_num][2];

    generate_union_array(union_array, union_num);
    printf("\nunion_array: \n");
    print_union_array(union_array, union_num);

    test_union_find(s, union_array, union_num);
    printf("\nresult: \n");
    print_union(s);
}
