cc=/usr/bin/g++
cflags= -c -Wall

build: main esm
	$(cc) ./build/obj/esm.o ./build/obj/main.o -o ./build/esmtest

prep:
	mkdir build
	mkdir build/obj

esm: 
	$(cc) $(cflags) ./esm.cpp -o ./build/obj/esm.o

main:
	$(cc) $(cflags) ./main.cpp -o ./build/obj/main.o

clean:
	rm -rf ./build/*.o

run:
	./build/esmtest