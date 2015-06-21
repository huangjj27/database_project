OBJ =  main.o catalog.o serial.o binary_tree.o
FILE = main.cpp catalog.cpp serial.cpp binary_tree.h binary_tree.cpp
DATA = catalog.data create.data
EXC = main.out
CC = g++
FLAG = -std=gnu++11

$(EXC): $(OBJ)
	$(CC) $(FLAG) -o $(EXC) $(OBJ)
main.o: main.cpp catalog.h serial.h
	$(CC) $(FLAG) -c main.cpp
serial.o: serial.cpp serial.h
	$(CC) $(FLAG) -c serial.cpp
catalog.o: catalog.cpp catalog.h binary_tree.h
	$(CC) $(FLAG) -c catalog.cpp
binary_tree.o: binary_tree.h binary_tree.cpp
	$(CC) $(FLAG) -c binary_tree.cpp

cleanAll:
	rm $(OBJ) $(EXC) $(DATA)
cleanOBJ:
	RM $(OBJ)
cleanDATA:
	rm $(DATA)
