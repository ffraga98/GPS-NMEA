#ifndef CONFIG__H
#define CONFIG__H

#include <stdio.h>
#include "types.h"

typedef struct{
		doc_type_t doc_type;
		string output_file; 
		string input_file;
	} config_t;

config_t config;

#endif