CC=gcc
<<<<<<< HEAD
CFLAGS=-ggdb3 -c -Wall
LDFLAGS=-pthread
SOURCES=httpserver.c libhttp.c
=======
CFLAGS=-ggdb3 -c -Wall -std=gnu99
LDFLAGS=-pthread
SOURCES=httpserver.c libhttp.c wq.c
>>>>>>> a6ef9f69aaa37bd8797091bd4eb338c15b7a7c2d
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=httpserver

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)
