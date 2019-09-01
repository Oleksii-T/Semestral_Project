#constants
flags=-Wall -pedantic -Wno-long-long -O0 -ggdb -std=c++11
objects=bin/main.o bin/CChess.o bin/CBoard.o bin/CPiece.o

#commands
all: compile doc

compile: ./tarbeole

run: ./tarbeole
	./tarbeole

clean:
	rm -rf bin doc ./scores.txt ./tarbeole
	rm -f Game_*.txt

#rules
./tarbeole: $(objects)
	g++ $(flags) $(objects) -o ./tarbeole

bin/main.o: src/main.cpp src/CChess.hpp
	mkdir -p bin
	g++ $(flags) -c src/main.cpp -o bin/main.o

bin/CChess.o: src/CChess.cpp src/CChess.hpp src/CBoard.hpp src/CPiece.hpp
	mkdir -p bin
	g++ $(flags) -c src/CChess.cpp -o bin/CChess.o

bin/CBoard.o: src/CBoard.cpp src/CBoard.hpp src/CPiece.hpp
	mkdir -p bin
	g++ $(flags) -c src/CBoard.cpp -o bin/CBoard.o

bin/CPiece.o: src/CPiece.cpp src/CPiece.hpp
	mkdir -p bin
	g++ $(flags) -c src/CPiece.cpp -o bin/CPiece.o

doc:
	doxygen
	

