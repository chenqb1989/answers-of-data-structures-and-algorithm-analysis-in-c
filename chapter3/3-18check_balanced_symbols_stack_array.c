/*
note:
1. struct stack_record: capacity, top_of_stack, data
2. in pop and push, we check empty and full stack, which consumes time
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define ElementType char*
#define EmptyToS (-1)
#define MinStackSize (5)

struct stack_record;
typedef struct stack_record *t_stack;

struct stack_record
{
    int capacity;
    int top_of_stack;
    ElementType *data;
};

int is_empty(t_stack stk)
{
    return (stk->top_of_stack == EmptyToS);
}

t_stack create_stack(int max_elements)
{
    if (max_elements < MinStackSize)
    {
        printf("stack size is too small!\n");
        return NULL;
    }

    t_stack stk = malloc(sizeof(struct stack_record));
    if (stk == NULL)
    {
        printf("out of space!\n");
        return NULL;
    }
    stk->data = malloc(sizeof(ElementType) * max_elements);
    if (stk->data == NULL)
    {
        printf("out of space!\n");
        return NULL;
    }
    stk->capacity = max_elements;
    stk->top_of_stack = -1;
    return stk;
}

int push(t_stack stk, ElementType x)
{
    if (stk->top_of_stack >= (stk->capacity - 1))
    {
        printf("stack is full!\n");
        return -1;
    }
    stk->data[++(stk->top_of_stack)] = x;
    return 0;
}

int pop(t_stack stk)
{
    if (is_empty(stk) == 1)
    {
        printf("empty stack!\n");
        return -1;
    }
    (stk->top_of_stack)--;
    return 0;
}

ElementType top(t_stack stk)
{
    if (is_empty(stk))
    {
        printf("empty stack!\n");
        return NULL;
    }
    return stk->data[stk->top_of_stack];
}

int free_stack(t_stack stk)
{
    if (stk == NULL)
        return 0;
    free(stk->data);
    free(stk);
}

int check_balance(ElementType *array, int len)
{
    if (array == NULL || len <= 0)
        return -1;
    t_stack stk = create_stack(100);
    for (int i = 0; i < len; i++)
    {
        ElementType new_cell = array[i];
        if (!is_empty(stk))
        {
            ElementType top_cell = top(stk);
            if (strcmp(top_cell, "begin") == 0)
            {
                if (strcmp(new_cell, "end") == 0)
                    pop(stk);
                else
                    push(stk, new_cell);
            }
            else if (strcmp(top_cell, "begin") == 0)
            {
                if (strcmp(new_cell, "end") == 0)
                    pop(stk);
                else
                    push(stk, new_cell);
            }
            else if (strcmp(top_cell, "(") == 0)
            {
                if (strcmp(new_cell, ")") == 0)
                    pop(stk);
                else
                    push(stk, new_cell);
            }
            else if (strcmp(top_cell, "[") == 0)
            {
                if (strcmp(new_cell, "]") == 0)
                    pop(stk);
                else
                    push(stk, new_cell);
            }
            else if (strcmp(top_cell, "{") == 0)
            {
                if (strcmp(new_cell, "}") == 0)
                    pop(stk);
                else
                    push(stk, new_cell);
            }
            else if (strcmp(top_cell, "/*") == 0)
            {
                if (strcmp(new_cell, "*/") == 0)
                    pop(stk);
                else
                    push(stk, new_cell);
            }
            else
                push(stk, new_cell);
        }
        else
            push(stk, new_cell);
    }
    if (is_empty(stk))
    {
        printf("balanced\n");
        free_stack(stk);
        return 1;
    }
    else
    {
        printf("unbalanced\n");
        free_stack(stk);
        return 0;
    }
}

int main()
{
    int array_len = 6;
    char *array[6] = {"begin", "(", ")", "end", "[", "]"};
    // char *array[6] = {"begin", "(", "(", "end", "[", "]"};
    // char *array[6] = {"/*", "(", "(", "[", "]", "*/",};
    // char *array[6] = {"/*", "(", "*/", "[", "]", ")",};
    check_balance(array, array_len);
}