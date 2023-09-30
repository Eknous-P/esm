cc=/usr/bin/g++
cflags= -c -Wall

build: main esm
	$(cc) ./build/esm.o ./build/main.o -o ./build/esmtest

esm: 
	$(cc) $(cflags) ./esm.cpp -o ./build/esm.o

main:
	$(cc) $(cflags) ./main.cpp -o ./build/main.o

clean:
	rm -rf ./build/*.o

run:
	./build/esmtest