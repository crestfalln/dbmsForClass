bin/libs/dbms.so: src/libs/dbms.cpp src/include/dbms.h
	g++ -I src/include -c -o bin/libs/dbms.so src/libs/dbms.cpp

bin/libs/main.so: src/main.cpp src/include/dbms.h
	g++ -I src/include -c -o bin/libs/main.so src/main.cpp

all: bin/libs/main.so bin/libs/dbms.so 
	g++ -o bin/proj.out -l boost_serialization bin/libs/main.so bin/libs/dbms.so 