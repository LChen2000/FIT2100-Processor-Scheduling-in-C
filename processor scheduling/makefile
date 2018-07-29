OBJS = 26445549.o processor_scheduling.o
CC = gcc
CFLAGS = -Wall -c
LFLAGS = -Wall

26445549: $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o 26445549

26445549.o: 26445549.c processor_scheduling.h
	$(CC) $(CFLAGS) 26445549.c

processor_scheduling.o: processor_scheduling.c processor_scheduling.h
	$(CC) $(CFLAGS) processor_scheduling.c

clean:
	touch *