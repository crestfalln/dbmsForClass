CC=g++
CFLAGS=-Isrc/include $(shell wx-config-gtk3 --cxxflags) -g
LFLAGS= $(shell wx-config-gtk3 --libs) 
OBJECTS=bin/libs/dbms.o bin/libs/main.o bin/libs/ui.o

bin/libs/%.o: src/libs/%.cpp src/include/%.h
	$(CC) $(CFLAGS) -c -o $@ $<

bin/libs/main.o: src/main.cpp src/include/dbms.h
	g++ $(CFLAGS) -c -o $@ $<

all: $(OBJECTS)
	$(CC) $(LFLAGS) $(CFLAGS) -o bin/proj.out  $^