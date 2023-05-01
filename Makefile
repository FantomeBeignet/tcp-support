BUILD := release

CC=/usr/bin/clang
LDFLAGS=-L/lib 
CPPFLAGS=-I/include
cflags.common := -std=c99 -Wall -Wextra -pedantic -fdiagnostics-color=always
cflags.release := -O2
cflags.debug := -Werror -O0
CFLAGS+=$(CPPFLAGS) ${cflags.common} ${cflags.${BUILD}}

SRCS=$(wildcard src/*.c)
OBJS=$(SRCS:.c=.o)

TEST_SRCS=$(wildcard test/*.c)
TEST_OBJS=$(TEST_SRCS:.c=.o)
TESTS=$(TEST_OBJS:.o=.exe)

all: $(OBJS)

$(TESTS): $(TEST_OBJS) $(OBJS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@

test: $(TESTS)
	for test in $(TESTS); do $$test; done

clean:
	rm -rf $(OBJS) $(TEST_OBJS) $(TESTS) $(OUT)

.PHONY:
	clean
