
CFLAGS  += -g -D_XOPEN_SOURCE=700 -std=c11 -Wall -Wextra -Wpedantic
LDFLAGS +=
LDLIBS  +=

ifeq ($(CC),clang)
	CFLAGS += -Weverything
endif
