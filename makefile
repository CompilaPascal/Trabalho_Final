CC=gcc
CC_FLAGS= -O2 -Wall
BINARY?=analisador_lexico

all: $(BINARY)

analisador_lexico: 
	$(CC) $(CC_FLAGS) $@.c -o $@

clean:
	rm -f $(BINARY) entrada
