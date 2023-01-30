// print all elements of one linked list
#include <stdlib.h>
#include <stdio.h>
#define ElementType int

struct Node;
typedef struct Node *PtrToNode;
typedef PtrToNode List;
typedef PtrToNode Position;

struct Node
{
    ElementType Element;
    Position Next;
};

int Insert(List L, ElementType X, Position P)
{
    PtrToNode TmpCell = (PtrToNode)malloc(sizeof(struct Node));
    if (TmpCell == NULL)
        return -1;
    TmpCell->Element = X;
    TmpCell->Next = P->Next;
    P->Next = TmpCell;
    return 0;
}

int Print(List L)
{
    if (L->Next == NULL)
        return 0; // empty list
    PtrToNode P = L->Next;
    while (P != NULL)
    {
        printf("%d ", P->Element);
        P = P->Next;
    }
    return 0;
}

int main()
{
    PtrToNode L = (PtrToNode)malloc(sizeof(struct Node));
    L->Element = 0;
    L->Next = NULL;
    printf("print empty list:");
    Print(L);
    PtrToNode P = L;
    for (int i = 0; i < 10; i++)
    {
        Insert(L, i + 1, P);
        P = P->Next;
    }
    printf("\nprint inserted list:");
    Print(L);
    printf("\n");
}
