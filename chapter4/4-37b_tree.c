/*
note:
1. insert and delete firtly, then split or merge (self-adjusting)
1. delete, note to update tree
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define ElementType int
#define M 3
#define MIN_NUM (M + 1) / 2

struct tree_node;
typedef struct tree_node *position;
typedef struct tree_node *b_tree;

struct tree_node
{
    ElementType *data;
    b_tree *child;
    int data_len;
    int child_len;
};

int free_node(position node)
{
    if (node == NULL)
        return 0;
    free(node->data);
    free(node->child);
    free(node);
}

int free_tree(b_tree tree)
{
    if (tree == NULL)
        return 0;
    if (tree->child_len == 0)
    {
        free_node(tree);
    }
    else
    {
        for (int i = 0; i < tree->child_len; i++)
            free_tree(tree->child[i]);
        free_node(tree);
    }
    return 0;
}

position find_min(b_tree tree)
{
    if (tree == NULL)
        return NULL;
    if (tree->child_len > 0)
        find_min(tree->child[0]);
    else
        return tree;
}

b_tree split_leaf_node(position node)
{
    position node2 = malloc(sizeof(struct tree_node));
    if (node2 == NULL)
        return NULL;

    node2->data = malloc(sizeof(ElementType) * (M + 1));
    if (node2->data == NULL)
        return NULL;

    node2->child = malloc(sizeof(b_tree) * (M + 1));
    if (node2->child == NULL)
        return NULL;

    node2->data_len = node->data_len / 2;
    node->data_len -= node2->data_len;
    for (int i = 0; i < node2->data_len; i++)
        node2->data[i] = node->data[node->data_len + i];

    node2->child_len = 0;
    return node2;
}

b_tree split_internal_node(position node)
{
    position node2 = malloc(sizeof(struct tree_node));
    if (node2 == NULL)
        return NULL;

    node2->data = malloc(sizeof(ElementType) * (M + 1));
    if (node2->data == NULL)
        return NULL;

    node2->child = malloc(sizeof(b_tree) * (M + 1));
    if (node2->child == NULL)
        return NULL;

    node2->child_len = node->child_len / 2;
    node->child_len -= node2->child_len;
    for (int i = 0; i < node2->child_len; i++)
        node2->child[i] = node->child[node->child_len + i];
    
    node->data_len = node->child_len - 1;
    node2->data_len = node2->child_len - 1;
    for (int i = 0; i < node->data_len; i++)
        node->data[i] = node->child[i + 1]->data[0];
    for (int i = 0; i < node2->data_len; i++)
        node2->data[i] = node2->child[i + 1]->data[0];

    return node2;
}

b_tree split_root_node(position node)
{
    position node1 = malloc(sizeof(struct tree_node));
    if (node1 == NULL)
        return NULL;

    node1->data = malloc(sizeof(ElementType) * (M + 1));
    if (node1->data == NULL)
        return NULL;

    node1->child = malloc(sizeof(b_tree) * (M + 1));
    if (node1->child == NULL)
        return NULL;

    position node2 = malloc(sizeof(struct tree_node));
    if (node2 == NULL)
        return NULL;

    node2->data = malloc(sizeof(ElementType) * (M + 1));
    if (node2->data == NULL)
        return NULL;

    node2->child = malloc(sizeof(b_tree) * (M + 1));
    if (node2->child == NULL)
        return NULL;

    node2->child_len = node->child_len / 2;
    node1->child_len = node->child_len - node2->child_len;
    for (int i = 0; i < node1->child_len; i++)
        node1->child[i] = node->child[i];
    for (int i = 0; i < node2->child_len; i++)
        node2->child[i] = node->child[node1->child_len + i];

    node1->data_len = node1->child_len - 1;
    node2->data_len = node2->child_len - 1;
    for (int i = 0; i < node1->data_len; i++)
    {
        position tmp_cell = find_min(node1->child[i + 1]);
        if (tmp_cell)
            node1->data[i] = tmp_cell->data[0];
    }
    for (int i = 0; i < node2->data_len; i++)
    {
        position tmp_cell = find_min(node2->child[i + 1]);
        if (tmp_cell)
            node2->data[i] = tmp_cell->data[0];
    }

    node->data_len = 1;
    node->child_len = 2;
    node->child[0] = node1;
    node->child[1] = node2;
    position tmp_cell = find_min(node->child[1]);
    if (tmp_cell)
        node->data[0] = tmp_cell->data[0];
    return node;
}

int insert_tree_in(ElementType x, b_tree tree)
{
    position node2 = NULL;
    if (tree->child[0] == NULL)
    {
        // leaf node
        int index = -1;
        for (int i = 0; i < M; i++)
        {
            if (x < tree->data[i])
            {
                index = i;
                break;
            }
        }
        if (index == -1)
            index = tree->data_len;
        tree->data_len++;

        for (int i = tree->data_len - 1; i > index; i--)
            tree->data[i] = tree->data[i - 1];

        tree->data[index] = x;
    }
    else
    {
        // internal node
        int child_index = -1;
        for (int i = 0; i < tree->data_len; i++)
            if (x < tree->data[i])
            {
                child_index = i;
                break;
            }
        if (child_index == -1)
            child_index = tree->child_len - 1;

        insert_tree_in(x, tree->child[child_index]);
        if (tree->child[child_index]->data_len > M || tree->child[child_index]->child_len > M)
        {
            // new child by spliting one child node
            if (tree->child[child_index]->child_len > M)
                node2 = split_internal_node(tree->child[child_index]);
            else
                node2 = split_leaf_node(tree->child[child_index]);
            tree->child_len++;
            tree->data_len++;
            for (int i = tree->child_len - 1; i > child_index + 1; i--)
                tree->child[i] = tree->child[i - 1];
            tree->child[child_index + 1] = node2;
            for (int i = 0; i < tree->data_len; i++)
            {
                position tmp_cell = find_min(tree->child[i + 1]);
                if (tmp_cell)
                    tree->data[i] = tmp_cell->data[0];
            }
        }
    }
}

b_tree insert_tree(ElementType x, b_tree tree)
{
    if (tree == NULL)
    {
        position tmp_cell = malloc(sizeof(struct tree_node));
        if (tmp_cell == NULL)
            return NULL;

        tmp_cell->data = malloc(sizeof(ElementType) * (M + 1));
        if (tmp_cell->data == NULL)
            return NULL;

        tmp_cell->child = malloc(sizeof(b_tree) * (M + 1));
        if (tmp_cell->child == NULL)
            return NULL;

        position root = malloc(sizeof(struct tree_node));
        if (root == NULL)
            return NULL;

        root->data = malloc(sizeof(ElementType) * (M + 1));
        if (root->data == NULL)
            return NULL;

        root->child = malloc(sizeof(b_tree) * (M + 1));
        if (root->child == NULL)
            return NULL;

        for (int i = 0; i < M; i++)
        {
            tmp_cell->data[i] = 0;
            root->data[i] = 0;
        }

        for (int i = 0; i < M; i++)
        {
            tmp_cell->child[i] = NULL;
            root->child[i] = NULL;
        }

        tmp_cell->data[0] = x;
        tmp_cell->data_len = 1;
        tmp_cell->child_len = 0;

        root->child[0] = tmp_cell;
        root->data_len = 0;
        root->child_len = 1;
        return root;
    }

    insert_tree_in(x, tree);
    if (tree->child_len > M)
    {
        tree = split_root_node(tree);
    }
    return tree;
}

int merge_leaf_node(position tree, int del_index)
{
    if (tree->child_len < 2)
        return 0;
    position sibling_left = NULL;
    position sibling_right = NULL;
    position merge_to = NULL;
    int merge_to_left = 1;  // flag shows merge to left or right, 1 means left, 0 means right
    if (del_index - 1 >= 0)
        sibling_left = tree->child[del_index - 1];
    if (del_index + 1 < tree->child_len)
        sibling_right = tree->child[del_index + 1];
    
    if (sibling_left == NULL)
    {
        merge_to = sibling_right;
        merge_to_left = 0;
    }   
    else if (sibling_right == NULL)
        merge_to = sibling_left;
    else
        if (sibling_right->data_len < sibling_left->data_len)
        {
            merge_to = sibling_right;
            merge_to_left = 0;
        }
        else
            merge_to = sibling_left;
    
    if (merge_to->data_len < M)
    {
        merge_to->data_len++;
        if (!merge_to_left)
        {
            for (int i = merge_to->data_len - 1; i > 0; i--)
                merge_to->data[i] = merge_to->data[i - 1];
            merge_to->data[0] = tree->child[del_index]->data[0];
        }
        else
            merge_to->data[merge_to->data_len - 1] = tree->child[del_index]->data[0];
        
        // update tree
        tree->child_len--;
        free_node(tree->child[del_index]);
        for (int i = del_index; i < tree->child_len; i++)
            tree->child[i] = tree->child[i + 1];
        tree->data_len--;
        for (int i = 0; i < tree->data_len; i++)
            tree->data[i] = find_min(tree->child[i + 1])->data[0];  // todo, may be no need to use find_min 
    }
    else
    {
        // merge_to is full, move one element from merge_to to delete node, but don't delete the node
        tree->child[del_index]->data_len++;
        merge_to->data_len--;
        if (!merge_to_left)
        {
            tree->child[del_index]->data[1] = merge_to->data[0];
            for (int i = 0; i < merge_to->data_len; i++)
                merge_to[i] = merge_to[i + 1];
            
        }
        else
        {
            tree->child[del_index]->data[1] = tree->child[del_index]->data[0];
            tree->child[del_index]->data[0] = merge_to->data[merge_to->data_len];
        }
        
        // update tree
        for (int i = 0; i < tree->data_len; i++)
            tree->data[i] = find_min(tree->child[i + 1])->data[0];
    }
    return 0;
}

int merge_internal_node(b_tree tree, int del_index)
{
    if (tree->child_len < 2)
        return 0;
    position sibling_left = NULL;
    position sibling_right = NULL;
    position merge_to = NULL;
    int merge_to_left = 1;  // flag shows merge to left or right, 1 means left, 0 means right
    if (del_index - 1 >= 0)
        sibling_left = tree->child[del_index - 1];
    if (del_index + 1 < tree->child_len)
        sibling_right = tree->child[del_index + 1];
    
    if (sibling_left == NULL)
    {
        merge_to = sibling_right;
        merge_to_left = 0;
    }   
    else if (sibling_right == NULL)
        merge_to = sibling_left;
    else
        if (sibling_right->child_len < sibling_left->child_len)
        {
            merge_to = sibling_right;
            merge_to_left = 0;
        }
        else
            merge_to = sibling_left;
    
    if (merge_to->child_len < M)
    {
        merge_to->child_len++;
        merge_to->data_len++;

        if (!merge_to_left)
        {
            for (int i = merge_to->child_len - 1; i > 0; i--)
                merge_to->child[i] = merge_to->child[i - 1];
            merge_to->child[0] = tree->child[del_index]->child[0];

            // update data
            for (int i = merge_to->data_len - 1; i > 0; i--)
                merge_to->data[i] = merge_to->data[i - 1];
            merge_to->data[0] = find_min(merge_to->child[1])->data[0];
        }
        else
        {
            merge_to->child[merge_to->child_len - 1] = tree->child[del_index]->child[0];
            merge_to->data[merge_to->data_len - 1] = find_min(tree->child[del_index]->child[0])->data[0];
        }

        // update tree
        tree->child_len--;
        free_node(tree->child[del_index]);
        for (int i = del_index; i < tree->child_len; i++)
            tree->child[i] = tree->child[i + 1];
        tree->data_len--;
        for (int i = 0; i < tree->data_len; i++)
            tree->data[i] = find_min(tree->child[i + 1])->data[0];  // todo, may be no need to use find_min 
    }
    else
    {
        // merge_to is full, move one element from merge_to to delete node, but don't delete the node
        tree->child[del_index]->child_len++;
        merge_to->child_len--;
        tree->child[del_index]->data_len++;
        merge_to->data_len--;

        if (!merge_to_left)
        {
            tree->child[del_index]->child[1] = merge_to->child[0];
            for (int i = 0; i < merge_to->child_len; i++)
                merge_to[i] = merge_to[i + 1];
            tree->child[del_index]->data[0] = find_min(tree->child[del_index]->child[1])->data[0];
        }
        else
        {
            tree->child[del_index]->child[1] = tree->child[del_index]->child[0];
            tree->child[del_index]->child[0] = merge_to->child[merge_to->child_len];
            tree->child[del_index]->data[0] = find_min(tree->child[del_index]->child[1])->data[0];
        }
        
    }
}

int delete_tree_in(ElementType x, b_tree tree)
{
    if (tree->child_len == 0)
    {
        // leaf node
        int index = -1;
        for (int i = 0; i < tree->data_len; i++)
            if (tree->data[i] == x)
            {
                index = i;
                break;
            }
        if (index == -1)
        {
            printf("no deleted element %d found!\n", x);
            return -1;
        }

        for (int i = index; i < tree->data_len - 1; i++)
            tree->data[i] = tree->data[i + 1];
        
        tree->data_len--;
        return index;
    }
    else
    {
        // internal node
        int child_index = -1;
        for (int i = 0; i < tree->data_len; i++)
            if (x < tree->data[i])
            {
                child_index = i;
                break;
            }
        if (child_index == -1)
            child_index = tree->child_len - 1;
        
        if (delete_tree_in(x, tree->child[child_index]) < 0)
            return -1;
        if (tree->child[child_index]->child_len == 0)
        {
            // the child is leaf node
            if (child_index > 0)
                tree->data[child_index - 1] = tree->child[child_index]->data[0];
            if (tree->child[child_index]->data_len < 2)
                merge_leaf_node(tree, child_index);
                
        }
        else
        {
            // the child is internal node
            if (tree->child[child_index]->child_len < 2)
                merge_internal_node(tree, child_index);
        }
    }
    return 0;
}

int delete_tree(ElementType x, b_tree tree)
{
    if (tree == NULL)
        return -1;
    if (delete_tree_in(x, tree) < 0)
        return -1;
    if (tree->child_len == 1)
    {
        // delete root
        position tree_child0 = tree->child[0];
        tree->child = tree_child0->child;
        tree->data = tree_child0->data;
        tree->child_len = tree_child0->child_len;
        tree->data_len = tree_child0->data_len;
        free(tree_child0);
    }
    return 0;
}

int print_tree(b_tree tree)
{
    if (tree == NULL)
        return 0;
    else
    {
        if (tree->child_len > 0)
        {
            printf("internal_node: ");
            for (int i = 0; i < tree->data_len; i++)
                printf("%d ", tree->data[i]);
            printf("\n");
            for (int i = 0; i < tree->child_len; i++)
                print_tree(tree->child[i]);
        }
        else
        {
            printf("layer_node: ");
            for (int i = 0; i < tree->data_len; i++)
                printf("%d ", tree->data[i]);
            printf("\n");
        }
    }
}

int create_array(int *array, int len)
{
    if (array != NULL && len <= 0)
    {
        return -1;
    }
    for (int i = 0; i < len; i++)
        array[i] = i;
    for (int i = 0; i < len; i++)
    {
        int r = rand() % len;
        int tmp = array[i];
        array[i] = array[r];
        array[r] = tmp;
        // array[i] = r;
    }
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

int main()
{
    srand(time(NULL)); // Initialization, should only be called once.
    int number = 19;
    // int array[number];
    // if (create_array(array, number) == -1)
    //     return -1;
    int array[19] = {1, 0, 5, 10, 8, 7, 3, 9, 6, 12, 14, 13, 4, 15, 16, 17, 18, 19, 20};
    printf("\narray: ");
    print_array(array, number);

    b_tree tree = NULL;
    for (int i = 0; i < number; i++)
        tree = insert_tree(array[i], tree);
    printf("\ntree: ");
    print_tree(tree);

    int array_del[19] = {5, 13, 17, 4, 8, 19, 16, 9, 6, 10, 14, 1, 18, 15, 12, 3, 7, 14, 20};
    for (int i = 0; i < 19; i++)
    {
        if (delete_tree(array_del[i], tree) < 0)
            return -1;
        printf("\n\nafter deleting %d, the tree: \n", array_del[i]);
        print_tree(tree);
    }
}