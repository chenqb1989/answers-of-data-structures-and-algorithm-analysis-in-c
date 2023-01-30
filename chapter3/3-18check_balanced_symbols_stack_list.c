/*
note:
1. note the growth direction of stack (line: 60-62)
2. in this example, 'top' and 'pop' can't be integrated into 'top_pop'
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define ElementType char*

struct t_node;
typedef struct t_node *ptr_to_node;
typedef ptr_to_node t_stack;

struct t_node
{
    ElementType element;
    ptr_to_node next;
};

int is_empty(t_stack stk)
{
    return (stk->next == NULL);
}

int print_stack(t_stack stk)
{
    if (is_empty(stk))
        return 0;
    ptr_to_node p = stk->next;
    while (p != NULL)
    {
        printf("%s ", p->element);
        p = p->next;
    }
    return 0;
}

t_stack create_stack()
{
    t_stack stk = malloc(sizeof(struct t_node));
    if (stk == NULL)
    {
        printf("out of space!\n");
        return NULL;
    }
    stk->next = NULL;
    return stk;
}

int push(t_stack stk, ElementType x)
{
    ptr_to_node tmp_cell;
    tmp_cell = malloc(sizeof(struct t_node));
    if (tmp_cell == NULL)
    {
        printf("out of space!\n");
        return -1;
    }
    tmp_cell->element = x;
    tmp_cell->next = stk->next;
    stk->next = tmp_cell;
    return 0;
}

int pop(t_stack stk)
{
    ptr_to_node first_cell;
    if (is_empty(stk) == 1)
    {
        printf("empty stack!\n");
        return -1;
    }
    else
    {
        first_cell = stk->next;
        stk->next = stk->next->next;
        free(first_cell);
    }
    return 0;
}

ElementType top(t_stack stk)
{
    if (is_empty(stk) == 1)
    {
        printf("empty stack!\n");
        return NULL;
    }
    return stk->next->element;
}

int free_stack(t_stack stk)
{
    ptr_to_node first_cell;
    while (!is_empty(stk))
    {
        first_cell = stk->next;
        stk->next = stk->next->next;
        free(first_cell);
    }
    free(stk);
    return 0;
}

int check_balance(ElementType *array, int len)
{
    if (array == NULL || len <= 0)
        return -1;
    t_stack stk = create_stack();
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
    t_stack stk = create_stack();
    int array_len = 6;
    // char *array[6] = {"begin", "(", ")", "end", "[", "]"};
    // char *array[6] = {"begin", "(", "(", "end", "[", "]"};
    // char *array[6] = {"/*", "(", "(", "[", "]", "*/",};
    char *array[6] = {"/*", "(", "*/", "[", "]", ")",};
    check_balance(array, array_len);
}