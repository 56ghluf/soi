all: run_tramp 

run_tramp: tramp
	./tramp

tramp: tramp.o
	g++ -O3 -std=c++17 -o tramp tramp.o

tramp.o: tramp.cpp
	g++ -O3 -std=c++17 -c tramp.cpp

clean:
	rm tramp tramp.o
