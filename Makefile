# Write The Main Code Name Here Without Any Extensions

# Compiler
CC = g++

# Compiler flags
CFLAG = -std=c++11

# Input path
INPUT_PATH = ./headers/

# Executable file name
EXE_NAME = project

all: project

project: project.o $(INPUT_PATH)print_func.o $(INPUT_PATH)functions.o
	$(CC) $(CFLAG) project.o $(INPUT_PATH)print_func.o $(INPUT_PATH)functions.o -o $(EXE_NAME)

project.o: project.cpp
	$(CC) $(CFLAG) -c project.cpp

$(INPUT_PATH)print_func.o: $(INPUT_PATH)print_func.cpp
	$(CC) $(CFLAG) -c $(INPUT_PATH)print_func.cpp -o $(INPUT_PATH)print_func.o

$(INPUT_PATH)functions.o: $(INPUT_PATH)functions.cpp
	$(CC) $(CFLAG) -c $(INPUT_PATH)functions.cpp -o  $(INPUT_PATH)functions.o



.PHONY : clean
clean:
	rm -rf $(INPUT_PATH)*.o  $(INPUT_PATH)*~  *.o  *~ project  

