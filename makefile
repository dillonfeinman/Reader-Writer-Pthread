CFLAGS = -Wall -g -Wextra -DDebug
P = readwrite
O = LinkedList

all: $(P)

$(P): $(P).o $(O).o
	g++ -g $(P).o -o $(P)

$(O).o: $(O).cpp $(O).h
	g++ -c $(O).cpp

$(P).o: $(P).cpp $(O).o
	g++ -c $(P).cpp


clean:
	rm -rf *.o $(P) $(O)
run: all
	./$(P)
checkmem: all
	valgrind -v --leak-check=full --track-origins=yes ./$(P)
