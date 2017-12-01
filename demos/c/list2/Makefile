PASTAJAVA := $(shell ../../../scripts/findjava.sh)
INCL=-I../../../diversity/c -I$(PASTAJAVA)/include/ -I$(PASTAJAVA)/include/linux -I../../protobuf/list2/build
LIBS=-L $(PASTAJAVA)/jre/lib/amd64/server
INCL += `PKG_CONFIG_PATH=../../../protobuf/build/lib/pkgconfig pkg-config --cflags --libs libprotobuf-c protobuf | sed 's/-lprotobuf-c//' | sed 's/-lprotobuf//'`
CCFLAGS=-g -Wextra -Wall 
CPFLAGS=-g -Wextra -Wall -fpermissive --std=c++0x
SFX=../../../protobuf/build/lib/libprotobuf-c.a
SFX += ../../../protobuf/build/lib/libprotobuf.a

bench_server: 
	-mkdir build
	gcc $(CCFLAGS) -o build/bench_server.out $(INCL) $(LIBS) ../../protobuf/list2/build/*.pb-c.c bench_server.c ../../../diversity/c/build/wrapper.o ../../../diversity/c/build/bftsmart-c-server.o $(SFX) -lstdc++ -ljvm

.PHONY: clean

clean:
	rm -rfv *.o *~ *.out build



   


