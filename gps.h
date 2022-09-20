#ifndef GPS__H
#define GPS__H

#include <stdio.h>
#include "types.h"

#define NMEA_DELIMITTER ','
#define NMEA_START_CHARACTER_CHECKSUM '$'
#define NMEA_FINISH_CHARACTER_CHECKSUM '*'
#define CHECKSUM_MASK 0x7f
#define MINUTES_PER_DEGREE 60
#define MAX_LATITUDE 90
#define MAX_LONGITUDE 180
#define CARDINAL_POINT_NORTH_TOKEN "N"
#define CARDINAL_POINT_SOUTH_TOKEN "S"
#define CARDINAL_POINT_EAST_TOKEN "E"
#define CARDINAL_POINT_WEST_TOKEN "W"
#define PROTOCOL_GGA_TOKEN "$GPGGA"
#define MAX_CHARACTERS_PROTOCOL 7
#define MAX_CHARACTERS_COORDINATES 11
#define MAX_CHARACTERS_TIME 15
#define POSITION_PROTOCOL 0
#define POSITION_UTC_TIME 1
#define POSITION_VALUE_LATITUDE 2
#define POSITION_NORTH_SOUTH_INDICATOR 3
#define POSITION_VALUE_LONGITUDE 4
#define POSITION_EAST_WEST_INDICATOR 5
#define POSITION_ORTHOMETRIC_HEIGHT 9
#define VALIDATION_SUM_MASK 0x00
#define CHARACTERS_CHECKSUM 2
#define NUMBER_DIGITS_LATITUDE_DEGREES 2
#define NUMBER_DIGITS_LATITUDE_MINUTES 8
#define NUMBER_DIGITS_LONGITUDE_DEGREES 3
#define NUMBER_DIGITS_LONGITUDE_MINUTES 8
#define NUMBER_DIGITS_TIME_UTC 6

typedef struct{
	double latitude;
	double longitude;
	double orthometric_height;
	int time_UTC;
}ADT_GPS_Record_t;

status_t ADT_GPS_Record_new(ADT_GPS_Record_t**);
status_t ADT_GPS_Record_delete(void *);
status_t ADT_GPS_Record_get_latitude(const string str,const string cardinal_point_indicator, double * latitude,  bool_t * is_valid_coordinate);
status_t ADT_GPS_Record_get_longitude(const string str,const string cardinal_point_indicator, double * latitude,  bool_t * is_valid_coordinate);
status_t ADT_GPS_Record_get_orthometric_height(const string str, double * orthometric_height, bool_t * is_valid_coordinate);
status_t ADT_GPS_Record_get_time_UTC(const string str, int * time, bool_t * is_valid_time);
status_t ADT_GPS_Record_export_as_CSV(void * pv,const char delimitter,FILE * fo);
status_t ADT_GPS_Record_export_as_XML(void * pv,const char delimitter,FILE * fo);
bool_t is_valid_protocol(const string line,const string protocol);
bool_t is_valid_NMEA_checksum(const string line);
status_t ADT_GPS_Record_load_GGA_line(const string str, ADT_GPS_Record_t** record, bool_t * is_valid_record);
int ADT_GPS_Record_diff_HHMMSS_time(const void* ,const  void*);

#endif