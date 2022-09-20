#ifndef GPS_DATA_PROCESSING__H
#define GPS_DATA_PROCESSING__H

#include <stdio.h>
#include "vector.h"
#include "types.h"
#include "config.h"

#define MAX_CARACTERES_CADENA 100
#define TAG_NAME "coordinates"
#define HEADER_FILE "kml_header.txt"
#define FOOTER_FILE "kml_footer.txt"
#define CSV_DELIMITTER '|'
#define COORDINATES_DELIMITTER ','

status_t process_gps_data(ADT_Vector_t *pv, FILE * fi, FILE * fo);
status_t export_gps_data_as_KML(ADT_Vector_t * pv,const char delimitter,FILE * fo, printer_t printer);

#endif