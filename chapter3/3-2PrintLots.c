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

int PrintLots(List L, List P)
{
    if (L->Next == NULL || P->Next == NULL)
        return 0; // empty list
    PtrToNode LP = L->Next;
    PtrToNode PP = P->Next;
    int element_p = PP->Element;
    int index_l = 0;
    while (LP != NULL)
    {
        if (element_p == index_l)
        {
            printf("%d ", LP->Element);
            if ((PP = PP->Next) != NULL)
                element_p = PP->Element;
            else
                return 0;
        }
        else if (element_p > index_l)
        {
            LP = LP->Next;
            index_l++;
        }
        else
        {
            return 0;
        }
    }
    return 0;
}

int main()
{
    // construct List L
    PtrToNode L = (PtrToNode)malloc(sizeof(struct Node));
    L->Element = 0;
    L->Next = NULL;
    PtrToNode LP = L;
    for (int i = 0; i < 10; i++)
    {
        Insert(L, (i + 1) * 2, LP);
        LP = LP->Next;
    }

    // construct List P
    PtrToNode P = (PtrToNode)malloc(sizeof(struct Node));
    P->Element = 0;
    P->Next = NULL;
    PtrToNode PP = P;
    for (int i = 0; i < 10; i = i + 3)
    {
        Insert(L, i, PP);
        PP = PP->Next;
    }
    printf("\nL:");
    Print(L);
    printf("\nP:");
    Print(P);
    printf("\nthe result of PrintLots:");
    PrintLots(L, P);
    printf("\n");
}
