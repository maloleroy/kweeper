# AUTOMATICALLY GENERATED FILE. MODIFY WITH CAUTION.
CC = gcc
.PHONY : clean

OBJ_FILES = obj/management.o obj/events.o obj/args.o obj/grid.o obj/paint.o obj/display.o
TARGETS = bin/kweeper $(OBJ_FILES)

CC_OPTIONS = -pedantic
CC_INCLUDE = -I./src
WARN = -Wall -Wextra
SDL2_FLAGS = $(shell sdl2-config --cflags --libs)
CFLAGS = $(CC_INCLUDE) $(CC_OPTIONS) $(SDL2_FLAGS) $(WARN)

bin/kweeper: src/kweeper.c $(OBJ_FILES)
	@echo "⚙ src/kweeper.c $(OBJ_FILES) -> bin/kweeper"
	@$(CC) src/kweeper.c $(OBJ_FILES) -o bin/kweeper $(CFLAGS)

obj/%.o: src/%.c
	@echo "⚙ $< -> $@"
	@$(CC) -I./src -c $< -o $@ $(CFLAGS)

bin/test: test/test.c $(OBJ_FILES)
	@$(CC) "test/test.c" /home/malo/Bureau/confer/obj/confer.o $(OBJ_FILES) $(CFLAGS) -I./test -I/home/malo/Bureau/confer/src/ -o "bin/test"

clean:
	rm ${TARGETS}
