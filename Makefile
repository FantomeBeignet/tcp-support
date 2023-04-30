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

all: $(OBJS)

clean:
	rm -rf $(OBJS) $(OUT)

.PHONY:
	clean
