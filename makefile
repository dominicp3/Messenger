CC=gcc
SEARCH=./lib
OUT=./bin
CFLAGS= -Wall -Werror=vla -Wextra -Wshadow -Wswitch-default -g -fsanitize=address

all: server client talk

server: server/server.c server/diagnostic.c $(SEARCH)/list.c $(SEARCH)/sendrecv.c
	$(CC) -I$(SEARCH) -o $(OUT)/$@ -lpthread $(CFLAGS) $^

client: client/client.c $(SEARCH)/sendrecv.c
	$(CC) -I$(SEARCH) -o $(OUT)/$@ -lpthread $(CFLAGS) $^ 

talk: server/talk.c $(SEARCH)/sendrecv.c
	$(CC) -I$(SEARCH) -o $(OUT)/$@ -lpthread $(CFLAGS) $^