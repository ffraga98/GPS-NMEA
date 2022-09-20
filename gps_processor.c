#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "config.h"
#include "gps.h"
#include "vector.h"
#include "gps_processor.h"
#include "utils.h"

status_t export_gps_data_as_KML(ADT_Vector_t * pv, const char delimitter,FILE * fo, printer_t printer)
{
	FILE * footer_file, * header_file;
	status_t status;

	if((footer_file=fopen(FOOTER_FILE,"rt"))==NULL) 
		return ERROR_OPENING_FOOTER_FILE;

	if((status=ADT_Vector_set_footer(&pv,footer_file))!=OK)
	{
		fclose(footer_file);
		return status;
	}
	fclose(footer_file);

	if((header_file=fopen(HEADER_FILE,"rt"))==NULL)
		return ERROR_OPENING_HEADER_FILE;
	
	if((status=ADT_Vector_set_header(&pv,header_file))!=OK)
	{	
		fclose(header_file);
		return status;
	}
	fclose(header_file);

	if((status=ADT_Vector_set_XML_tag(&pv,TAG_NAME))!=OK)
		return status;
		
	if((status=ADT_Vector_export_as_XML(pv,delimitter,fo,printer))!=OK)
		return status;

	return OK;
}


status_t process_gps_data(ADT_Vector_t * pv, FILE * fi, FILE * fo)
{
	status_t status;
	string str;
	bool_t eof,is_valid_record;
	ADT_GPS_Record_t * GPS_record;

	if((status=ADT_Vector_new(&pv))!=OK)
		return status;

	if((str=read_line(fi,&eof))==NULL)
		return ERROR_MEMORY;

	while(eof!=TRUE)
	{
		if((is_valid_protocol(str,PROTOCOL_GGA_TOKEN))==TRUE && (is_valid_NMEA_checksum(str))==TRUE )
		{ 
			if((status=ADT_GPS_Record_load_GGA_line(str,&GPS_record,&is_valid_record))!=OK)
				return status;

			if(is_valid_record==TRUE)
			{
				if((status=ADT_Vector_append_element(&pv,GPS_record,ADT_Vector_delete,ADT_GPS_Record_delete))!=OK)
				{	
					ADT_Vector_delete(&pv,ADT_GPS_Record_delete);
					return status;
				}
			}
		}
		free(str);
		if((str=read_line(fi,&eof))==NULL)
		{
			ADT_Vector_delete(&pv,ADT_GPS_Record_delete);
			return ERROR_MEMORY;
		}
	}
	free(str);

	if(ADT_Vector_is_empty(pv)==FALSE)
	{
		if((status=ADT_Vector_sort(&pv,ADT_GPS_Record_diff_HHMMSS_time))!=OK)
		{
			ADT_Vector_delete(&pv,ADT_GPS_Record_delete);
			return status;
		}
	}
	
	switch(config.doc_type)
	{
		case DOC_TYPE_CSV:

			if((status=ADT_Vector_export_as_CSV(pv,CSV_DELIMITTER,fo,ADT_GPS_Record_export_as_CSV))!=OK)
			{
				ADT_Vector_delete(&pv,ADT_GPS_Record_delete);
				return status;
			}
			break;
				
		case DOC_TYPE_KML:
		
			export_gps_data_as_KML(pv,COORDINATES_DELIMITTER,fo,ADT_GPS_Record_export_as_XML);
			{
				ADT_Vector_delete(&pv,ADT_GPS_Record_delete);
				return status;
			}
			
			free(str);
			break;
		
		default:
			ADT_Vector_delete(&pv,ADT_GPS_Record_delete);
			return ERROR_INVALID_FORMAT;
		
	}
	return OK;
}