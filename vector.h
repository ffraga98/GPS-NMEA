#ifndef VECTOR__H
#define VECTOR__H

#include <stdio.h>
#include "types.h"
#define ADT_VECTOR_CHOP_SIZE 10

typedef struct{
	size_t size;
	size_t used_size;
	void ** elements;
	string tag;
	string footer;
	string header;
}ADT_Vector_t;

status_t ADT_Vector_new(ADT_Vector_t **);
status_t ADT_Vector_delete(ADT_Vector_t **, destructor_t);
status_t ADT_Vector_append_element(ADT_Vector_t **, void *, status_t(*pf)(ADT_Vector_t**,destructor_t), destructor_t);
status_t ADT_Vector_export_as_CSV(const ADT_Vector_t * vector,const char delimitter,FILE * fo,printer_t printer);
status_t ADT_Vector_export_as_XML(const ADT_Vector_t * pvector,const char delimitter,FILE * fo, printer_t printer);
status_t ADT_Vector_swap_elements(ADT_Vector_t ** vector,size_t position_1,size_t position_2);
status_t ADT_Vector_sort(ADT_Vector_t ** vector, comparator_t cmp);
status_t ADT_Vector_set_XML_tag(ADT_Vector_t ** pv, const string str);
status_t ADT_Vector_set_footer(ADT_Vector_t ** pv,FILE * stream);
status_t ADT_Vector_set_header(ADT_Vector_t ** pv,FILE * stream);
bool_t ADT_Vector_is_empty(ADT_Vector_t * pvector);

#endif