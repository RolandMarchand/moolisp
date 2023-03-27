#include "repl.h"

int main() {
	char* input;
	while ((input = readline("> ")) != NULL) {
		add_history(input);
		free(input);
	}
}
