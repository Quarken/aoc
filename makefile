BUILDDIR=build
CC=gcc
TARGET=aoc
SOURCES=aoc.c aoc_posix.c $(wildcard 2022/solutions/*.c)
INCLUDES=-I.

CFLAGS=-std=gnu11 -rdynamic -ldl
RELCFLAGS=-O3 -DNDEBUG
DBGCFLAGS=-g -Og -fsanitize=address

release: | $(BUILDDIR)
	$(CC) $(CFLAGS) $(RELCFLAGS) $(INCLUDES) $(SOURCES) -o $(BUILDDIR)/$(TARGET)

debug: | $(BUILDDIR)
	$(CC) $(CFLAGS) $(DBGCFLAGS) $(INCLUDES) $(SOURCES) -o $(BUILDDIR)/$(TARGET)

$(BUILDDIR):
	mkdir $(BUILDDIR)