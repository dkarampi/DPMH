CC = gcc
CFLAGS = -c -Wall -pedantic -std=c99 -g
SOURCES = assignment01.c
OBJECTS = $(SOURCES:.c=.o)
EXECUTABLE = prog

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@
