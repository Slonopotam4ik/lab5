#include "graph.h"
#include "dialog.h"


int main() {
	Graph* graph = NULL;

	int cond = 0;
	int choice = 98;

	do{
		choice = menu();
		if (choice == -1) {
			cond = -1;
			continue;
		}

		if (!graph && choice != 0){
			printf("Error: Graph doesnt exist\n");
			continue;
		}
		switch (choice) {
			case 0:
				cond = dialog_create_graph(&graph);
				break;
			case 1:
				cond = dialog_search_dude(&graph);
				break;
			case 2:
				cond = dialog_add_dude(&graph);
				break;
			case 3:
				cond = dialog_delete_dude(&graph);
				break;
			case 4:
				cond = dialog_edit_dude(&graph);
				break;
			case 5:
				cond = dialog_print_graph(&graph);
				break;
			case 6:
				cond = dialog_add_edge(&graph);
				break;
			case 7:
				cond = dialog_delete_edge(&graph);
				break;
			case 8:
				cond = dialog_edit_edge(&graph);
				break;
			case 9:
				cond = dialog_shortest_path(&graph);
				break;
			case 10:
				cond = dialog_dfs_male_oldest(&graph);
				break;
			case 11:
				cond = dialog_distribute_money(&graph);
				break;
			case 12:
				cond = dialog_shortest_path(&graph);
				break;
		}
	} while (cond != -1 && cond != 1);


	if (cond == 1)
		printf("MemoryError: Not enough memory\n");
	puts("\n┌────────────────────────────────────────────────────────────┐");
	puts("│              ┌─ ╷ ╷ ┌─╮   ┌─┐ ┌─   ┌─ ┬ ╷  ┌─              │");
	puts("│              ├─ │╲│ │ │   │ │ ├─   ├─ │ │  ├─              │");
	puts("│              └─ ╵ ╵ └─╯   └─┘ ╵    ╵  ┴ └─ └─              │");
	puts("└────────────────────────────────────────────────────────────┘");
	free_graph(graph);
	return 0;
}