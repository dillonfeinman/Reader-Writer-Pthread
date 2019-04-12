CFLAGS = -Wall -g -Wextra -DDebug -pthread

all: main.o buffer.o rwLock.o
	g++ $(CFLAGS) main.o buffer.o rwLock.o -o main

main.o: main.cpp buffer.h rwLock.h
	g++ $(CFLAGS) -c main.cpp

buffer.o: buffer.cpp buffer.h rwLock.h
	g++ $(CFLAGS) -c buffer.cpp

rwLock.o: rwLock.cpp buffer.h rwLock.h
	g++ $(CFLAGS) -c rwLock.cpp

clean:
	rm -rf *.o main
run: all
	./main
checkmem: all
	valgrind -v --leak-check=full --track-origins=yes ./$(P)
