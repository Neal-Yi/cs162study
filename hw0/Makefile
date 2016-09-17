CC=gcc
CFLAGS=-g
LDFLAGS=
SOURCES=main.c map.c wc.c
# generate all executable file 
EXECUTABLE=$(SOURCES:.c=)
all:$(EXECUTABLE)

.c:
	$(CC) $(CFLAGS) $< -o $@
	sudo chmod +x $@
clean:
	rm main map wc
