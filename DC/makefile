<<<<<<< HEAD
=======
#makefile for dataCreator
CCompiler = gcc
CFLAGS = -Wall -Wextra -I../Common/inc -Iinc

SRC = src
INC = inc
OBJ = obj

TARGETPROGRAM = ../DC/bin/dataCreator

SOURCES = $(SRC)/dataCreator.c
HEADERS = $(INC)/DC.h ../Common/inc/hooch.h
OBJECTS = $(OBJ)/dataCreator.o


$(TARGETPROGRAM): $(OBJECTS)
	$(CCompiler) $(CFLAGS) -o $(TARGETPROGRAM) $(OBJECTS)


#compilation of the object files
$(OBJ)/dataCreator.o: $(SOURCES) $(HEADERS)
	$(CCompiler) $(CFLAGS) -c $(SOURCES) -o $(OBJECTS)

clean:
	rm -rf obj bin
	rm -f ../DR/dataCreator

.PHONY: all clean
>>>>>>> 401e01e (makefile bs)
