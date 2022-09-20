#include <stdio.h>
#include <time.h>
#include "date_support.h"
#include "types.h"

status_t get_system_calender_time(time_t * seconds) /*Debe decibir un time_t!!*/
{
 	if(seconds==NULL)
 		return ERROR_NULL_POINTER;

    *seconds=time(NULL); 
	return OK;
}

status_t print_date_YYYYMMDD(const time_t seconds, FILE * stream)
{	
	struct tm * date;

 	if(stream==NULL)
 		return ERROR_NULL_POINTER;

 	date=localtime(&seconds);
	fprintf(stream,"%d%02d%02d",date->tm_year +1900,date->tm_mon +1,date->tm_mday);
	return OK;
}