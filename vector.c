#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "vector.h"
#include "utils.h"

/*Crea un vector vacío*/
status_t ADT_Vector_new(ADT_Vector_t ** pv)
{
	if(pv==NULL)
		return ERROR_NULL_POINTER;
	
	if(((*pv)=(ADT_Vector_t *)malloc(sizeof(ADT_Vector_t)))==NULL)
		return ERROR_MEMORY;

	(*pv)->size=0;
	(*pv)->used_size=0;
	(*pv)->header=NULL;
	(*pv)->footer=NULL;
	(*pv)->tag=NULL;
	(*pv)->elements=NULL;
	return OK;
}
/*Recibe un vector con el correspondiente destructor de sus elementos. OJO, EL FOOTER Y EL HEADER DEBE SER MEMORIA DINAMICA*/
status_t ADT_Vector_delete(ADT_Vector_t ** pvector, destructor_t pf)
{
	size_t i;
	status_t st;
	if(pvector==NULL || pf==NULL)
		return ERROR_NULL_POINTER;

	for(i=0; i< (*pvector)-> size; i++)
	{
		if((st=(*pf)((*pvector)->elements[i]))!=OK)
			return st; 

		(*pvector)->elements[i]=NULL;
	} 	
	free((*pvector)->header);
	free((*pvector)->footer);
	free((*pvector)->elements);
	(*pvector)->size=0;
	(*pvector)->used_size=0;
	return OK;
}
/*TRUE si está vacío, de lo contrario FALSE*/
bool_t ADT_Vector_is_empty(ADT_Vector_t * pvector)
{
	return (!(pvector->used_size))? TRUE:FALSE;
}
/*Agrega por interfaz un elemento al vector, se necesita pasarle por argumento como borrar el elemento, ya que no poseemos información al respecto*/
status_t ADT_Vector_append_element(ADT_Vector_t ** pvector, void * element, status_t (*delete_vector)(ADT_Vector_t**,destructor_t), destructor_t destroy_elements)
{
	size_t l=0;
	void * aux;

	if(pvector==NULL||element==NULL||delete_vector==NULL||destroy_elements==NULL)
		return ERROR_NULL_POINTER;

	l=((*pvector)->used_size);
	if(l==(*pvector)->size)
	{
		if((aux=(void **)realloc((*pvector)->elements,(((*pvector)->size) + ADT_VECTOR_CHOP_SIZE)*sizeof(void *)))==NULL)
		{
			(*delete_vector)(pvector, destroy_elements);
			return ERROR_MEMORY;
		}
		(*pvector)->elements=aux;
		(*pvector)->size+=ADT_VECTOR_CHOP_SIZE;
	}
	(*pvector)->elements[l]=element;
	(*pvector)->used_size++;
	return OK;
}
/*Exporta en CSV un vector, necesita saber como imprimir cada elemento.*/
status_t ADT_Vector_export_as_CSV(const ADT_Vector_t * vector,const char delimitter, FILE * fo, printer_t printer)
{	
	status_t st;
	size_t i;

	if(vector==NULL||printer==NULL)
		return ERROR_NULL_POINTER;

	for(i=0;i<(vector->used_size);i++)
	{
		if((st=(*printer)(vector->elements[i],delimitter,fo))!=OK)
			return st;
	}
	return OK;
}
/*Inicializa el tag de un archivo XML con la cadena recibida en el argumento*/
status_t ADT_Vector_set_XML_tag(ADT_Vector_t ** pv,const string str)
{
	status_t status;
	if(str==NULL||pv==NULL)
		return ERROR_NULL_POINTER;

	if((status=strdupl(&((*pv)->tag),str))!=OK)		
		return status;

	return OK;
}

status_t ADT_Vector_set_header(ADT_Vector_t ** pv,FILE * stream)
{
	status_t status;
	if(stream==NULL||pv==NULL)
		return ERROR_NULL_POINTER;

	if((status=load_text_file(&((*pv)->header),stream))!=OK)
		return status;

	return OK;
}


status_t ADT_Vector_set_footer(ADT_Vector_t ** pv,FILE * stream)
{
	status_t status;
	if(stream==NULL||pv==NULL)
		return ERROR_NULL_POINTER;

	if((status=load_text_file(&((*pv)->footer),stream))!=OK)
			return status;
		
	return OK;
}
/*Exporta el vector en formato XML. Necesita saber como imprimir los elementos y el delimitador. */
status_t ADT_Vector_export_as_XML(const ADT_Vector_t * pvector,const char delimitter,FILE * fo, printer_t printer)
{
	size_t i;
	status_t st;

	if(pvector==NULL||fo==NULL)
		return ERROR_NULL_POINTER;

	fprintf(fo,"%s",pvector->header);
	fprintf(fo,"\t\t\t\t%c%s%c",'<', pvector->tag,'>');

	for(i=0;i< pvector->used_size;i++)
	{
		if((st=(*printer)(pvector->elements[i],delimitter,fo))!=OK)		
			return st;
	}
	fprintf(fo,"%s%s%c\n","</", pvector->tag,'>');
	fprintf(fo,"%s",pvector->footer);
	return OK;
}
/*Intercambia la posición dos elementos de un vector. Recibe el vector, y ambas posiciones. Retorna si falló o no.*/ 
status_t ADT_Vector_swap_elements(ADT_Vector_t ** vector,size_t position_1,size_t position_2)
{
	void * aux;
	if(vector==NULL)
		return ERROR_NULL_POINTER;

	aux=((*vector)->elements[position_1]);
	((*vector)->elements[position_1])=((*vector)->elements[position_2]);
	((*vector)->elements[position_2])=aux;
	return OK;
}
/*Método de ordenamiento selectivo de los elementos del vector. Necesita el vector y el criterio de comparación*/
status_t ADT_Vector_sort(ADT_Vector_t ** vector, comparator_t cmp)
{	
	status_t status;
	size_t i,j,min,l;

	if(vector==NULL||cmp==NULL)
		return ERROR_NULL_POINTER;

	l=(*vector)->used_size;  
	for(i=0; i<(l-1); i++)
	{
		min=i;
		for(j=(i+1);j<l;j++)
		{
			if(((*cmp)((*vector)->elements[min],(*vector)->elements[j]))>0)
				min=j;

		}
		if((status=ADT_Vector_swap_elements(vector,i,min))!=OK)
			return status;	
	}
	return OK;
}