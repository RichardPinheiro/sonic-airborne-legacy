CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g -fsanitize=address -Iinclude $(shell pkg-config --cflags sdl2 sdl2_mixer sdl2_image)
LIBS = $(shell pkg-config --libs sdl2 sdl2_mixer sdl2_image) -fsanitize=address

OBJECTS = src/game.o src/sprite.o src/enemy.o src/buzz.o src/sonic.o src/events.o src/audio.o

game: $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f game $(OBJECTS)