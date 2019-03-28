CFLAGS = -Wall -g -Wextra -DDebug
P = readwrite

all: $(P)

$(P): $(P).cpp
	g++ -c $(P).cpp
	g++ -g $(P).o -o $(P)


clean:
	rm -rf *.o $(P) *.csv *.png
run: all
	./$(P)
checkmem: all
	valgrind -v --leak-check=full --track-origins=yes ./$(P)
