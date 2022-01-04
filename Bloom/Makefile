CC = clang
CFLAGS = -Wall -Werror -Wpedantic -Wextra
COMMON_OBJS = bst.o bv.o node.o bf.o speck.o ht.o parser.o

all: banhammer

banhammer: banhammer.o $(COMMON_OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

test: test.o bst.o node.o bv.o bf.o speck.o ht.o
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f banhammer test *.o

format:
	clang-format -i -style=file *.c *.h

# Credit to Sloan
