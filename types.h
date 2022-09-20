#ifndef TYPES__H
#define TYPES__H

#include <stdio.h>

typedef enum{
		OK=0,
		ERROR_NULL_POINTER=1,
		ERROR_MEMORY=2,
		ERROR_INVALID_FORMAT=3,
		ERROR_OPENING_INPUT_FILE=4,
		ERROR_OPENING_OUTPUT_FILE=5,
		ERROR_PROGRAM_INVOCATION=6,
		ERROR_OPENING_HEADER_FILE=7, 
		ERROR_OPENING_FOOTER_FILE=8 
	}status_t;

typedef enum{
		TRUE,
		FALSE
	}bool_t;

typedef enum{
		DOC_TYPE_CSV=0,
		DOC_TYPE_KML=1
	} doc_type_t;

typedef char* string;

typedef status_t (*destructor_t)(void*);
typedef status_t (*printer_t)(void * str, const char delimitter, FILE * stream);
typedef int (*comparator_t)(const void * r1,const void * r2);

#endif