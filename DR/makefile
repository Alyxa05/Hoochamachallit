CC = gcc
CFLAGS = -Wall -Wextra -Iinc -I../Common/inc

OBJS = obj/datareader.o obj/dr_helpers.o
TARGET = bin/datareader

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p bin
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

obj/datareader.o: src/datareader.c inc/dr_helpers.h ../Common/inc/hooch.h
	@mkdir -p obj
	$(CC) $(CFLAGS) -c src/datareader.c -o obj/datareader.o

obj/dr_helpers.o: src/dr_helpers.c inc/dr_helpers.h ../Common/inc/hooch.h
	@mkdir -p obj
	$(CC) $(CFLAGS) -c src/dr_helpers.c -o obj/dr_helpers.o

clean:
	rm -rf obj bin

.PHONY: all clean