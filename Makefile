# Write The Main Code Name Here Without Any Extensions


# Spcify Compiler
CC = g++ -std=c++11

# Specify the executable name
EXE_NAME = project


# Write The Input Path for Header Files and Objects
INPUT_PATH = ./headers/


# Specify Headers : The ".h" Files for your Library 
HEADERS = $(INPUT_PATH)print_func.h       \
	$(INPUT_PATH)functions.h	\

                

# Specify Objects : The ".o" Files Generated by Compiling the ".cpp" Files
OBJECTS = $(INPUT_PATH)print_func.o        \
	$(INPUT_PATH)functions.o	\


# Object File for print_functions used in the main program
$(INPUT_PATH)print_func.o : $(patsubst %.o, %.cpp, $@) $(INPUT_PATH)print_func.h
	$(CC) -c $(patsubst %.o, %.cpp, $@) -o $@



# Object File for functions : Contains all the functions used in algorithms
$(INPUT_PATH)functions.o : $(patsubst %.o, %.cpp, $@) $(INPUT_PATH)functions.h
	$(CC) -c $(patsubst %.o, %.cpp, $@) -o $@


# Object File for kstat_functions : Contains all the functions related to kstat
#$(INPUT_PATH)kstat_functions.o : $(patsubst %.o, %.c, $@) $(INPUT_PATH)kstat_functions.h
#        $(CC) -c $(patsubst %.o, %.c, $@) -o $@



         
# Compilation of the Main Code
ifneq ($(EXE_NAME), clean)
$(EXE_NAME) : $(EXE_NAME).o $(OBJECTS)
	$(CC) -o $(EXE_NAME)   $(EXE_NAME).o $(OBJECTS)  

# Main Code Object File
$(EXE_NAME).o : $(EXE_NAME).cpp $(HEADERS)
	$(CC) -c $(EXE_NAME).cpp 
endif

        


.PHONY : clean
clean :
	rm -f $(OBJECTS) *.o
