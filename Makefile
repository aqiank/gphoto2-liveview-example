CC=gcc
CFLAGS=-O2 -std=c99
LDFLAGS=-lSDL2 -lSDL2_image
OUTPUT_FILENAME=gphoto2-liveview-example

all: $(OUTPUT_FILENAME)

$(OUTPUT_FILENAME): main.c
	$(CC) $? $(CFLAGS) $(LDFLAGS) -o $(OUTPUT_FILENAME)

clean:
	-@rm $(OUTPUT_FILENAME) 2> /dev/null || true
