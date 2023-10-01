cc=/usr/bin/g++
cflags= -c -Wall

buildcli: maincli esm
	$(cc) ./build/obj/esm.o ./build/obj/maincli.o -o ./build/esmtestcli

prep:
	mkdir build
	mkdir build/obj

esm: 
	$(cc) $(cflags) ./esm.cpp -o ./build/obj/esm.o

maincli:
	$(cc) $(cflags) ./maincli.cpp -o ./build/obj/maincli.o

clean:
	rm -rf ./build/*.o

run:
	./build/esmtestcli