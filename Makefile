CC=gcc
CFLAGS=-Wall -std=c99 -I/usr/local/include -I/opt/homebrew/include
LDFLAGS=-L/usr/local/lib -L/opt/homebrew/lib -lraylib -lm -lpthread -ldl -lrt -lX11

# On macOS with homebrew, you might need:
# LDFLAGS=-L/opt/homebrew/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreFoundation

all: guitar_tabs_c

guitar_tabs_c: main_c.o guitar_tabs_c.o
	$(CC) $^ -o $@ $(LDFLAGS)

main_c.o: main_c.c guitar_tabs_c.h
	$(CC) $(CFLAGS) -c main_c.c -o $@

guitar_tabs_c.o: guitar_tabs_c.c guitar_tabs_c.h
	$(CC) $(CFLAGS) -c guitar_tabs_c.c -o $@

clean:
	rm -f *.o guitar_tabs_c

run: guitar_tabs_c
	./guitar_tabs_c