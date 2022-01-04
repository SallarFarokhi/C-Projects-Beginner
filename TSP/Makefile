CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

all: tsp

tsp: graph.o path.o stack.o tsp.o
	$(CC) -o tsp $^ -lm 

%.o: %.c
	$(CC) $(CFLAGS) -c $^
	
clean:
	rm -f tsp graph.o path.o stack.o tsp.o

format:
	clang-format -i -style=file *.[ch] graph.c path.c stack.c tsp.c
