#include "graph.h"


Graph* create_graph() {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if (!graph)
        return NULL;

    graph->dudes = NULL;
    graph->size = 0;

    return graph;
}


Dude* create_dude(const char* name, Sex sex, int birth, int death) {
    Dude* dude = (Dude* )malloc(sizeof(Dude));
    if (!dude || !name)
        return NULL;
    
    dude->name = strdup(name);
    if (!dude->name) {
        free(dude);
        return NULL;
    }
    dude->sex = sex;
    dude->birth = birth;
    dude->death = death;
    dude->edges = NULL;
    
    return dude;
}


void free_dude(Dude* dude) {
    if (!dude)
        return;

    if (dude->name)
        free(dude->name);

    Edge* curr = dude->edges;
    while (curr) {
        Edge* next = curr->next;
        free(curr);
        curr = next;
    }

    free(dude);
}

void free_graph(Graph* graph) {
    if (!graph)
        return;

    if (graph->dudes) {
        for (int i = 0; i < graph->size; ++i)
            free_dude(graph->dudes[i]);
        free(graph->dudes);
    }

    free(graph);
} 


int search_dude(Graph* graph, const char* name) {
    if (!graph || !graph->dudes || !name)
        return -1;
    for (int i = 0; i < graph->size; ++i)
        if (strcmp(graph->dudes[i]->name, name) == 0)
            return i;
    return -1;
}


StatusCode add_dude(Graph* graph, const char* name, Sex gender, int birth, int death) {
    if (!graph || !name || name[0] == '\0' || death < birth)
        return INVALID_ARGUMENT;   // Некорретные исходные данные

    if (search_dude(graph, name) != -1)
        return DUDE_DUPLICATE_ERROR;

    Dude* dude = create_dude(name, gender, birth, death);
    if (!dude)
        return MEMORY_ERROR;   // Ошибка памяти

    graph->dudes = (Dude** )realloc(graph->dudes, (graph->size + 1) * sizeof(Dude* ));
    if (!graph->dudes) {
        free(dude);
        return MEMORY_ERROR; // Ошибка памяти
    }

    graph->dudes[graph->size] = dude;
    ++graph->size;

    return OK; // Всё ок
}


StatusCode delete_dude(Graph* graph, int index) {
    if (!graph || index < 0 || index >= graph->size) {
        return INVALID_ARGUMENT; // Некорректные данные
    }

    for (int i = 0; i < graph->size; ++i) {
        Edge** prev_next = &graph->dudes[i]->edges;
        Edge* current = graph->dudes[i]->edges;

        while (current != NULL) {
            if (current->to == index) {
                Edge* to_remove = current;
                *prev_next = current->next;
                current = current->next;
                free(to_remove);
            } else {
                prev_next = &current->next;
                current = current->next;
            }
        }
    }

    free_dude(graph->dudes[index]);

    if (index != graph->size - 1) {
        Dude* last_dude = graph->dudes[graph->size - 1];
        graph->dudes[index] = last_dude;

        for (int i = 0; i < graph->size - 1; ++i) {
            Edge* current = graph->dudes[i]->edges;
            while (current != NULL) {
                if (current->to == graph->size - 1)
                    current->to = index;
                current = current->next;
            }
        }
    }

    graph->size--;

    if (graph->size > 0) {
        graph->dudes = (Dude** )realloc(graph->dudes, graph->size * sizeof(Dude*));
        if (!graph->dudes)
            return MEMORY_ERROR; // Ошибка памяти
    } else {
        free(graph->dudes);
        graph->dudes = NULL;
    }

    return OK;
}


StatusCode delete_dude_by_name(Graph* graph, const char* name) {
    if (!graph || !name)
        return INVALID_ARGUMENT;

    int index = search_dude(graph, name);
    if (index == -1)
        return DUDE_NOT_EXIST_ERROR;

    return delete_dude(graph, index);
}


StatusCode edit_dude(Graph* graph, int index, const char* new_name, Sex new_sex, int new_birth, int new_death) {
    if (!graph || index < 0 || index >= graph->size || new_death < new_birth) {
        return INVALID_ARGUMENT;
    }
    bool flag = false;
    Dude* dude = graph->dudes[index];

    if (new_name != NULL && new_name != graph->dudes[index]->name) {
        for (int i = 0; i < graph->size; ++i)
            if (i != index && strcmp(graph->dudes[i]->name, new_name) == 0)
                return NAME_DUPLICATE_ERROR;
        

        free(dude->name);
        dude->name = strdup(new_name);
        if (!dude->name)
            return MEMORY_ERROR;
        
        flag = true;
    }

    if (new_sex >= 0 && new_sex != dude->sex) {
        dude->sex = new_sex;
        flag = true;
    }
    if (new_birth != dude->birth) {
        dude->birth = new_birth;
        flag = true;
    }
    if (new_death != dude->death) {
        dude->death = new_death;
        flag = true;
    }

    if (flag)
        return OK;
    return NO_CHANGE_ERROR;
}


StatusCode edit_dude_by_name(Graph* graph, const char* old_name, const char* new_name, Sex new_gender, int new_birth, int new_death) {
    if (!graph || !old_name)
        return INVALID_ARGUMENT;

    int index = search_dude(graph, old_name);
    if (index == -1)
        return DUDE_NOT_EXIST_ERROR;

    return edit_dude(graph, index, new_name, new_gender, new_birth, new_death);
}


StatusCode add_edge(Graph* graph, int from, int to, Type type) {
    if (!graph || !graph->size || from < 0 || from >= graph->size || to < 0 || to >= graph->size || from == to)
        return INVALID_ARGUMENT; // Неправильные исходные данные

    Edge* curr = graph->dudes[from]->edges;
    
    while (curr) {
        if (curr->to == to)
            return EDGE_DUPLICATE_ERROR; // Есть уже такой эдге
        curr = curr->next;
    }

    Edge* edge = (Edge*)malloc(sizeof(Edge));
    if (!edge)
        return MEMORY_ERROR; // Ошибка памяти

    edge->to = to;
    edge->type = type;
    edge->next = graph->dudes[from]->edges;
    graph->dudes[from]->edges = edge;

    return OK;
}


StatusCode add_edge_by_names(Graph* graph, const char* from_name, const char* to_name, Type type) {
    if (!graph || !from_name || !to_name)
        return INVALID_ARGUMENT;
    
    int from = search_dude(graph, from_name);
    int to = search_dude(graph, to_name);
    
    if (from == -1 || to == -1)
        return DUDE_NOT_EXIST_ERROR;
    
    StatusCode cond = add_edge(graph, from, to, type);
    if (cond == OK)
        return add_edge(graph, to, from, (type + 1) % 2);
    
    return cond;
}


StatusCode delete_edge(Graph* graph, int from, int to) {
    if (!graph || from < 0 || from >= graph->size || to < 0 || to >= graph->size || from == to)
        return INVALID_ARGUMENT;

    bool flag = false;
    Edge** prev_next = &graph->dudes[from]->edges;
    Edge* curr = graph->dudes[from]->edges;
    Edge* target = NULL;

    while (curr != NULL) {
        if (curr->to == to) {
            target = curr;
            *prev_next = curr->next;
            curr = curr->next;
            free(target);
            flag = true;
        }
        else {
            prev_next = &curr->next;
            curr = curr->next;
        }
    }

    if (flag)
        return OK;
    else
        return EDGE_NOT_EXIST_ERROR;
}


StatusCode delete_edge_by_names(Graph* graph, const char* from_name, const char* to_name) {
    if (!graph || !from_name || !to_name)
        return INVALID_ARGUMENT;
    
    int from = search_dude(graph, from_name);
    int to = search_dude(graph, to_name);
    
    if (from == -1 || to == -1)
        return DUDE_NOT_EXIST_ERROR;
    
    StatusCode cond = delete_edge(graph, from, to);

    if (cond == OK)
        return delete_edge(graph, to, from);

    return cond;
}


StatusCode edit_edge(Graph* graph, int from, int to, Type type) {
    if (!graph || !graph->size || from < 0 || from >= graph->size || to < 0 || to >= graph->size || to == from)
        return INVALID_ARGUMENT; // Неправильные исходные данные

    Edge* curr = graph->dudes[from]->edges;

    while (curr != NULL && curr->to != to)
        curr = curr->next;  

    if (!curr)
        return EDGE_NOT_EXIST_ERROR; // Такого элемента нет
    
    if (curr->type == type)
        return NO_CHANGE_ERROR; // Установлен тот же параметр

    curr->type = type;    

    return OK;
}


StatusCode edit_edge_by_names(Graph* graph, const char* from_name, const char* to_name, Type type) {
    if (!graph || !from_name || !to_name)
        return INVALID_ARGUMENT;
    
    int from = search_dude(graph, from_name);
    int to = search_dude(graph, to_name);
    
    if (from == -1 || to == -1)
        return DUDE_NOT_EXIST_ERROR;
    
    StatusCode cond = edit_edge(graph, from, to, type);
    if (cond == OK)
        return edit_edge(graph, to, from, (type + 1) % 2);
    return cond;
}


void print_edges(Graph* graph, int index) {
    Dude* dude = graph->dudes[index];
    Edge* curr_edge = dude->edges;
    
    char* type = NULL;
    char* pronoun = "his";
    if (dude->sex == FEMALE)
        pronoun = "her";

    while (curr_edge) {
        Dude* connected = graph->dudes[curr_edge->to];

        type = "child";
        if (curr_edge->type == PARENT)
            type = "parent";

        printf("\t%s %s %s\n",connected->name, pronoun, type);

        curr_edge = curr_edge->next;
    }
}


void print_dude(Graph* graph, int index) {
    if (!graph || !graph->dudes || index < 0 || index >= graph->size) {
        printf("Invalid paramets\n");
        return;
    }

    Dude* dude = graph->dudes[index];

    printf("[%d]: %s, ", index, dude->name);
    char* sex = "MAN";
    if (dude->sex == FEMALE)
        sex = "WOMAN";
    printf("%s, ", sex);

    if (dude->birth == INT_MIN)
        puts("year of birth unknown, ");
    else
        printf("was born in %d, ", dude->birth);
    
    if (dude->death == INT_MAX)
        puts("ALIVE");
    else
        printf("died in %d\n", dude->death);
    printf("Immediate family:\n");
    print_edges(graph, index);
    printf("\n");
}


void print_graph(Graph* graph) {
    if (!graph) {
        printf("Graph doesn't exist\n");
        return;
    }

    if (!graph->dudes || graph->size == 0) {
        printf("Graph is empty\n");
        return;
    }

    printf("\nYour family tree (number of dudes: %d)\n", graph->size);

    for (int i = 0; i < graph->size; ++i)
        print_dude(graph, i);
}


StatusCode shortest_path(Graph* graph, int from, int to, int* answer) {
    puts("--__--");
    if (!graph || to < 0 || from < 0 || to >= graph->size || from >= graph->size || graph->size == 0)
        return INVALID_ARGUMENT;

    int *dist = calloc(graph->size, sizeof(int));
    if (!dist)
        return MEMORY_ERROR;

    int *color = calloc(graph->size, sizeof(int));
    if (!color) {
        free(dist);
        return MEMORY_ERROR;
    }

    for (int i = 0; i < graph->size; ++i) {
        dist[i] = INT_MAX;
        color[i] = 0;
    }

    dist[from] = 0;
    for (int k = 0; k < graph->size; ++k) {
        int min = -1, mindist = INT_MAX;
        for (int i = 0; i < graph->size; ++i) {
            if (color[i] == 0 && dist[i] < mindist) {
                min = i;
                mindist = dist[i];
            }
        }
        if (min == -1)
            break;

        color[min] = 1;

        Edge* curr_edge = graph->dudes[min]->edges;
        while (curr_edge != NULL) {
                if (dist[min] + 1 < dist[curr_edge->to])
                    dist[curr_edge->to] = dist[min] + 1;
                curr_edge = curr_edge->next;
        }
    }

    *answer = dist[to];
    free(dist);
    free(color);

    return OK;
}


StatusCode shortest_path_by_names(Graph* graph, const char* from_name, const char* to_name, int* answer) {
    if (!graph || !from_name || !to_name)
        return INVALID_ARGUMENT;
    
    int from = search_dude(graph, from_name);
    int to = search_dude(graph, to_name);
    
    if (from == -1 || to == -1)
        return DUDE_NOT_EXIST_ERROR;

    return shortest_path(graph, from, to, answer);
}


static void dfs_male_oldest(Graph* graph, int from, bool* color, int* oldest_birth, int* result) {
    color[from] = true;

    Dude* dude = graph->dudes[from];
    
    if (dude->death == INT_MAX && dude->sex == MALE && dude->birth != INT_MIN) {
        if (dude->birth < *oldest_birth) {
            *result = from;
            *oldest_birth = dude->birth;
        }
    }
    
    Edge* edge = dude->edges;
    while (edge != NULL) {
        if (edge->type == PARENT && !color[edge->to])
            dfs_male_oldest(graph, edge->to, color, oldest_birth, result);
        edge = edge->next;
    }
}


StatusCode init_dfs_male_oldest(Graph* graph, int from, int* answer) {
    int result = -1;

    if (!graph || from < 0 || from >= graph->size || !answer)
        return INVALID_ARGUMENT;
    
    bool* color = (bool*)calloc(graph->size, sizeof(bool));
    if (!color)
        return MEMORY_ERROR;
    
    int oldest_birth = INT_MAX;
    Edge* curr_edge = graph->dudes[from]->edges;
    while (curr_edge) {
        if (curr_edge->type == PARENT && !color[curr_edge->to])
            dfs_male_oldest(graph, curr_edge->to, color, &oldest_birth, &result);
        curr_edge = curr_edge->next;
    }
    
    free(color);
    
    *answer = result;
    return OK;
}


StatusCode dfs_male_oldest_by_names(Graph* graph, const char* from_name, int* answer) {
    if (!graph || !from_name)
        return INVALID_ARGUMENT;
    
    int from = search_dude(graph, from_name);
    
    if (from == -1)
        return DUDE_NOT_EXIST_ERROR;

    return init_dfs_male_oldest(graph, from, answer);
}


DistMatrix* create_dist_matrix(int size) {
    DistMatrix* matrix = (DistMatrix* )malloc(sizeof(DistMatrix));
    if (!matrix)
        return NULL;

    matrix->size = size;
    matrix->data = (int**)malloc(size * sizeof(int*));
    
    if (!matrix->data) {
        free(matrix);
        return NULL;
    }

    for (int i = 0; i < size; ++i) {
        matrix->data[i] = (int*)malloc(size * sizeof(int));
        if (!matrix->data[i]) {
            for (int j = 0; j < i; ++j)
                free(matrix->data[j]);

            free(matrix->data);
            free(matrix);
            return NULL;
        }
        for (int j = 0; j < size; ++j)
            if (j == i)
                matrix->data[i][j] = 0;
            else
                matrix->data[i][j] = INT_MAX;
    }
    return matrix;
}


void free_dist_matrix(DistMatrix* matrix) {
    for (int i = 0; i < matrix->size; ++i)
        free(matrix->data[i]);
    free(matrix->data);
    free(matrix);
    return;
}


void fill_dist_matrix(Graph* graph, DistMatrix* matrix) {
    for (int i = 0; i < graph->size; ++i) {
        Edge* curr_edge = graph->dudes[i]->edges;
        while (curr_edge != NULL) {
            if (curr_edge->type == CHILD)
                matrix->data[i][curr_edge->to] = 1;
            curr_edge = curr_edge->next;
        }
    }

    for (int k = 0; k < graph->size; k++)
        for (int i = 0; i < graph->size; i++)
            for (int j = 0; j < graph->size; j++)
                if (matrix->data[i][k] != INT_MAX &&matrix->data[k][j] != INT_MAX)
                    if (matrix->data[i][j] > matrix->data[i][k] + matrix->data[k][j])
                        matrix->data[i][j] = matrix->data[i][k] + matrix->data[k][j];
}


int power(int base, int exponent) {
    int result = 1;

    for (int i = 0; i < exponent; i++)
        result *= base;

    return result;
}

StatusCode distribute_money(Graph* graph, int from, float summ, DistMatrix* matrix, float* answer) {
    if (!graph || !graph->size || from < 0 || from > graph->size || summ <= 0 || !matrix || !answer)
        return INVALID_ARGUMENT;

    float* spi_mult = (float*)calloc(graph->size, sizeof(float));
    if (!spi_mult)
        return MEMORY_ERROR;

    for (int i = 0; i < graph->size; ++i)
        if (i != from && matrix->data[from][i] < 4 && graph->dudes[i]->death == INT_MAX)
            spi_mult[i] = (float)1 / (float)power(2, matrix->data[from][i] - 1);

    float mult = 0;
    for (int i = 0; i < graph->size; i++)
        mult += spi_mult[i];

    if (mult == 0)
        return DUDE_CANT_BE_FOUND;
    float x = summ / mult;

    for (int i = 0; i < graph->size; ++i)
        answer[i] = spi_mult[i] * x;
    
    free(spi_mult);

    return OK;
}


StatusCode distribute_money_by_name(Graph* graph, const char* name, double summ, float* answer) {
    if (!graph || !graph->size || !name || summ <= 0 || !answer)
        return INVALID_ARGUMENT;
    int from = search_dude(graph, name);
    if (from == -1)
        return DUDE_NOT_EXIST_ERROR;
    
    DistMatrix* matrix = create_dist_matrix(graph->size);
    if (!matrix)
        return MEMORY_ERROR;

    fill_dist_matrix(graph, matrix);

    StatusCode cond = distribute_money(graph, from, summ, matrix, answer);
    free_dist_matrix(matrix);

    return cond;
}


StatusCode export_to_dot(Graph* graph, const char* filename) {
    if (!graph || !filename) return INVALID_ARGUMENT;
    
    FILE* file = fopen(filename, "w");
    if (!file)
        return INVALID_ARGUMENT;
    
    fprintf(file, "digraph FAMILY_GRAPH {\n");
    fprintf(file, "  rankdir=TB;\n");
    fprintf(file, "  node [shape=box, style=filled, fillcolor=lightblue];\n");
    
 
    for (int i = 0; i < graph->size; i++) {
        Dude* dude = graph->dudes[i];
        char* sex_color =  "lightblue";
        if (dude->sex == FEMALE)
            sex_color = "pink";

        fprintf(file, "  \"%d_%s\" [label=\"%s\\n", i, dude->name, dude->name);
        if (dude->birth != INT_MIN)
            fprintf(file, "%d - ", dude->birth);
        else
            fprintf(file, "---- - ");
        if (dude->death != INT_MAX)
            fprintf(file, "%d");
        else
            fprintf(file, "ALIVE\", fillcolor=\"%s\"];\n", sex_color);
    }
    
    for (int i = 0; i < graph->size; i++) {
        Edge* curr_edge = graph->dudes[i]->edges;
        while (curr_edge) {
            fprintf(file, "  \"%d_%s\" -> \"%d_%s\" [arrowhead=none];\n", i, graph->dudes[i]->name, curr_edge->to, graph->dudes[curr_edge->to]->name);
            curr_edge = curr_edge->next;
        }
    }
    
    fprintf(file, "}\n");
    fclose(file);
    return OK;
}


StatusCode generate_image(const char* filename) {
    if (!filename)
        return INVALID_ARGUMENT;
    
    char command[512];
    snprintf(command, sizeof(command), "dot -Tpng %s -o %s.png", filename, filename);
    
    int result = system(command);
    if (result != 0)
        return INVALID_ARGUMENT;
    
    return OK;
}