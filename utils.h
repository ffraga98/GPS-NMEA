#ifndef UTILS__H
#define UTILS__H

#include <stdio.h>
#include <stdlib.h>
#include "types.h"

#define INIT_CHOP 50
#define CHOP_SIZE 10

status_t load_text_file(char ** str, FILE * fi);
status_t split(const string s,const char del, size_t *l, string** campos);
status_t destroy_strings(string ** str, size_t largo); 
status_t strdupl(string * str,const string s); 
string read_line(FILE * fi,bool_t * eof);
string strtoken(char ** str,char delimitter);

#endif