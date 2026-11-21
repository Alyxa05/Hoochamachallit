CC = gcc
CFLAGS = -Wall -Wextra -I../Common/inc

TARGET = bin/datareader
SRC = src/datareader.c
OBJ = obj/datareader.o

all: $(TARGET)

$(TARGET): $(OBJ)
	mkdir -p bin
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

$(OBJ): $(SRC) ../Common/inc/hooch.h
	mkdir -p obj
	$(CC) $(CFLAGS) -c -o $(OBJ) $(SRC)

clean:
	rm -rf obj bin

.PHONY: all clean