#include <stdio.h>
#include <stdlib.h>
#include "plinterpreter.h"

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("Usage: %s <program.pl>\n", argv[0]);
		return EXIT_FAILURE;
	}

	FILE *database = fopen(argv[1], "r");
	if (!database) {
		printf("Could not open file: %s\n", argv[1]);
		return EXIT_FAILURE;
	}

	PLHandleInput(database, stdin);

	fclose(database);
	return EXIT_SUCCESS;
}

