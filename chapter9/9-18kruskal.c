/*
note:
1. note we assume that from_vertex is smaller than to_vertex, so that we only store once for each undirected edge
2. one heap node is the pointer to one edge, and the heap is built based on edge weight
3. note the first element of disjset is meaningless header (line 331)
4. note we should union the result of find, for they are roots. (line: 394-398)
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MaxVertexNum 100
#define INT32_MAX __INT32_MAX__
#define MinPQSize 1
#define MinData -1

typedef int ElementType;
/* edge struct */
struct t_edge;
typedef struct t_edge *ptr_to_edge;

struct t_edge
{
    int from_vertex;  // edge from vertex, note we assume from_vertex is smaller than to_vertex
    int to_vertex;  // edge to vertex
    int weight;  // edge weight
    ptr_to_edge next;
};

/* graph struct */
struct t_graph;
typedef struct t_graph *ptr_to_graph;

struct t_graph
{
    int vertex_num;
    int edge_num;
    ptr_to_edge *edges;
};

/* heap struct */
struct heap_struct;
typedef struct heap_struct *priority_queue;

struct heap_struct
{
    int capacity;
    int size;
    ptr_to_edge *edges;
};

/* disjset struct */
struct disjset_struct;
typedef struct disjset_struct *ptr_to_disjset;

struct disjset_struct
{
    int set_size;
    int *nodes;
};


/* ******edge func****** */
int insert_edge(int from_vertex, int to_vertex, int weight, ptr_to_edge p)
{
    ptr_to_edge tmp_cell = (ptr_to_edge)malloc(sizeof(struct t_edge));
    if (tmp_cell == NULL)
        return -1;
    tmp_cell->from_vertex = from_vertex;
    tmp_cell->to_vertex = to_vertex;
    tmp_cell->weight = weight;
    tmp_cell->next = p->next;
    p->next = tmp_cell;
    return 0;
}

void init_edge(ptr_to_edge lst)
{
    lst->from_vertex = -1;
    lst->to_vertex = -1;
    lst->weight = 0;
    lst->next = NULL;
}

// print edges of one node with header l
void print_node_edges(ptr_to_edge l)
{
    ptr_to_edge p = l->next;
    while (p != NULL)
    {
        printf(" to %d weight %d; ", p->to_vertex + 1, p->weight);
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

/* ****** graph func ******* */
ptr_to_graph create_graph(int vertex_num, int edges[][3], int edge_num)
{
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
    graph->edge_num = edge_num;
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
        if (insert_edge(edges[i][0] - 1, edges[i][1] - 1, edges[i][2], graph->edges[edges[i][0] - 1]) < 0)
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
            print_node_edges(graph->edges[i]);
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

/* ******heap func****** */
priority_queue initialize_heap(int max_elments)
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

    h->edges = malloc(sizeof(ptr_to_edge) * (max_elments + 1));
    if (h->edges == NULL)
    {
        printf("out of space\n");
        return NULL;
    }

    h->capacity = max_elments;
    h->size = 0;
    h->edges[0] = malloc(sizeof(struct t_edge));
    if (h->edges[0] == NULL)
    {
        printf("out of space\n");
        return NULL;
    }

    h->edges[0]->weight = MinData;
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

ptr_to_edge delete_min(priority_queue h)
{
    if (is_empty(h))
    {
        printf("the priority queue is empty!\n");
        return NULL;
    }

    ptr_to_edge min_element = h->edges[1];
    ptr_to_edge last_element = h->edges[h->size--];
    int child, i;
    for (i = 1; i * 2 <= h->size; i = child) // note the termination condition
    {
        child = i * 2;
        if (child != h->size && h->edges[child + 1]->weight < h->edges[child]->weight)
            child++;
        
        if (last_element->weight > h->edges[child]->weight)
            h->edges[i] = h->edges[child];
        else
            break;
    }
    h->edges[i] = last_element;
    return min_element;
}

void percolate_down(priority_queue h, int index)
{
    int child, i;
    for (i = index; i * 2 <= h->size; i = child)
    {
        child = i * 2;
        if (child != h->size && h->edges[child + 1]->weight < h->edges[child]->weight)
            child++;
        
        if (h->edges[i]->weight > h->edges[child]->weight)
        {
            ptr_to_edge temp = h->edges[i];
            h->edges[i] = h->edges[child];
            h->edges[child] = temp;
        }
        else
            break;
    }
}

priority_queue build_heap(ptr_to_graph graph)
{
    priority_queue h = initialize_heap(graph->edge_num);
    int edge_num = 0;
    for (int i = 0; i < graph->vertex_num; i++)
    {
        ptr_to_edge edge = graph->edges[i]->next;
        while (edge != NULL)
        {
            h->edges[++edge_num] = edge;
            edge = edge->next;
        }
    }
    h->size = graph->edge_num;
    for (int i = h->size / 2; i > 0; i--)
        percolate_down(h, i);
    
    return h;
}

/* union func */
ptr_to_disjset create_disjset(int set_size)
{
    ptr_to_disjset s = malloc(sizeof(struct disjset_struct));
    if (s == NULL)
    {
        printf("out of space!\n");
        return NULL;
    }
    s->set_size = set_size + 1;
    s->nodes = malloc(sizeof(int) * s->set_size); // note the first element is header
    if (s->nodes == NULL)
    {
        printf("out of space!\n");
        return NULL;
    }
    for (int i = s->set_size; i > 0; i--)
        s->nodes[i] = 0;
    
    return s;
}

void free_disjset(ptr_to_disjset s)
{
    if (s != NULL)
    {
        free(s->nodes);
        free(s);
    }
}

// assume root1 and root2 are roots
void set_union_height(ptr_to_disjset s, int root1, int root2)
{
    if(s->nodes[root2] < s->nodes[root1])
    {
        s->nodes[root1] = root2;
    }
    else
    {
        if (s->nodes[root1] == s->nodes[root2])
            s->nodes[root1]--;
        s->nodes[root2] = root1;
    }
}

int find_height_compression(ElementType x, ptr_to_disjset s)
{
    if (s->nodes[x] <= 0)
        return x;
    else
        return s->nodes[x] = find_height_compression(s->nodes[x], s);
}

ptr_to_edge *krustal(ptr_to_graph graph)
{
    ptr_to_edge *result = malloc(sizeof(ptr_to_edge) * (graph->vertex_num - 1));
    if (result == NULL)
        return NULL;
    ptr_to_disjset s = create_disjset(graph->vertex_num);
    if (s == NULL)
        return NULL;
    
    priority_queue h = build_heap(graph);
    if (h == NULL)
        return NULL;
    
    int edges_accepted = 0;
    while (edges_accepted < graph->vertex_num - 1)
    {
        ptr_to_edge edge = delete_min(h);
        // in dijset, first element is header, but in heap and graph, from_vertex can be zero, so let it add 1
        int u = find_height_compression(edge->from_vertex + 1, s);
        int v = find_height_compression(edge->to_vertex + 1, s);
        if (u != v)
        {
            set_union_height(s, u, v);
            result[edges_accepted++] = edge;
        }
    }

    // free heap and disjset
    free(h->edges);
    free(h);
    free(s->nodes);
    free(s);
    return result;
}

void print_edges(ptr_to_edge *edges, int edge_num)
{
    if (edges != NULL)
    {
        for (int i = 0; i < edge_num; i++)
            printf("\n%d to %d with weight %d", edges[i]->from_vertex + 1, edges[i]->to_vertex + 1, edges[i]->weight);
    }
}

int main()
{
    int vertex_num = 7;
    int edge_num = 12;
    int edges[12][3] = {{1, 2, 2}, {1, 4, 1}, {2, 4, 3}, {2, 5, 10}, {1, 3, 4}, {3, 6, 5}, 
            {3, 4, 2}, {4, 5, 2}, {4, 6, 8}, {4, 7, 4}, {5, 7, 6}, {6, 7, 1}};
    ptr_to_graph graph = create_graph(vertex_num, edges, edge_num);
    printf("graph: ");
    print_graph(graph);
    ptr_to_edge *result = krustal(graph);
    printf("\nresult: ");
    print_edges(result, graph->vertex_num - 1);
    printf("\n");
    free(result);
}
