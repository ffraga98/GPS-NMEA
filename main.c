#include <stdio.h>
#include <string.h>
#include "types.h"
#include "config.h"
#include "gps_processor.h"
#include "error.h"
#include "main.h"
extern config_t config;

/*Obtiene el formato elegido por el usuario*/
status_t get_doc_type_file(const string typed_format, doc_type_t * doc_type)
{
	static string doc_type_dictionary[MAX_FILE_FORMATS]={DICTIONARY_DOC_TYPE_CSV,DICTIONARY_DOC_TYPE_KML};
	size_t i;

	if(typed_format==NULL||doc_type==NULL)
		return ERROR_NULL_POINTER;

	for (i=0;i<MAX_FILE_FORMATS;i++){
		if(!strcmp(typed_format,doc_type_dictionary[i])){
			*doc_type=i;
			return OK;
		}
	}
	return ERROR_INVALID_FORMAT;
}
/*El llamado de las banderas (-fmt o -out) no tienen un orden, pero siempre que se llama al archivo de salida, como siguiente argumento debe ser el archivo de entrada*/
status_t validate_arguments(int argc, string argv[], config_t * config)
{
	size_t i,arguments_to_parse;
	status_t status;

	if (argv==NULL || config==NULL)
		return ERROR_NULL_POINTER;

	if(argc!=MAX_ARGUMENTS)
		return ERROR_PROGRAM_INVOCATION;

	arguments_to_parse=MAX_ARGUMENTS-FIXED_ARGUMENTS;
	for(i=1;i<MAX_ARGUMENTS;i++)
	{
		if(!strcmp(argv[i],FORMAT_TOKEN))
		{
			if((status=get_doc_type_file(argv[i+CMD_ARG_DOC_TYPE_VALUE],&(config->doc_type)))!=OK)
				return status;

			arguments_to_parse-=1;
		}
		if(!strcmp(argv[i],OUTPUT_FILE_TOKEN))
		{
			config->output_file=argv[i+CMD_ARG_OUTPUT_FILE];
			config->input_file=argv[i+CMD_ARG_INPUT_FILE];
			arguments_to_parse-=2;
		}
	}
	if(arguments_to_parse!=0)
		return ERROR_PROGRAM_INVOCATION;

	return OK;
}

int main (int argc, char * argv[])
{
	FILE * fi,* fo;
	status_t status;
	ADT_Vector_t records_vector;


	if((status=validate_arguments(argc,argv,&config))!=OK)
	{
		print_error(stderr,status);
		return status;
	}
	if((fo=fopen((config.output_file),"wt"))==NULL)
	{	
		print_error(stderr,ERROR_OPENING_OUTPUT_FILE);
		return ERROR_OPENING_OUTPUT_FILE;
	}
	if((fi=fopen((config.input_file),"rt"))==NULL)
	{
		fclose(fo);
		print_error(stderr,ERROR_OPENING_INPUT_FILE);
		return ERROR_OPENING_INPUT_FILE;
	}
	if((status=process_gps_data(&records_vector,fi,fo))!=OK)
	{
		fclose(fi);
		fclose(fo);
		print_error(stderr,status);
		return status;
	}
	if(fclose(fo)==EOF)
	{
		fclose(fi);
		print_error(stderr,ERROR_MEMORY);
		return ERROR_MEMORY;
	}
	fclose(fi);
	return OK;
}