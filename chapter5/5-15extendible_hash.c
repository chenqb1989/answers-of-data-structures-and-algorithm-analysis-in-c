/*
note:
1. I refer to the work https://github.com/nitish6174/extendible-hashing/blob/master/main.cpp, it is great, and 
    I just rewrite the 'insert' function, so pls refer to that
2. note the hash function, the above work use low bits of a key to map to some leaf, 
    instead of high bits, which is used in the book.
3. splitting leaf node is difficult. There are two cases: 
    one is that the depth of the leaf is equal to the depth of the table. In this case, splitting the leaf node 
    will increase the depth of the table, and then increase the number of directories, and let the newly added 
    directory points to the old leaf nodes (line: 159-171). In addition, allocate space for the 'pair_index' leaf node(line: 179-180). 
    Then, clear the elements of the leaf node to be split and reinsert them into the hash table (line: 187-195). 
    If the leaf node is successfully splitted after reinserting, continue inserting new elements. 
    Otherwise, repeat the process.

    The second case is that the depth of the leaf is less than the depth of the table. At this time, the space 
    is allocated for the leaf node at 'pair_index'. At the same time, the leaves whose distance from 'pair_index' is 'index_diff'
     are updated so that they point to the newly allocated leaf (line: 179-185). Then, clear the elements of the leaf node to be split 
     and reinsert them into the hash table. If the leaf node is successfully splitted after reinserting, 
     continue inserting new elements. Otherwise, repeat the process.
分裂叶子节点很困难。
分为两种情况，一种是叶子的depth等于表的depth，此时分裂叶子节点增加表的depth，进而增加目录数量，让新增加的目录指向原来目录所指的叶子节点。另外，为pair_index处的叶子节点重新分配空间。然后，清空要分裂叶子节点元素，重新将要分裂的叶子节点中的元素插入到该hash表。如果重新插入后，叶子节点被成功分裂，则继续插入新元素。否则，重复上述过程。
第二种情况是叶子的depth小于表的depth。此时为pair_index处的叶子节点重新分配空间，同时，更新和pair_index距离为index_diff的叶子，使他们指向新分配的叶子。然后清空要分裂叶子节点元素，重新将要分裂的叶子节点中的元素插入到该hash表。如果重新插入后，叶子节点被成功分裂，则继续插入新元素。否则，重复上述过程。
*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define ElementType u_int8_t
#define M 4
#define DirIncreaseStep 256
// #define MinTableSize 100

struct hash_entry;
typedef struct hash_entry *leaf;
struct hashtbl;
typedef struct hashtbl *hash_table;

struct hash_entry
{
    ElementType *element;
    int elem_num;
    int depth;
};


struct hashtbl
{
    int table_depth;
    int dir_num;
    int dir_size;
    leaf *the_dirs;
};

leaf create_leaf(int depth)
{
    leaf l = malloc(sizeof(struct hash_entry));
    if (l == NULL)
    {
        printf("out of space!\n");
        return NULL;
    }
    l->element = malloc(sizeof(ElementType) * M);
    if (l->element == NULL)
    {
        printf("out of space!\n");
        return NULL;
    }
    l->elem_num = 0;
    l->depth = depth;
    return l;
}

leaf copy_leaf(leaf l)
{
    if (l == NULL)
        return NULL;
    
    leaf l_copy = create_leaf(l->depth);
    if (l_copy == NULL)
        return NULL;
    l_copy->elem_num = l->elem_num;
    memcpy(l_copy->element, l->element, sizeof(ElementType) * l->elem_num);
    return l_copy;
}

int expand_dir(hash_table h, int new_dir_size)
{
    int old_dir_size = h->dir_size;
    if (old_dir_size >= new_dir_size)
        return 0;
    
    leaf *temp = malloc(sizeof(leaf) * old_dir_size);
    if (temp == NULL)
    {
        printf("out of space!\n");
        return -1;
    }
    memcpy(temp, h->the_dirs, sizeof(leaf) * old_dir_size);

    h->the_dirs = malloc(sizeof(leaf) * new_dir_size);
    if (h->the_dirs == NULL)
    {
        printf("out of space!\n");
        return -1;
    }
    memcpy(h->the_dirs, temp, sizeof(leaf) * old_dir_size);

    for (int i = old_dir_size; i < new_dir_size; i++)
    {
        h->the_dirs[i] = create_leaf(1);
        if (h->the_dirs[i] == NULL)
        {
            return -1;
        }
    }
    h->dir_size = new_dir_size;
}

hash_table initialize_table()
{
    hash_table h = malloc(sizeof(struct hashtbl));
    if (h == NULL)
    {
        printf("out of space!\n");
        return NULL;
    }
    h->table_depth = 1;
    h->dir_num = 1 << h->table_depth;
    h->dir_size = DirIncreaseStep;
    h->the_dirs = malloc(sizeof(leaf *) * h->dir_size);
    if (h->the_dirs == NULL)
    {
        printf("out of space!\n");
        return NULL;
    }
    for (int i = 0; i < h->dir_num; i++)
    {
        h->the_dirs[i] = create_leaf(h->table_depth);
        if (h->the_dirs[i] == NULL)
        {
            return NULL;
        }
    }
    return h;
}

int hash(ElementType key, int dir_bit)
{
    return key & ((1 << dir_bit) - 1);
}

int insert(ElementType key, hash_table h)
{
    if (h == NULL)
        return -1;
    int p = hash(key, h->table_depth);

    if (h->the_dirs[p]->elem_num == M)
    {
        h->the_dirs[p]->depth++;
        if (h->the_dirs[p]->depth > h->table_depth)
        {
            h->table_depth++;
            if ((1 << h->table_depth) <= h->dir_size)
            {
                for (int i = 0; i < (1 << (h->table_depth - 1)); i++)
                {
                    h->the_dirs[h->dir_num++] = h->the_dirs[i];
                }
            }
            else
            {
                if (expand_dir(h, h->dir_size + DirIncreaseStep) < 0)
                    return -1;
            }
        }

        int pair_index = p ^ (1 << (h->the_dirs[p]->depth - 1));
        h->the_dirs[pair_index] = create_leaf(h->the_dirs[p]->depth);
        int index_diff = 1 << h->the_dirs[p]->depth;
        for (int i = pair_index - index_diff; i >= 0; i -= index_diff)
            h->the_dirs[i] = h->the_dirs[pair_index];
        for (int i = pair_index + index_diff; i < h->dir_num; i += index_diff)
            h->the_dirs[i] = h->the_dirs[pair_index];
        
        leaf temp = copy_leaf(h->the_dirs[p]);
        if (temp == NULL)
            return -1;
        h->the_dirs[p]->elem_num = 0;

        for (int i = 0; i < temp->elem_num; i++)
        {
            if (insert(temp->element[i], h) < 0)
                return -1;
        }
        insert(key, h);

    }
    else
    {
        h->the_dirs[p]->element[h->the_dirs[p]->elem_num++] = key;
    }
    return 0;
}

int print_hash_table(hash_table h)
{
    if (h == NULL)
        return 0;
    for (int i = 0; i < h->dir_num; i++)
    {
        printf("\ndir: %d, node: ", i);
        for (int j = 0; j < h->the_dirs[i]->elem_num; j++)
            printf("%u ", h->the_dirs[i]->element[j]);
    }
    return 0;
}

int main()
{
    hash_table h = initialize_table();
    int number = 11;
    int array[11] = {1, 16, 32, 64, 128, 48, 3, 5, 7, 9, 11};
    for (int i = 0; i < number; i++)
    {
        if (insert(array[i], h) < 0)
            return -1;
    }
    print_hash_table(h);

    return 0;
}
