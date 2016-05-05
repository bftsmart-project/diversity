PASTAJAVA := $(shell ../../../../util/findjava.sh)
INCL=-I../../../bft-smart/intermediate/src -I$(PASTAJAVA)/include/ -I$(PASTAJAVA)/include/linux -I../protobuf/build
LIBS=-L $(PASTAJAVA)/jre/lib/amd64/server
INCL += `PKG_CONFIG_PATH=../../protobuf/build/lib/pkgconfig pkg-config --cflags --libs libprotobuf-c protobuf | sed 's/-lprotobuf-c//' | sed 's/-lprotobuf//'`
CCFLAGS=-g -Wextra -Wall -fpermissive --std=c++0x
CPFLAGS=-g -Wextra -Wall -fpermissive --std=c++0x
SFX=../../protobuf/build/lib/libprotobuf-c.a
SFX += ../../protobuf/build/lib/libprotobuf.a

bench_server: 
	-mkdir build
	gcc $(CCFLAGS) -o build/bench_server.out $(INCL) $(LIBS) ../protobuf/build/Vazio.pb-c.c bench_server.c ../../../bft-smart/intermediate/dist/wrapper.o $(SFX) -ljvm

.PHONY: clean

clean:
	rm -fv *.o *~ *.out build



   


