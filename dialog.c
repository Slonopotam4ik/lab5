#include "dialog.h"


int dialog_create_graph(Graph** graph) {
	if (*graph)
		free_graph(*graph);
	*graph = create_graph();
	if (!graph)
		return 1;
	return 0;
}


int dialog_search_dude(Graph** graph) {
	if (!(*graph)->dudes || !(*graph)) {
		puts("Graph is empty");
		return 0;
	}

	char* name = str_input("Enter the guy name: ");
	if (!name)
		return -1;

	int index_dude = search_dude(*graph, name);
	if (index_dude == -1)
		puts("This guy is not from this family(");
	else
		print_dude(*graph, index_dude);
	return 0;
}


int dialog_add_dude(Graph** graph) {
	int birth = 0;
	int death = 0;
	int cond = 0;
	Sex sex;
	char* name = str_input("Enter the guy name: ");
	if (!name)
		return -1;

	if (int_input(&birth, "Enter his year of birth, if unknown enter any number greater than 2025: ")) {
		free(name);
		return -1;
	}

	if (int_input(&death, "Enter his year of death, if he is alive enter any year greater than 2025: ")){
		free(name);
		return -1;
	}

	if (birth > 2025)
		birth = INT_MIN;
	if (death > 2025)
		death = INT_MAX;

	if (int_input(&cond, "Enter an even number if it is male and an odd number if it is female: ")){
		free(name);
		return -1;
	}
	
	sex = MALE;
	if (cond % 2)
		sex = FEMALE;
	
	switch (add_dude(*graph, name, sex, birth, death)) {
		case INVALID_ARGUMENT:
			puts("Error: Invalid arguments");
			break;
		case DUDE_DUPLICATE_ERROR:
			puts("Error: Non-unique name");
			break;
		case MEMORY_ERROR:
			puts("Error: Memory Error");
			free(name);
			return -1;
		case OK:
			puts("Succes");
			break;
	}

	free(name);
	return 0;
}


int dialog_delete_dude(Graph** graph) {
	char* name = str_input("Enter the guy name: ");
	if (!name)
		return -1;

	switch (delete_dude_by_name(*graph, name)) {
			case INVALID_ARGUMENT:
				puts("Error: Invalid arguments");
				break;
			case DUDE_NOT_EXIST_ERROR:
				puts("Error: Uncorrect name");
				break;
			case MEMORY_ERROR:
				puts("Error: Memory Error");
				return 1;
			case OK:
				puts("Succes");
				break;
	}
	free(name);
	return 0;
}


int dialog_edit_dude(Graph** graph) {
	int birth = 0;
	int death = 0;
	int cond = 0;
	Sex sex;
	char* old_name = str_input("Enter the guy name: ");
	if (!old_name)
		return -1;

	char* new_name = str_input("Enter the NEW guy name: ");
	if (!new_name) {
		free(old_name);
		return -1;
	}

	if (int_input(&birth, "Enter his NEW year of birth, if unknown enter any number greater than 2025: ")) {
		free(old_name);
		free(new_name);
		return -1;
	}

	if (int_input(&death, "Enter his NEW year of death, if he is alive enter any year greater than 2025: ")){
		free(old_name);
		free(new_name);
		return -1;
	}

	if (birth > 2025)
		birth = INT_MIN;
	if (death > 2025)
		death = INT_MAX;

	if (int_input(&cond, "Enter an even number if it is male and an odd number if it is female: ")){
		free(old_name);
		free(new_name);
		return -1;
	}
	
	sex = MALE;
	if (cond % 2)
		sex = FEMALE;


	switch (edit_dude_by_name(*graph, old_name, new_name, sex, birth, death)) {
			case INVALID_ARGUMENT:
				puts("Error: Invalid arguments");
				break;
			case DUDE_NOT_EXIST_ERROR:
				puts("Error: Uncorrect name");
				break;
			case MEMORY_ERROR:
				puts("Error: Memory Error");
				free(old_name);
				free(new_name);
				return 1;
			case OK:
				puts("Succes");
				break;
			case NAME_DUPLICATE_ERROR:
				puts("Error: New name is already taken");
				break;
			case NO_CHANGE_ERROR:
				puts("Error: You didn't change anything");
				break;
	}

	free(old_name);
	free(new_name);
	return 0;
}


int dialog_print_graph(Graph** graph) {
	print_graph(*graph);

	return 0;
}


int dialog_add_edge(Graph** graph) {
	int cond = 0;
	Type type = PARENT;
	char* from_name = str_input("Enter the first guy name: ");
	if (!from_name)
		return -1;

	char* to_name = str_input("Enter the second guy name: ");
	if (!to_name) {
		free(from_name);
		return -1;
	}

	printf("Enter an even number if %s is %s's child and an odd number if %s is %s's parent: ", from_name, to_name, from_name, to_name);

	if (int_input(&cond, "")){
		free(to_name);
		free(from_name);
		return -1;
	}
	
	if (cond % 2)
		type = CHILD;

	switch (add_edge_by_names(*graph, from_name, to_name, type)) {
			case INVALID_ARGUMENT:
				puts("Error: Invalid arguments");
				break;
			case DUDE_NOT_EXIST_ERROR:
				puts("Error: Uncorrect name");
				break;
			case MEMORY_ERROR:
				puts("Error: Memory Error");
				free(to_name);
				free(from_name);
				return 1;
			case OK:
				puts("Succes");
				break;
			case EDGE_DUPLICATE_ERROR:
				puts("Error: This edge alredy exist");
				break;
	}

	free(to_name);
	free(from_name);
	return 0;
}


int dialog_delete_edge(Graph** graph) {
	char* from_name = str_input("Enter the first guy name: ");
	if (!from_name)
		return -1;

	char* to_name = str_input("Enter the second guy name: ");
	if (!to_name) {
		free(from_name);
		return -1;
	}

	switch (delete_edge_by_names(*graph, from_name, to_name)) {
			case INVALID_ARGUMENT:
				puts("Error: Invalid arguments");
				break;
			case DUDE_NOT_EXIST_ERROR:
				puts("Error: Uncorrect name");
				break;
			case EDGE_NOT_EXIST_ERROR:
				puts("Error: Uncorrect edge");
				break;
			case OK:
				puts("Succes");
				break;
	}
	free(to_name);
	free(from_name);
	return 0;
}


int dialog_edit_edge(Graph** graph) {
	int cond = 0;
	Type type = PARENT;
	char* from_name = str_input("Enter the first guy name: ");
	if (!from_name)
		return -1;

	char* to_name = str_input("Enter the second guy name: ");
	if (!to_name) {
		free(from_name);
		return -1;
	}

	printf("Enter an even number if %s is %s's child and an odd number if %s is %s's parent: ", from_name, to_name, from_name, to_name);

	if (int_input(&cond, "")){
		free(to_name);
		free(from_name);
		return -1;
	}
	
	if (cond % 2)
		type = CHILD;

	switch (edit_edge_by_names(*graph, from_name, to_name, type)) {
			case INVALID_ARGUMENT:
				puts("Error: Invalid arguments");
				break;
			case DUDE_NOT_EXIST_ERROR:
				puts("Error: Uncorrect name");
				break;
			case EDGE_NOT_EXIST_ERROR:
				puts("Error: Uncorrect edge");
				break;
			case OK:
				puts("Succes");
				break;
			case NO_CHANGE_ERROR:
				puts("Error: You didn't change anything");
				break;
	}

	free(to_name);
	free(from_name);
	return 0;
}


int dialog_shortest_path(Graph** graph) {
	char* from_name = str_input("Enter the first guy name: ");
	if (!from_name)
		return -1;

	char* to_name = str_input("Enter the second guy name: ");
	if (!to_name) {
		free(from_name);
		return -1;
	}

	int answer = 0;

	switch (shortest_path_by_names(*graph, from_name, to_name, &answer)) {
			case INVALID_ARGUMENT:
				puts("Error: Invalid arguments");
				break;
			case DUDE_NOT_EXIST_ERROR:
				puts("Error: Uncorrect name");
				break;
			case OK:
				puts("Succes");
				if (answer != INT_MAX)
					printf("Degree of kinship: %d\n", answer - 1);
				else
					puts("This two dudes re not related");
				break;
	}
	free(to_name);
	free(from_name);
	return 0;
}


int dialog_dfs_male_oldest(Graph** graph) {
	char* from_name = str_input("Enter the guy name: ");
	if (!from_name)
		return -1;

	int answer = 0;

	switch (dfs_male_oldest_by_names(*graph, from_name, &answer)) {
			case INVALID_ARGUMENT:
				puts("Error: Invalid arguments");
				break;
			case DUDE_NOT_EXIST_ERROR:
				puts("Error: Uncorrect name");
				break;
			case OK:
				puts("Succes");
				if (answer != -1) {
					printf("%s's oldest male alive ancestor is:\n", from_name);
					print_dude(*graph, answer);
				}
				else
					printf("%s doesn't have oldest male alive ancestor(\n");
				break;
	}
	free(from_name);
	return 0;
}


int dialog_distribute_money(Graph** graph) {
	char* from_name = str_input("Enter the guy name: ");
	if (!from_name)
		return -1;
	float* answer = calloc((*graph)->size, sizeof(float));
	if (!answer)
		return 1;
	
	int summ_int = 0;
	if (int_input(&summ_int,  "Enter the amount to be divided") == 1)
        return -1;
	
	float summ = (float)(summ_int);

	switch (distribute_money_by_name(*graph, from_name, summ, answer)) {
			case INVALID_ARGUMENT:
				puts("Error: Invalid arguments");
				break;
			case DUDE_NOT_EXIST_ERROR:
				puts("Error: Uncorrect name");
				break;
			case OK:
				puts("Succes");
				break;
	}

	for (int i = 0; i < (*graph)->size; ++i)
		if (answer[i])
			printf("%s will recive %.2f rubles", (*graph)->dudes[i], answer[i]);

	free(from_name);
	return 0;
}


int dialog_to_png(Graph** graph) {
	char* filename = str_input("Enter the .png file name: ");
	if (!filename)
		return -1;

	if (export_to_dot(*graph, filename) != OK) {
		puts("Invalid filename");
		free(filename);
		return 0;
	}
	if (generate_image(filename) != OK) {
		puts("Invalid  filename");
		free(filename);
		return 0;
	}
	puts("Succes");
	printf("File %s.png created", filename);
	free(filename);
	return 0;
}


int menu() {
        int cond = 13, choice = 89;

        //printf("────────────────────────────┤LAB4├────────────────────────────\n");
		printf("============================ LAB5 ============================\n");
        do{
                if (!cond)
                        printf("============================ LAB5 ============================\n"\
                               "ChoiceError: Invalid choice\n"\
                               "let's try again...\n"\
                               "============================ LAB5 ============================\n");
						// printf("────────────────────────────┤LAB4├────────────────────────────\n"\
                        //        "ChoiceError: Invalid choice\n"\
                        //        "let's try again...\n"\
                        //        "────────────────────────────┤LAB4├────────────────────────────\n");
                if (int_input(&choice,  "\t(00) Create graph\n"\
                                        "\t(01) Search dude\n"\
                                        "\t(02) Add dude\n"\
                                        "\t(03) Delete dude\n"\
                                        "\t(04) Edit Information about dude\n"\
                                        "\t(05) Print graph\n"\
                                        "\t(06) Add edge\n"\
                                        "\t(07) Delete edge\n"\
                                        "\t(08) Edit Information about dude\n"\
                                        "\t(09) Degree of kinship \n"\
										"\t(10) Get information about the oldest alive man\n"\
                                        "\t(11) Distribute money \n"\
										"\t(12) To PNG\n"\
                                        "Input selected option: ") == 1)
                        return -1;
                cond = (choice >= 0 && choice < 13);
        }while (!cond);

        printf("============================ LAB5 ============================\n");
        return choice;
}