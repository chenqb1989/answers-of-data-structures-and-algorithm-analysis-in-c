/*
note:
1. pls refer to the work about fibonacci heap https://greeensy.github.io/2013/04/16/Fibonacci-Dijkstra/, the code is from there
2. note to update fib_node attributes timely and accurately, which is errorprone
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MaxTrees 100
#define MaxVertexNum 100
#define DistType int
#define NotAVertex (-1)
#define INT32_MAX __INT32_MAX__
#define MinPQSize 1
#define MinData -1

/* fibonacci heap struct */
struct fib_node;
struct collection;
typedef struct fib_node *position;
typedef struct fib_node *fib_tree;
typedef struct collection *fib_heap;
struct table_entry;
typedef struct table_entry *ptr_to_entry;


struct fib_node
{
    position parent;
    position prev;
    position next;
    position child;
    int degree;
    char marked;
    ptr_to_entry element;
};

struct collection
{
    fib_tree min_root;
    fib_tree trees[MaxTrees];
};

/* edge struct */
struct t_edge;
typedef struct t_edge *ptr_to_edge;

struct t_edge
{
    int vertex;  // edge to vertex
    int weight;  // edge weight
    ptr_to_edge next;
};

/* graph struct */
struct t_graph;
typedef struct t_graph *ptr_to_graph;

struct t_graph
{
    int vertex_num;
    ptr_to_edge *edges;
};

/* table struct */
struct table_entry
{
    ptr_to_edge header;  // adjacency list
    int known;
    DistType dist;
    int path;
    fib_tree heap_node;  // pointer to node in heap
    int pos_in_table;  // position in table
};

struct t_table
{
    int entry_num;
    ptr_to_entry entries;
};
typedef struct t_table *ptr_to_table;

/* ******edge func****** */
int insert_edge(int vertex, int weight, ptr_to_edge p)
{
    ptr_to_edge tmp_cell = (ptr_to_edge)malloc(sizeof(struct t_edge));
    if (tmp_cell == NULL)
        return -1;
    tmp_cell->vertex = vertex;
    tmp_cell->weight = weight;
    tmp_cell->next = p->next;
    p->next = tmp_cell;
    return 0;
}

void init_edge(ptr_to_edge lst)
{
    lst->vertex = -1;
    lst->weight = 0;
    lst->next = NULL;
}

void print_edges(ptr_to_edge l)
{
    ptr_to_edge p = l->next;
    while (p != NULL)
    {
        printf(" to %d weight %d; ", p->vertex + 1, p->weight);
        p = p->next;
    }
}

int free_edge(ptr_to_edge lst)
{
    ptr_to_edge del_p;
    while (lst != NULL)
    {
        del_p = lst;
        lst = lst->next;
        free(del_p);
    }
    free(lst);
    return 0;
}

/* fibonacci heap func */
fib_heap create_fib_heap()
{
    fib_heap heap = malloc(sizeof(struct collection));
    if (heap == NULL)
    {
        printf("out of space!");
        return NULL;
    }
    heap->min_root = NULL;
    return heap;
}

fib_tree insert(fib_heap heap, ptr_to_entry x)
{
    position tree = malloc(sizeof(struct fib_node));
    if (tree == NULL)
    {
        printf("out of space!");
        return NULL;
    }
    tree->child = NULL;
    tree->parent = NULL;
    tree->degree = 0;
    tree->element = x;
    tree->marked = 0;
    
    if (heap->min_root)
    {
        tree->prev = heap->min_root;
        tree->next = heap->min_root->next;
        heap->min_root->next->prev = tree;
        heap->min_root->next = tree;
        if (tree->element->dist < heap->min_root->element->dist)
            heap->min_root = tree;
    }
    else
    {
        tree->prev = tree->next = tree;
        heap->min_root = tree;
    }
    return tree;
}

char is_empty(fib_heap heap)
{
    return heap->min_root == NULL;
}

ptr_to_entry find_min(fib_heap heap)
{
    if (is_empty(heap))
    {
        printf("the heap is empty!");
        return NULL;
    }
    else
        return heap->min_root->element;
}

ptr_to_entry delete_min(fib_heap heap)
{
     if (is_empty(heap))
    {
        printf("the heap is empty!");
        return NULL;
    }
    position min_root = heap->min_root;

    // get children of min_root
    position child = heap->min_root->child;
    position curr_node;
    if (child != NULL)
    {
        curr_node = child;
        do{
            curr_node->parent = NULL;
            curr_node->marked = 0;  // update marked
            curr_node = curr_node->next;
        } while (child != curr_node);

        child->prev->next = min_root->next;
        min_root->next->prev = child->prev;
        child->prev = min_root;
        min_root->next = child;
    }

    // combine trees with same degree
    curr_node = min_root->next;
    position next_node;
    while (curr_node != min_root)
    {
        next_node = curr_node->next;
        int degree = curr_node->degree;
        position target = heap->trees[degree];
        while (target != NULL) /* combine curr_node and target */ 
        {
            if (curr_node->element->dist > target->element->dist)
            {
                position temp = curr_node;
                curr_node = target;
                target = temp;
            }

            // get rid of target from bidirection list
            target->prev->next = target->next;
            target->next->prev = target->prev;

            // combine
            if (curr_node->child)
            {
                target->next = curr_node->child;
                target->prev = curr_node->child->prev;
                curr_node->child->prev->next = target;
                curr_node->child->prev = target;
            }
            else
            {
                curr_node->child = target->prev = target->next = target;
            }
            target->parent = curr_node;

            heap->trees[degree] = NULL;
            degree = curr_node->degree++;
            target = heap->trees[curr_node->degree];
        }
        heap->trees[degree] = curr_node;
        curr_node = next_node;
    }

    // find new min and clear heap->trees
    curr_node = min_root->next;
    position min_temp = NULL;
    while (curr_node != min_root)
    {
        if (min_temp == NULL || curr_node->element->dist < min_temp->element->dist)
            min_temp = curr_node;
        heap->trees[curr_node->degree] = NULL;
        curr_node = curr_node->next;
    }
    min_root->next->prev = min_root->prev;
    min_root->prev->next = min_root->next;
    ptr_to_entry min_root_value = min_root->element;
    free(min_root);
    heap->min_root = min_temp;
    return min_root_value;
}

void cut(position node, fib_heap heap)
{
    position curr_node = node;
    do {
        position parent = curr_node->parent;
        if (--parent->degree == 0)
            parent->child = NULL;
        else
        {
            // get rid of curr_node
            curr_node->prev->next = curr_node->next;
            curr_node->next->prev = curr_node->prev;
            parent->child = curr_node->next;
        }
        // add curr_node to roots
        curr_node->parent = NULL;
        curr_node->marked = 0;
        curr_node->next = heap->min_root->next;
        heap->min_root->next->prev = curr_node;
        curr_node->prev = heap->min_root;
        heap->min_root->next = curr_node;
        curr_node = parent;
    } while (curr_node->marked == 1);
    if (curr_node->parent)
        curr_node->marked = 1;
}

void decrease_key(position node, int delta, fib_heap heap)
{
    node->element->dist -= delta;
    if (node->parent)
    {
        if (node->element->dist >= node->parent->element->dist)
            return;
        cut(node, heap);
    }
    if (node->element->dist < heap->min_root->element->dist)
        heap->min_root = node;
}

void free_tree(position node)
{
    if (!node)
        return;
    position curr_node = node;
    do {
        free_tree(curr_node->child);
        position temp = curr_node;
        curr_node = curr_node->next;
        free(temp);
    } while (curr_node != node);
}

void free_fib_heap(fib_heap heap)
{
    free_tree(heap->min_root);
    heap->min_root = NULL;
}

/* ****** graph func ******* */
ptr_to_graph create_graph(int vertex_num, int edges[][3], int edge_num)
{
    // edges: contains 3 columns: "edge_from_vertex, edge_to_vertex, edge_weight"
    if (vertex_num > MaxVertexNum)
    {
        printf("too many vertexes!\n");
        return NULL;
    }
    ptr_to_graph graph = malloc(sizeof(struct t_graph));
    if (graph == NULL)
    {
        printf("out of space!\n");
        return NULL;
    }
        
    graph->vertex_num = vertex_num;
    graph->edges = malloc(MaxVertexNum * sizeof(ptr_to_edge));
    if (graph->edges == NULL)
    {
        printf("out of space!\n");
        return NULL;
    }

    for (int i = 0; i < vertex_num; i++)
    {
        graph->edges[i] = malloc(sizeof(struct t_edge));
        if (graph->edges[i] == NULL)
        {
            printf("out of space!\n");
            return NULL;
        }
        init_edge(graph->edges[i]);
    }

    // read edges
    for (int i = 0; i < edge_num; i++)
    {
        if (insert_edge(edges[i][1] - 1, edges[i][2], graph->edges[edges[i][0] - 1]) < 0)
            return NULL;
    }

    return graph;
}

void print_graph(ptr_to_graph graph)
{
    if (graph != NULL)
        for (int i = 0; i < graph->vertex_num; i++)
        {
            printf("\nvertex: %d:", i + 1);
            print_edges(graph->edges[i]);
        }
}

void free_graph(ptr_to_graph graph)
{
    if (graph != NULL)
    {
        for (int i = 0; i < graph->vertex_num; i++)
            free_edge(graph->edges[i]);
        
        free(graph->edges);
        free(graph);
    }
}

/* ********table*********** */
ptr_to_table create_table(int start, ptr_to_graph graph)
{
    if (graph == NULL || graph->vertex_num <= 0)
        return NULL;
    
    ptr_to_table table = malloc(sizeof(struct t_table));
    if (table == NULL)
    {
        printf("out of space!\n");
        return NULL;
    }

    table->entry_num = graph->vertex_num;
    table->entries = malloc(sizeof(struct table_entry) * table->entry_num);
    if (table->entries == NULL)
    {
        printf("out of space!\n");
        return NULL;
    }
    
    // initialize table
    for (int i = 0; i < table->entry_num; i++)
    {
        table->entries[i].header = graph->edges[i];
        table->entries[i].known = 0;
        table->entries[i].dist = INT32_MAX;
        table->entries[i].path = NotAVertex;
        table->entries[i].heap_node = NULL;
        table->entries[i].pos_in_table = i;
    }
    table->entries[start].dist = 0;
    return table;
}

void print_path(int vertex, ptr_to_table table)
{
    if (table->entries[vertex].path != NotAVertex)
    {
        print_path(table->entries[vertex].path, table);
        printf(" to ");
    }
    printf("%d", vertex + 1);
}

void free_table(ptr_to_table table)
{
    if (table == NULL)
        return;
    free(table->entries);
    free(table);
}

void dijkstra(ptr_to_table table)
{
    fib_heap h = create_fib_heap();
    // insert table entries into h
    for (int i = 0; i < table->entry_num; i++)
    {
        fib_tree tree = insert(h, &table->entries[i]);
        table->entries[i].heap_node = tree;
    }
    
    for(; ;)
    {
        ptr_to_entry v = delete_min(h);
        if (v == NULL)
            break;
        
        ptr_to_edge w = v->header->next;
        while (w != NULL)
        {
            if (v->dist + w->weight < table->entries[w->vertex].dist)
            {
                decrease_key(table->entries[w->vertex].heap_node, (table->entries[w->vertex].dist - (v->dist + w->weight)), h);
                table->entries[w->vertex].path = v->pos_in_table;
            }
            w = w->next;
        }
    }
    free_fib_heap(h);
}

int main()
{
    int vertex_num = 7;
    int edge_num = 12;
    int start = 1;
    int edges[12][3] = {{1, 2, 2}, {1, 4, 1}, {2, 4, 3}, {2, 5, 10}, {3, 1, 4}, {3, 6, 5}, 
            {4, 3, 2}, {4, 5, 2}, {4, 6, 8}, {4, 7, 4}, {5, 7, 6}, {7, 6, 1}};

    ptr_to_graph graph = create_graph(vertex_num, edges, edge_num);
    print_graph(graph);
    ptr_to_table table = create_table(start - 1, graph);
    dijkstra(table);
    for (int i = 0; i < table->entry_num; i++)
    {
        printf("\npath of vertex %d: ", i + 1);
        print_path(i, table);
    }
    free_graph(graph);
    free_table(table);
    
    printf("\n");
}