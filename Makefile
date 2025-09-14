CC = gcc
# FLAGS= -Wall -std=c99 -pedantic -g -lSDL3
FLAGS= -Wall -std=c99 -g -lSDL3

MAIN = main.out
OBJS = bin/main.o bin/functions.o
PROGS = $(MAIN)

all : $(PROGS)

bin/%.o: src/%.c
	$(CC) $(FLAGS) -c $< -o $@

$(MAIN) : $(OBJS)
	$(CC) $(FLAGS) -o $(MAIN) $(OBJS)

clean :
	rm bin/*.o $(PROGS) core
