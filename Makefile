CC=gcc
CFLAGS=-g -std=c99 -Wall
SRC=ADTs
OBJS_DIR=objs

all: $(OBJS_DIR) main

#create a directory to keep object files
$(OBJS_DIR): 
	mkdir $@

#craete a .o file at OBJS_DIR directory for every .c file on SRC directory
$(OBJS_DIR)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $^
	mv *.o $(OBJS_DIR)/

#linking all .o files
$(SRC)/lib.o: $(OBJS_DIR)/*.o
	ld -r -o $@ $^

#cleaniiiiiiiiiiiinnnnnnnnnnnnnnnngggggggggggg
clean:
	rm -r $(OBJS_DIR)
	rm $(SRC)/lib.o
	rm main 

#compiling the main.c file with the lib.o file
main: $(SRC)/lib.o main.c
	gcc $^ -o $@
