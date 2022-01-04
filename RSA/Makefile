CC = clang
CFLAGS = -Wall -Werror -Wpedantic $(shell pkg-config --cflags gmp)
LDFLAGS = $(shell pkg-config --libs gmp)
COMMON_OBJS = rsa.o randstate.o numtheory.o

all: keygen encrypt decrypt

keygen: keygen.o $(COMMON_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) -lm

encrypt: encrypt.o $(COMMON_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) -lm

decrypt: decrypt.o $(COMMON_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS) -lm

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f keygen encrypt decrypt rsa *.o

cleankeys:
	rm -f *.(pub, priv)

format:
	clang-format -i -style=file *.c *.h

# Credit to Sloan
