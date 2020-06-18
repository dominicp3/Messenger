CC=gcc
CFLAGS=-Wall -Werror=vla -Wextra -Wshadow -Wswitch-default
ASAN=-g -fsanitize=address
SEARCH=-I./include
OUT=-o ./bin/$@
COMPILE=$(CC) $(SEARCH) $(OUT) $^ $(CFLAGS) $(ASAN) -lpthread

CLIENT_SRC=./src/client
SENDRECV_SRC=./src/sendrecv
SERVER_SRC=./src/server

SERVER_PREREQ=$(SERVER_SRC)/command.c $(SERVER_SRC)/config.c $(SERVER_SRC)/list.c $(SERVER_SRC)/request.c $(SERVER_SRC)/server.c $(SENDRECV_SRC)/sendrecv.c
TALK_PREREQ=$(SERVER_SRC)/talk.c $(SENDRECV_SRC)/sendrecv.c
CLIENT_PREREQ=$(CLIENT_SRC)/command.c $(CLIENT_SRC)/config.c $(CLIENT_SRC)/request.c $(SENDRECV_SRC)/sendrecv.c

PROGRAMS=server talk client

all: $(PROGRAMS)

server: $(SERVER_PREREQ)
	$(COMPILE)

talk: $(TALK_PREREQ)
	$(COMPILE)

client: $(CLIENT_PREREQ)
	$(COMPILE)