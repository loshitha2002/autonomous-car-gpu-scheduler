CC = gcc
CFLAGS = -Wall -Wextra -std=c11
OBJS = main.o scheduler.o

all: scheduler

scheduler: $(OBJS)
	$(CC) $(CFLAGS) -o scheduler $(OBJS)

main.o: main.c scheduler.h
	$(CC) $(CFLAGS) -c main.c

scheduler.o: scheduler.c scheduler.h
	$(CC) $(CFLAGS) -c scheduler.c

clean:
	rm -f *.o scheduler