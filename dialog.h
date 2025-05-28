#ifndef DIALOG_H
#define DIALOG_H


#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "input.h"

int dialog_create_graph(Graph** graph);
int dialog_search_dude(Graph** graph);
int dialog_add_dude(Graph** graph);
int dialog_delete_dude(Graph** graph);
int dialog_edit_dude(Graph** graph);
int dialog_print_graph(Graph** graph);
int dialog_add_edge(Graph** graph);
int dialog_delete_edge(Graph** graph);
int dialog_edit_edge(Graph** graph);
int dialog_shortest_path(Graph** graph);
int dialog_dfs_male_oldest(Graph** graph);
int dialog_distribute_money(Graph** graph);

int menu();


#endif