#include "input.h"


int int_input(int* value, const char* input_prompt){
	int input = 2;
	char check;
	do {
		if (input == 1 && check == '\n')
			return 0;
		if (input != 2) {
			scanf("%*[^\n]");
			printf("ValueError: Invalid literal for int\n"\
					"Lets try again...\n");
		}

		printf("%s", input_prompt);
		input = scanf("%d", value);
		scanf("%c", &check);
	} while (input != -1);

	return 1;
}


int size_t_input(size_t* value, const char* input_prompt){
	int input = 3;
	long int value_2 = 0;
	char check;

	do {
		if (input == 1 && check == '\n' && value_2 >= 0) {
			*value = value_2;
			return 0;
		}
		else {
			if (value_2 < 0) {
				printf("ValueError: invalid literal for size_t\n"\
					"Lets try again...\n");
				if (check != '\n')
					scanf("%*[^\n]");
			}
			else {
				if (input != 3) {
					printf("ValueError: Invalid literal for size_t\n"\
						"Lets try again...\n");
					scanf("%*[^\n]");
				}
			}
		}

		printf("%s", input_prompt);
		input = scanf("%ld", &value_2);
		scanf("%c", &check);
	} while(input != -1);

	return 1;
}


char *str_input(const char* prompt) {
	printf("%s", prompt);

	char buf[81] = {0};
	char *res = NULL;
	int len = 0;
	int n = 0;

	do {
		n = scanf("%80[^\n]", buf);
		if (n < 0) {
			if (!res)
			return NULL;
		}
		else
			if (n > 0) {
				int chunk_len = strlen(buf);
				int str_len = len + chunk_len;
				res = realloc(res, str_len + 1);
				memcpy(res + len, buf, chunk_len);
				len = str_len;
			}
			else
				scanf("%*c");
	} while (n > 0);

	if (len > 0)
		res[len] = '\0';
	else
		res = calloc(1, sizeof(char));
	return res;
}