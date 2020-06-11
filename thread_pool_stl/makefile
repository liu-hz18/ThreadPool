CC = g++
STAND = -std=c++17
CFLAGS = 
CLIBS = 
SRC_OBJECT = main.cpp
H_OBJECT = threadpool.h
OBJECT = main

all: main

main: $(SRC_OBJECT) $(H_OBJECT)
	$(CC) $(STAND) $(CFLAGS) $(SRC_OBJECT) -o $(OBJECT) -lpthread

.PHONY: clean
clean:
	rm -rf ./*.o main

run:
	./main