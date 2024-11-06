all: run_main 

run_main: main
	./main

main: main.o
	g++ -O3 -std=c++17 -o main main.o

main.o: main.cpp
	g++ -O3 -std=c++17 -c main.cpp

clean:
	rm main main.o
