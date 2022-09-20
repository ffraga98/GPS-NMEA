#ifndef MAIN__H
#define MAIN__H

#include <stdio.h>
#include "types.h"
#include "config.h"

#define MAX_ARGUMENTS 6
#define MAX_FILE_FORMATS 2
#define FORMAT_TOKEN "-fmt"
#define OUTPUT_FILE_TOKEN "-out"
#define CMD_ARG_OUTPUT_FILE 1
#define CMD_ARG_INPUT_FILE 2
#define CMD_ARG_DOC_TYPE_VALUE 1
#define FIXED_ARGUMENTS 3 
#define DICTIONARY_DOC_TYPE_CSV "csv"
#define DICTIONARY_DOC_TYPE_KML "kml"

status_t validate_arguments(int argc, string argv[],config_t *);
status_t get_doct_type_file(const string typed_format, doc_type_t * doc_format);

#endif