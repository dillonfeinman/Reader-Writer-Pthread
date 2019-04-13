CFLAGS = -Wall -g -Wextra -DDebug -pthread
P = readwrite

all: $(P)

$(P): $(P).o
	g++ $(CFLAGS) $(P).o -o $(P)


$(P).o: $(P).cpp node.h llist.h
	g++ -c $(CFLAGS) $(P).cpp


clean:
	rm -rf *.o $(P) $(O) *.txt
run: all
	./$(P)
checkmem: all
	valgrind -v --leak-check=full --track-origins=yes ./$(P)
