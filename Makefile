all: client gc

client: ./obj/func.o ./obj/client.o 
	g++ ./obj/func.o ./obj/client.o -o ./bin/client
	
gc:  ./obj/func.o ./obj/gc.o 
	g++ ./obj/func.o ./obj/gc.o -o ./bin/gc

./obj/func.o:	./src/func.cpp ./src/func.h
	g++ -c ./src/func.cpp -o ./obj/func.o
	
./obj/client.o: ./src/client.cpp ./src/func.h
	g++ -c ./src/client.cpp -o ./obj/client.o
	
./obj/gc.o: ./src/gc.cpp
	g++ -c ./src/gc.cpp -o ./obj/gc.o