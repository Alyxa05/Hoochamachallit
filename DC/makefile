#makefile for dataCreator
CCompiler = gcc
CFLAGS = -Wall -Wextra -I../Common/inc -Iinc

SRC = src
INC = inc
OBJ = obj

TARGETPROGRAM = ../DC/bin/datacreator

SOURCES = $(SRC)/dataCreator.c
HEADERS = $(INC)/DC.h ../Common/inc/hooch.h
OBJECTS = $(OBJ)/dataCreator.o


$(TARGETPROGRAM): $(OBJECTS)
	@mkdir -p ../DC/bin
	$(CCompiler) $(CFLAGS) -o $(TARGETPROGRAM) $(OBJECTS)


#compilation of the object files
$(OBJ)/dataCreator.o: $(SOURCES) $(HEADERS)
	@mkdir -p $(OBJ)
	$(CCompiler) $(CFLAGS) -c $(SOURCES) -o $(OBJECTS)

clean:
	rm -rf obj bin
	rm -f ../DR/bin/datacreator

.PHONY: all clean