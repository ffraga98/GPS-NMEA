#ifndef DATE_SUPPORT__H
#define DATE_SUPPORT__H

#include <stdio.h>
#include <time.h>
#include "types.h"

status_t get_system_calender_time(time_t * seconds);
status_t print_date_YYYYMMDD(const time_t seconds, FILE * stream);

#endif