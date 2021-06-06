CC=g++
CFLAGS=-Isrc/include 
LFLAGS=-lboost_serialization $(shell wx-config-gtk3 --libs)
OBJECTS=bin/libs/dbms.o bin/libs/main.o 

bin/libs/dbms.o: src/libs/dbms.cpp src/include/dbms.h
	g++ $(CFLAGS) -c -o $@ $<

bin/libs/main.o: src/main.cpp src/include/dbms.h
	g++ $(CFLAGS) -c -o $@ $<

bin/libs/%.o: src/libs/%.cpp src/include/%.h
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(OBJECTS)
	$(CC) $(LFLAGS) $(CFLAGS) -o bin/proj.out  $^