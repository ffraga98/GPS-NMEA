#ifndef ERROR__H
#define ERROR__H

#include <stdio.h>
#include "types.h"

#define MAX_ERRORS 9

static string errors_dictionary[MAX_ERRORS]={
	"OK",
	"Null pointer",
	"Error memory",
	"Invalid format",
	"Input file",
	"Output file",
	"Program invocation",
	"Header file",
	"Footer file"
	};

status_t print_error(FILE * fo, status_t status);

#endif