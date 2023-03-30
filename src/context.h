#pragma once

#include "common.h"

#define CONTEXT_ERROR_EXPECTED_FILE "expected source file"
#define CONTEXT_ERROR_UNKNOWN_ARG "unknown argument"

struct context {
	char **source;
	char *error;
	char *error_arg;
	int source_count;
	bool repl;
	bool license;
	bool version;
	bool verbose; /* unused */
};

struct context get_context(int argc, char **argv);
