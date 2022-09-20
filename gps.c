#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "gps.h"
#include "types.h"
#include "utils.h"
#include "date_support.h"

status_t ADT_GPS_Record_new(ADT_GPS_Record_t** precord)
{
	if(precord==NULL)
		return ERROR_NULL_POINTER;
	
	if(((*precord)=(ADT_GPS_Record_t*)malloc(sizeof(ADT_GPS_Record_t)))==NULL)
		return ERROR_MEMORY;
	
	(*precord)->latitude=0;
	(*precord)->longitude=0;
	(*precord)->orthometric_height=0;
	(*precord)->time_UTC=0;
	return OK;
}

status_t ADT_GPS_Record_delete(void * precord)
{
	if(precord==NULL)
		return ERROR_NULL_POINTER;

	precord=(ADT_GPS_Record_t*)(precord);
	free(precord);
	return OK;
}

/*NO PASAR PUNTERO NULO*/
bool_t is_valid_protocol(const string line,const string protocol)
{
	size_t i,j;
	char str[MAX_CHARACTERS_PROTOCOL];

	for(i=0,j=0; line[i]!=NMEA_DELIMITTER ; i++,j++)
		str[j]=line[i];

	return(strcmp(str,protocol))? FALSE:TRUE;
}

/*NO PASAR PUNTERO NULO*/
bool_t is_valid_NMEA_checksum(const string line)
{
	size_t i,check_sum,l=0;
	char validation_sum, c;
	char * tmp;
	char aux[MAX_CHARACTERS_PROTOCOL]=" ";
	validation_sum=VALIDATION_SUM_MASK;
 	
	for(i=0; line[i]!='\n' && line[i]!='\0'; i++)
		{	
			l=i;
			if(line[i]==NMEA_FINISH_CHARACTER_CHECKSUM)
				break;			
			if(line[i]!=NMEA_START_CHARACTER_CHECKSUM)
			{	 
				c=line[i];
				(validation_sum)^=(c&CHECKSUM_MASK);
			}
		}
	
	if(line[l]==NMEA_FINISH_CHARACTER_CHECKSUM)
		memcpy(aux,&line[++l],sizeof(char)*CHARACTERS_CHECKSUM);

	check_sum=strtoul(aux,&tmp,16);	
	if(*tmp)
		return FALSE;

	return (validation_sum==check_sum)? TRUE:FALSE; 
}

status_t ADT_GPS_Record_get_latitude(const string str,const string cardinal_point_indicator, double * latitude,  bool_t * is_valid_coordinate)
{	
	char str_aux[MAX_CHARACTERS_COORDINATES];
	int degrees;
	double minutes;
	char *tmp;

	if(str==NULL || latitude==NULL ||is_valid_coordinate==NULL || cardinal_point_indicator==NULL)
		return ERROR_NULL_POINTER;

	if(strlen(str)==0)
	{
		*is_valid_coordinate=FALSE;
		return OK;
	}
	memcpy(&str_aux,&str[0],NUMBER_DIGITS_LATITUDE_DEGREES);
	str_aux[NUMBER_DIGITS_LATITUDE_DEGREES]='\0';
	degrees=strtoul(str_aux,&tmp,10);
	if(*tmp)
	{
		*is_valid_coordinate=FALSE;
		return OK;
	}
	memcpy(&str_aux,&str[2],NUMBER_DIGITS_LATITUDE_MINUTES);
	str_aux[NUMBER_DIGITS_LATITUDE_MINUTES]='\0';
	minutes=strtod(str_aux,&tmp);
	if(*tmp)
	{
		*is_valid_coordinate=TRUE;
		return OK;
	}
	*latitude=degrees+ minutes/MINUTES_PER_DEGREE;
	if(*latitude > MAX_LATITUDE)
	{
		*is_valid_coordinate=FALSE;
		return OK;
	}
	if(strcmp(cardinal_point_indicator,CARDINAL_POINT_NORTH_TOKEN))
		*latitude = -(*latitude);

	*is_valid_coordinate=TRUE;
	return OK;
}

status_t ADT_GPS_Record_get_longitude(const string str,const string cardinal_point_indicator, double * longitude,  bool_t * is_valid_coordinate)
{	
	char str_aux[MAX_CHARACTERS_COORDINATES], *tmp;
	double degrees, minutes;
	
	if(str==NULL || longitude==NULL || is_valid_coordinate==NULL || cardinal_point_indicator==NULL)
		return ERROR_NULL_POINTER;
	
	if(strlen(str)==0)
	{
		*is_valid_coordinate=FALSE;
		return OK;
	}
	memcpy(&str_aux,&str[0],NUMBER_DIGITS_LONGITUDE_DEGREES);
	str_aux[NUMBER_DIGITS_LONGITUDE_DEGREES]='\0';
	degrees=strtoul(str_aux,&tmp,10);
	if(*tmp)
	{
		*is_valid_coordinate=FALSE;
		return OK;
	}

	memcpy(&str_aux,&str[2],NUMBER_DIGITS_LONGITUDE_MINUTES);
	str_aux[NUMBER_DIGITS_LONGITUDE_MINUTES]='\0';
	minutes=strtod(str_aux,&tmp);
	if(*tmp)
	{
		*is_valid_coordinate=FALSE;
		return OK;
	}
	*longitude= degrees + minutes/MINUTES_PER_DEGREE;
	if(*longitude > MAX_LONGITUDE)
	{
		*is_valid_coordinate=FALSE;
		return OK;
	}
	if(strcmp(cardinal_point_indicator,CARDINAL_POINT_EAST_TOKEN))
		*longitude = -(*longitude);
	
	*is_valid_coordinate=TRUE;
	return OK;
}

status_t ADT_GPS_Record_get_orthometric_height(const string str, double * orthometric_height, bool_t * is_valid_coordinate)
{
	char * tmp;

	if(str==NULL || orthometric_height==NULL || is_valid_coordinate==NULL)
		return ERROR_NULL_POINTER;

	if(strlen(str)==0)
	{
		*is_valid_coordinate=FALSE;
		return OK;
	}
	*orthometric_height=strtod(str,&tmp);
	if(*tmp)
	{
		*is_valid_coordinate=FALSE;
		return OK;
	}
	*is_valid_coordinate=TRUE;
	return OK;
}


/*Si es negativo, el valor p1 es más reciente.*/
int ADT_GPS_Record_diff_HHMMSS_time(const void * p1,const void * p2)
{
	ADT_GPS_Record_t*r1 = (ADT_GPS_Record_t*)p1;
	ADT_GPS_Record_t*r2 = (ADT_GPS_Record_t*)p2;
	return ((r1->time_UTC)-(r2->time_UTC));
}

status_t ADT_GPS_Record_get_time_UTC(const string str, int * time_UTC, bool_t * is_valid_time)
{
	char str_aux[MAX_CHARACTERS_TIME];
	char * tmp;

	if(str==NULL || time_UTC==NULL || is_valid_time==NULL)
		return ERROR_NULL_POINTER;

	if(strlen(str)==0)
	{
		*is_valid_time=FALSE;
		return OK;
	}
	memcpy(&str_aux[0],&str[0],NUMBER_DIGITS_TIME_UTC);
	str_aux[NUMBER_DIGITS_TIME_UTC]='\0';
	*time_UTC=strtoul(str_aux,&tmp,10);
	if(*tmp)
	{
		*is_valid_time=FALSE;
		return OK;
	}
	*is_valid_time=TRUE;
	return OK;	
}

status_t ADT_GPS_Record_export_as_CSV(void * pv, const char delimitter,FILE * fo) 
{
	status_t status;
	time_t seconds;
	ADT_GPS_Record_t * gps_precord;

	if(pv==NULL || fo==NULL)
		return ERROR_NULL_POINTER;

	gps_precord=(ADT_GPS_Record_t*)pv;

	if((status=get_system_calender_time(&seconds))!=OK)
		return status;

	if((status=print_date_YYYYMMDD(seconds,fo))!=OK)
		return status;

	fprintf(fo,"%d%c%f%c%f%c%f",gps_precord->time_UTC,delimitter,gps_precord->latitude,delimitter,gps_precord->longitude,delimitter,gps_precord->orthometric_height);
	fputc('\n',fo);
	return OK;
}

status_t ADT_GPS_Record_export_as_XML(void * pv, const char delimitter,FILE * fo) 
{
	ADT_GPS_Record_t* gps_precord;

	if(pv==NULL || fo==NULL)
		return ERROR_NULL_POINTER;

	gps_precord=(ADT_GPS_Record_t*)pv;
	fprintf(fo,"%f%c%f%c%f",gps_precord->latitude,delimitter,gps_precord->longitude,delimitter,gps_precord->orthometric_height);
	fputc('\n',fo);
	return OK;
} 

/*Suponemos que la linea leida es correcta, analiza y carga los datos que necesitamos.*/
status_t ADT_GPS_Record_load_GGA_line(const string str, ADT_GPS_Record_t** record, bool_t * is_valid_record)
{
	status_t st;
	string * gps_data;
	size_t size_records;
	bool_t is_valid_coordinate;
	
	if((st=ADT_GPS_Record_new(record))!=OK)
		return st;
	
	if((st=split(str,NMEA_DELIMITTER,&size_records,&gps_data))!=OK)
	{
		ADT_GPS_Record_delete(*record);
		return st;
	}
	st=ADT_GPS_Record_get_longitude(gps_data[POSITION_VALUE_LONGITUDE],gps_data[POSITION_EAST_WEST_INDICATOR],&((*record)->longitude),&is_valid_coordinate);
	if(st!=OK)
	{
		destroy_strings(&gps_data,size_records);
		ADT_GPS_Record_delete(*record);
		return st;
	}
	if((is_valid_coordinate==FALSE))
	{ 
		*is_valid_record=FALSE;
		return OK;
	}
	st=ADT_GPS_Record_get_latitude(gps_data[POSITION_VALUE_LATITUDE],gps_data[POSITION_NORTH_SOUTH_INDICATOR],&((*record)->latitude),&is_valid_coordinate);
	if(st!=OK)
	{
		destroy_strings(&gps_data,size_records);
		ADT_GPS_Record_delete(*record);
		return st;
	}
	if((is_valid_coordinate==FALSE))
	{ 
		*is_valid_record=FALSE;
		return OK;
	}
	st=ADT_GPS_Record_get_orthometric_height(gps_data[POSITION_ORTHOMETRIC_HEIGHT],&((*record)->orthometric_height),&is_valid_coordinate);
	if(st!=OK)
	{
		destroy_strings(&gps_data,size_records);
		ADT_GPS_Record_delete(*record);
		return st;
	}
	if((is_valid_coordinate==FALSE))
	{		
		*is_valid_record=FALSE;
		return OK;
	}
	st=ADT_GPS_Record_get_time_UTC(gps_data[POSITION_UTC_TIME],&( (*record)->time_UTC ),&is_valid_coordinate);
	if(st!=OK)
	{
		destroy_strings(&gps_data,size_records);
		ADT_GPS_Record_delete(*record);
		return st;
	}
	if((is_valid_coordinate==FALSE))
	{	 
		*is_valid_record=FALSE;
		return OK;
	}

	if((st=destroy_strings(&gps_data,size_records))!=OK)
		return st;

	*is_valid_record=TRUE;
	return OK;
}