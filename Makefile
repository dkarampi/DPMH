CC			= gcc
CFLAGS		= -Wall -Wextra -pedantic -std=c99
DEBUGFLAGS	= -g -O0 -DDEBUG
RELEASEFLAGS= -O3

SOURCES		= assignment01.c
OBJECTS		= $(SOURCES:.c=.o)
TARGET		= prog

all: $(TARGET)

$(TARGET): $(OBJECTS) 
	$(CC) $(DEBUGFLAGS) $(OBJECTS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(DEBUGFLAGS) -c $< -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)
