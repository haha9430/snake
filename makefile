main.exe: main.o console.o
	g++ -o main.exe main.o console.o

main.o: main.cpp
	g++ -Werror -std=c++11 -c main.cpp

console.o: console.cpp
	g++ -Werror -std=c++11 -c console.cpp