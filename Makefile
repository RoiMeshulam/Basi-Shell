CC=gcc
AR=ar
FLAGS= -Wall -g

all: shell server

shell:  Shell.o
	$(CC) $(FLAGS) -o Shell Shell.o

server:  Server.o
	$(CC) $(FLAGS) -o Server Server.o

Server.o: Server.c
	$(CC) $(FLAGS) -c Server.c

Shell.o: Shell.c
	$(CC) $(FLAGS) -c Shell.c

.PHONY: clean all
clean:
	rm -f *.o Shell Server
