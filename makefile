default: main
CC = g++
CFLAGS = -O3
CLIBS = -pthread
t = 4
n = 500

main: main.o
	$(CC) $(CLIBS) main.o -o main
main.o: main.cpp Mutex.h Locker.h Cond.h orderedList.h ThreadPool.h Runnable.h
	$(CC) $(CFLAGS) -c main.cpp -o main.o
debug: main.cpp Mutex.h Locker.h Cond.h orderedList.h ThreadPool.h Runnable.h
	$(CC) $(CFLAGS) $(CLIBS) -DDEBUG main.cpp -o main
clean:
	rm -rf ./*.o main
run:
	./main $(t) $(n)