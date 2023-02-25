CC = clang
OBJ = src/main.c src/texture.c src/tetris.c src/tetrimino.c
FLAGS = -g -Werror
INC = -I .
LIBS = -l SDL2 -l SDL2_image
TARGET = tetris

all: $(OBJ)
	$(CC) $(OBJ) $(FLAGS) $(INC) $(LIBS) -o $(TARGET)
