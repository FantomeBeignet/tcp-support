BUILD := release

CC=/usr/bin/clang
LDFLAGS=-L/lib 
CPPFLAGS=-I/include
cflags.common := -std=gnu99 -Wall -Wextra -pedantic -fdiagnostics-color=always
cflags.release := -O2
cflags.debug := -Werror -O0
CFLAGS+=$(CPPFLAGS) ${cflags.common} ${cflags.${BUILD}}

COMMON_SRC=$(wildcard src/common/*.c)
COMMON_OBJS=$(COMMON_SRC:.c=.o)

CLIENT_SRC=$(wildcard src/client/*.c)
CLIENT_OBJS=$(CLIENT_SRC:.c=.o)
CLIENT_OUT=client

SERVER_SRC=$(wildcard src/server/*.c)
SERVER_OBJS=$(SERVER_SRC:.c=.o)
SERVER_OUT=server

SRCS=$(COMMON_SRC) $(CLIENT_SRC) $(SERVER_SRC)
OBJS=$(COMMON_OBJS) $(CLIENT_OBJS) $(SERVER_OBJS)

TEST_SRCS=test/test_protocol.c
TEST_OBJS=$(TEST_SRCS:.c=.o)
TESTS=$(TEST_OBJS:.o=.exe)

OUT=$(CLIENT_OUT) $(SERVER_OUT)

all: common client server

common: $(COMMON_OBJS)

client: $(CLIENT_OBJS) $(COMMON_OBJS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $(CLIENT_OUT)

server: $(SERVER_OBJS) $(COMMON_OBJS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $(SERVER_OUT)

test/test_protocol.exe: test/test_protocol.o src/common/protocol.o
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@

test: $(TESTS)
	for test in $(TESTS); do $$test; done

clean:
	rm -rf $(OBJS) $(TEST_OBJS) $(TESTS) $(OUT)

.PHONY:
	clean
