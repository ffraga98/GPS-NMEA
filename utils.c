#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "utils.h"

status_t destroy_strings(string ** str ,size_t largo)
{
	size_t i;

	if(str==NULL)
		return ERROR_NULL_POINTER;

	for(i=0; i<largo; i++)
	{
		free((*str)[i]);
		(*str)[i]=NULL;
	}
	free(*str);
	*str=NULL;
	return OK;
}

status_t strdupl(string * str_aux,const string s)
{
	size_t i;

	if(s==NULL || str_aux==NULL)
		return ERROR_NULL_POINTER;

	if(((*str_aux)=malloc(sizeof(char)*(strlen(s)+1)))==NULL)
		return ERROR_MEMORY;
	
	for(i=0; ((*str_aux)[i]=s[i]) ;i++);
	return OK;
}

/*Recorta y retorna el primer campo separado por un delimitador y devuelve la cadena restante por interfaz. Devuelve NULL si no tiene memoria. NO PASAR PUNTERO NULL*/

string my_strtoken(string * str,char delimitter)
{
	size_t i,len_str,field_len,j;
	string field;

	if(str==NULL)
		return NULL;

	len_str=strlen(*str);

	for(i=0; (*str)[i]!='\0' ;i++)
		if((*str)[i]==delimitter)
		{
			(*str)[i]='\0';
			break;
		}


	field_len=strlen(*str)+1;
	if((field=(char *)malloc(sizeof(char)*field_len))==NULL)
		return NULL;

	for(i=0;(*str)[i]!='\0';i++)
		field[i]=(*str)[i];
	
	field[i]='\0';
	for(i=field_len,j=0; i<len_str; i++,j++)
		(*str)[j]=(*str)[i];
	
	(*str)[j]='\0';
	return field;
}

status_t split(const string s, const char del, size_t * cantidad_campos, string ** campos)
{
	status_t status;
	string st;
	size_t i;

	if(s==NULL || cantidad_campos==NULL || campos==NULL)
		return ERROR_NULL_POINTER;

	if((status=strdupl(&st,s))!=OK)
	{
		*cantidad_campos=0;
		return status;
	}
	for(i=0, *cantidad_campos=0; st[i]; i++)
	{
		if(st[i]==del)
		(*cantidad_campos)++;	
	}
	(*cantidad_campos)++;

	if(((*campos)=(string*)malloc(sizeof(string)*(*cantidad_campos)))==NULL)
	{
		free(st);
		*cantidad_campos=0;
		return ERROR_MEMORY;
	}
    for (i=0;i<*cantidad_campos;i++)
    {
		if(((*campos)[i]=my_strtoken(&st,del))==NULL)
		{
			free(st);
			destroy_strings(campos,*cantidad_campos);
			return ERROR_MEMORY;
		}
	}
	free(st);
	return OK;
}
/*NO PASAR PUNTERO NULO*/
string read_line(FILE * fi,bool_t * eof)
{
	size_t alloc_size,len;
	string aux;
	string str;
	int c;

	if((str=(char*)malloc(INIT_CHOP*sizeof(char)))==NULL)
		return NULL;

	alloc_size=INIT_CHOP;
	len=0;
	c=fgetc(fi);
	
	while(c!=EOF)
	{
		if(c=='\n'||c==EOF)
			break;
		str[len]=c;
		len++;

		if(len==alloc_size-1)
		{
			if((aux=realloc(str,(alloc_size+CHOP_SIZE)*sizeof(char)))==NULL)
			{
				free(str);
				str=NULL;
				return NULL;
			}
			alloc_size+=CHOP_SIZE;
			str=aux;
		}
		c=fgetc(fi);
	}
	if(c==EOF)
		*eof=TRUE;

	str[len]='\0';
	return str;
}

status_t load_text_file(string * str, FILE * fi)
{
	size_t alloc_size,l;
	string str_aux;
	int c;

	if(str==NULL||fi==NULL)
		return ERROR_NULL_POINTER;

	if((*str=malloc(INIT_CHOP*sizeof(char)))==NULL)
		return ERROR_MEMORY;

	alloc_size=INIT_CHOP;
	l=0;
	c=fgetc(fi);
	while(c!=EOF)
	{
		(*str)[l]=c;
		l++;
		if(l==alloc_size-1)
		{
			if((str_aux=realloc(*str,(alloc_size+CHOP_SIZE)*sizeof(char)))==NULL)
			{
				free(*str);
				*str=NULL;
				return ERROR_MEMORY;
			}
			alloc_size+=CHOP_SIZE;
			*str=str_aux;
		}
		c=fgetc(fi); 
	}
	(*str)[l]='\0';
	return OK;
}