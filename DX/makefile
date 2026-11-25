#makefile for DX, the data corruptor
CCOMPILER = gcc
CFLAGS = -std=c11

#variables formatted as
#{varrName} = {folderName or path}
SRC = src
INC = inc
OBJ = obj

TARGETPROGRAM = ../DR/bin/datacorruptor

SOURCES = $(SRC)/dataCorruptor.c $(SRC)/corruptorSupport.c
HEADERS = $(INC)/dataCorruptor.h ../Common/inc/hooch.h
OBJECTS = $(OBJ)/dataCorruptor.o $(OBJ)/corruptorSupport.o


$(TARGETPROGRAM): $(OBJECTS)
	@mkdir -p ../DR/bin
	$(CCOMPILER) $(CFLAGS) -o $(TARGETPROGRAM) $(OBJECTS)


#compilation of the object files
$(OBJ)/dataCorruptor.o: $(SRC)/dataCorruptor.c $(HEADERS)
	@mkdir -p $(OBJ)
	$(CCOMPILER) $(CFLAGS) -I$(INC) -I../Common/inc -c $(SRC)/dataCorruptor.c -o $(OBJ)/dataCorruptor.o

$(OBJ)/corruptorSupport.o: $(SRC)/corruptorSupport.c $(HEADERS)
	@mkdir -p $(OBJ)
	$(CCOMPILER) $(CFLAGS) -I$(INC) -I../Common/inc -c $(SRC)/corruptorSupport.c -o $(OBJ)/corruptorSupport.o

clean:
	rm -f ./obj/*.o
	rm -f ../DR/bin/datacorruptor
