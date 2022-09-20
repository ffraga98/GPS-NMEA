CFLAGS= -Wall -ansi -pedantic -O2
CC=gcc

all: clear gpsviewer clean

gpsviewer: main.o utils.o gps.o vector.o error.o gps_processor.o date_support.o
	$(CC) $(CFLAGS) -o gpsviewer  main.o error.o utils.o gps.o vector.o gps_processor.o date_support.o 

main.o: main.c main.h types.h error.h vector.h gps_processor.h config.h
	$(CC) $(CFLAGS) -o  main.o -c main.c

utils.o: utils.c utils.h types.h
	$(CC) $(CFLAGS) -o utils.o -c utils.c

gps.o: gps.c gps.h types.h utils.h date_support.h
	$(CC) $(CFLAGS) -o gps.o -c gps.c

vector.o: vector.c vector.h types.h utils.h
	$(CC) $(CFLAGS) -o vector.o -c vector.c

gps_processor.o:gps_processor.c gps_processor.h types.h gps.h vector.h config.h
	$(CC) $(CFLAGS) -o gps_processor.o -c gps_processor.c

date_support.o: date_support.c date_support.h types.h
		$(CC) $(CFLAGS) -o date_support.o -c date_support.c

error.o: error.c error.h types.h
	$(CC) $(CFLAGS) -o error.o -c error.c

clean:
	rm *.o

clear:
	clear	

