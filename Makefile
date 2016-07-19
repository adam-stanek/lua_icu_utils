SRC = $(wildcard src/*.c)
OBJ = ${SRC:.c=.o}

CC = gcc

# Theese flags are overriden by luarocks
# We just provide sane defaults for case when make is called without
# luarocks (in dev environment).
CFLAGS = -O2 -fPIC
LUA_INCDIR = /usr/share/nginx-openresty/luajit/include/luajit-2.1
LDFLAGS = -shared

# Combined flags
CFLAGS_ALL = $(CFLAGS) -Isrc -I$(LUA_INCDIR)
LDFLAGS_ALL = $(LDFLAGS) -licuuc -licui18n -licuio

all: icu_utils.so

icu_utils.so: $(OBJ)
	$(CC) $^ $(LDFLAGS_ALL) -o $@

%.o: %.c
	$(CC) $(CFLAGS_ALL) -c $< -o $@

test: icu_utils.so
	busted --cpath ./?.so spec

install: icu_utils.so
	cp icu_utils.so $(INST_LIBDIR)/

clean:
	rm -f icu_utils.so $(OBJ)

.PHONY: clean
