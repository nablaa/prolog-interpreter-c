#include <stdio.h>
#include <stdlib.h>
#include "tests.h"
#include "test_pltoken.h"

int main()
{
	WITH_COLOR(GREEN, printf("Tests begin\n"));
	test_pltoken();

	WITH_COLOR(GREEN, printf("Tests end\n"));
	return EXIT_SUCCESS;
}

