/*
note:
1. note how to store edge weight
2. one heap node is the pointer to one table entry, and the heap is built based on dist of table entry
3. note decrease_key is based on node index, so we have to define and update pos_in_heap
4. note the difference of entries in table and heap
5. note how to get pos of a node in heap and table
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MaxVertexNum 100
#define DistType int
#define NotAVertex (-1)
#define INT32_MAX __INT32_MAX__
#define MinPQSize 1
#define MinData -1

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
    int pos_in_heap;  // position in heap
    int pos_in_table;  // position in table
};
typedef struct table_entry *ptr_to_entry;

struct t_table
{
    int entry_num;
    ptr_to_entry entries;
};
typedef struct t_table *ptr_to_table;

/* heap struct */
struct heap_struct;
typedef struct heap_struct *priority_queue;

struct heap_struct
{
    int capacity;
    int size;
    ptr_to_entry *entries;
};

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

/* ******heap func****** */
priority_queue initialize(int max_elments)
{
    priority_queue h;

    if (max_elments < MinPQSize)
    {
        printf("priority queue size is too small\n");
        return NULL;
    }

    h = malloc(sizeof(struct heap_struct));
    if (h == NULL)
    {
        printf("out of space\n");
        return NULL;
    }

    h->entries = malloc(sizeof(ptr_to_entry) * (max_elments + 1));
    if (h->entries == NULL)
    {
        printf("out of space\n");
        return NULL;
    }

    h->capacity = max_elments;
    h->size = 0;
    h->entries[0] = malloc(sizeof(struct table_entry));
    if (h->entries[0] == NULL)
    {
        printf("out of space\n");
        return NULL;
    }

    h->entries[0]->dist = MinData;
    h->entries[0]->pos_in_heap = 0;
    return h;
}

int is_full(priority_queue h)
{
    if (h->size == h->capacity)
        return 1;
    else
        return 0;
}

int is_empty(priority_queue h)
{
    if (h->size == 0)
        return 1;
    else
        return 0;
}

ptr_to_entry delete_min(priority_queue h)
{
    if (is_empty(h))
    {
        printf("the priority queue is empty!\n");
        return NULL;
    }

    ptr_to_entry min_element = h->entries[1];
    ptr_to_entry last_element = h->entries[h->size--];
    int child, i;
    for (i = 1; i * 2 <= h->size; i = child) // note the termination condition
    {
        child = i * 2;
        if (child != h->size && h->entries[child + 1]->dist < h->entries[child]->dist)
            child++;
        
        if (last_element->dist > h->entries[child]->dist)
        {
            h->entries[i] = h->entries[child];
            h->entries[i]->pos_in_heap = i;
        }
        else
            break;
    }
    h->entries[i] = last_element;
    h->entries[i]->pos_in_heap = i;
    return min_element;
}

void percolate_up(priority_queue h, int index)
{
    int i = index;
    for (; h->entries[i / 2]->dist > h->entries[i]->dist; i /= 2)
    {
        h->entries[i]->pos_in_heap = i / 2;
        h->entries[i / 2]->pos_in_heap = i;

        ptr_to_entry temp = h->entries[i];
        h->entries[i] = h->entries[i / 2];
        h->entries[i / 2] = temp;
    }
}

void percolate_down(priority_queue h, int index)
{
    int child, i;
    for (i = index; i * 2 <= h->size; i = child)
    {
        child = i * 2;
        if (child != h->size && h->entries[child + 1]->dist < h->entries[child]->dist)
            child++;
        
        if (h->entries[i]->dist > h->entries[child]->dist)
        {
            h->entries[i]->pos_in_heap = child;
            h->entries[child]->pos_in_heap = i;

            ptr_to_entry temp = h->entries[i];
            h->entries[i] = h->entries[child];
            h->entries[child] = temp;
        }
        else
            break;
    }
}

priority_queue build_heap(ptr_to_table table)
{
    priority_queue h = initialize(MaxVertexNum);
    
    for (int i = 0; i < table->entry_num; i++)
        h->entries[i + 1] = &table->entries[i];
    h->size = table->entry_num;
    for (int i = h->size / 2; i > 0; i--)
        percolate_down(h, i);
    
    return h;
}

int decrease_key(int index, DistType delta, priority_queue h)
{
    if (delta < 0)
    {
        printf("delta is negative\n");
        return -1;
    }
    if (h == NULL)
    {
        printf("the priority query is NULL\n");
        return -1;
    }
    if (index > h->size || index < 1)
    {
        printf("out of range\n");
        return -1;
    }

    h->entries[index]->dist -= delta;
    percolate_up(h, index);

    return 0;
}

void free_heap(priority_queue h)
{
    if (h == NULL)
        return;
    free(h->entries[0]);
    free(h->entries);
    free(h);
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
        table->entries[i].pos_in_heap = i + 1;  // pos 0 is heap header
        table->entries[i].pos_in_table = i;
    }
    table->entries[start].dist = 0;
    return table;
}

void free_table(ptr_to_table table)
{
    if (table == NULL)
        return;
    free(table->entries);
    free(table);
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

void dijkstra(ptr_to_table table)
{
    priority_queue h = build_heap(table);
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
                decrease_key(table->entries[w->vertex].pos_in_heap, (table->entries[w->vertex].dist - (v->dist + w->weight)), h);
                table->entries[w->vertex].path = v->pos_in_table;
            }
            w = w->next;
        }
    }
    free(h);
}

int main()
{
    int vertex_num = 7;
    int edge_num = 12;
    int start = 1;
    int edges[12][3] = {{1, 2, 2}, {1, 4, 1}, {2, 4, 3}, {2, 5, 10}, {3, 1, 4}, {3, 6, 5}, 
            {4, 3, 2}, {4, 5, 2}, {4, 6, 8}, {4, 7, 4}, {5, 7, 6}, {7, 6, 1}};

    ptr_to_graph graph;
    ptr_to_table table;

    graph = create_graph(vertex_num, edges, edge_num);
    print_graph(graph);
    table = create_table(start - 1, graph);
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

