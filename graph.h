#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef enum StatusCode {
    OK = 0,
    MEMORY_ERROR = 1,
    INVALID_ARGUMENT = 2,
    DUDE_DUPLICATE_ERROR = 3,
    EDGE_DUPLICATE_ERROR = 4,
    NO_CHANGE_ERROR = 5,
    EDGE_NOT_EXIST_ERROR = 6,
    DUDE_NOT_EXIST_ERROR = 7,
    NAME_DUPLICATE_ERROR = 8,
    DUDE_CANT_BE_FOUND = 9
} StatusCode;

typedef enum {
    MALE = 0,
    FEMALE = 1
} Sex;

typedef enum {
    PARENT = 0,
    CHILD = 1
} Type;

typedef struct Edge {
    int to;
    Type type;
    struct Edge* next;
} Edge;

typedef struct Dude {
    char* name;
    Sex sex;
    int birth; // INT_MIN, в случае, если неизвестно
    int death; // INT_MAX в случае, если почему-то жив
    Edge* edges; 
} Dude;

typedef struct Graph {
    Dude** dudes;
    int size;
} Graph;


typedef struct DistMatrix {
    int** data;
    int size;
} DistMatrix;


Graph* create_graph();
void free_graph(Graph* graph);
int search_dude(Graph* graph, const char* name);
StatusCode add_dude(Graph* graph, const char* name, Sex sex, int birth, int death);
StatusCode delete_dude_by_name(Graph* graph, const char* name);
StatusCode edit_dude_by_name(Graph* graph, const char* old_name, const char* new_name, Sex new_gender, int new_birth, int new_death);
StatusCode add_edge_by_names(Graph* graph, const char* from_name, const char* to_name, Type type);
StatusCode delete_edge_by_names(Graph* graph, const char* from_name, const char* to_name);
StatusCode edit_edge_by_names(Graph* graph, const char* from_name, const char* to_name, Type type);
StatusCode shortest_path_by_names(Graph* graph, const char* from_name, const char* to_name, int* answer);
StatusCode dfs_male_oldest_by_names(Graph* graph, const char* from_name, int* answer);
StatusCode distribute_money_by_name(Graph* graph, const char* name, double summ, float* answer);
void print_dude(Graph* graph, int index);
void print_graph(Graph* graph);
StatusCode export_to_dot(Graph* graph, const char* filename);
StatusCode generate_image(const char* filename);

#endif