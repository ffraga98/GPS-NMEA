#include <stdio.h>
#include "types.h"
#include "error.h"

status_t print_error(FILE * fo, status_t status)
{
	if (fo==NULL)
		return ERROR_NULL_POINTER;

	fprintf(fo,"%s\n",errors_dictionary[status]);
		return OK;
}