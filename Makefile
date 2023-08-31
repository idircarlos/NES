CC=gcc
CFLAGS=-Wall -Wextra -pedantic -O1 -std=c99 -Wunused-parameter
LIBS=-L. -LC:/raylib/raylib/src -LC:/raylib/raylib/src
INCLUDE=-I. -IC:/raylib/raylib/src -IC:/raylib/raylib/src/external
RAYLIBFLAGS=-lraylib -lopengl32 -lgdi32 -lwinmm -DPLATFORM_DESKTOP
RAYLIBDATA=C:/raylib/raylib/src/raylib.rc.data

nes: src/*.c src/mappers/*.c
		$(CC) $(CFLAGS) -o nes src/*.c src/mappers/*.c $(RAYLIBDATA) $(LIBS) $(INCLUDE) $(RAYLIBFLAGS)

