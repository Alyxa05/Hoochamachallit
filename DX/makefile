#makefile for DX, the data corruptor
CCOMPILER = gcc
CFLAGS = -std=11 -Wall -Wextra -Werror -pedantic

#variables formatted as
#{varrName} = {folderName or path}
SRC = src
INC = inc
OBJ = obj

TARGETPROGRAM = ../DR/datacorruptor

SOURCES = $(SRC)/datacorruptor.c 
HEADERS = $(INC)/datacorruptor.h ../Common/inc/hooch.h
OBJECTS = $(OBJ)/datacorruptor.o


$(TARGETPROGRAM): $(OBJECTS)
	$(CCOMPILER) $(CFLAGS) -o $(TARGETPROGRAM) $(OBJECTS)


#compilation of the object files
$(OBJ)/datacorruptor.o: $(SOURCES) $(HEADERS)
	$(CCOMPILER) $(CFLAGS) -I$(HEADERS) -c $(SOURCES) -o $(OBJECTS)


clean:
	rm -f ./obj/*.o 
	rm -f ../DR/datacorruptor