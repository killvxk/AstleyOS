#include <commands.h>
#include <stdio.h>
#include <console.h>

COMMAND_FUNCTION(clear);
COMMAND_HELP(clear);

COMMAND_FUNCTION(clear) {
	clear();

	return OK;
}

COMMAND_HELP(clear) {
	if(argc >= 1) {
		printf("<clear> Error: too many arguments.\n");

		return ERROR;
	}

	printf("Clears the screen. Ignores arguments.\n");
	printf("Usage: 'clear'\n");

	return OK;
}
