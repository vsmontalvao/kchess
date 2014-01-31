all: xadrez

posMov.o: posMov.cpp
	g++ -c posMov.cpp -o posMov.o

main.o: main.cpp
	g++ -c main.cpp -o main.o

Aberturas.o: Aberturas.cpp
	g++ -c Aberturas.cpp -o Aberturas.o
	
heuHash.o: heuHash.cpp
	g++ -c heuHash.cpp -o heuHash.o
	
Decision.o: Decision.cpp
	g++ -c Decision.cpp -o Decision.o
	
Heuristic.o: Heuristic.cpp
	g++ -c Heuristic.cpp -o Heuristic.o

heuristics.o: heuristics.cpp
	g++ -c heuristics.cpp -o heuristics.o
	
Move_List.o: Move_List.cpp
	g++ -c Move_List.cpp -o Move_List.o
	
move.o: move.cpp
	g++ -c move.cpp -o move.o
	
xadrez: posMov.o main.o Move_List.o heuristics.o Heuristic.o Decision.o heuHash.o Aberturas.o move.o
	g++ posMov.o main.o Move_List.o heuristics.o Heuristic.o Decision.o heuHash.o Aberturas.o move.o -o xadrez -lSDL -lSDL_image
	
clean:
	rm *.o
