PASTAJAVA := $(shell ../../../scripts/findjava.sh)
INCL=-I../../../diversity/cpp -I$(PASTAJAVA)/include/ -I$(PASTAJAVA)/include/linux -I../../protobuf/list2/build -I../../../diversity/c
LIBS=-L $(PASTAJAVA)/jre/lib/amd64/server
INCL += `PKG_CONFIG_PATH=../../../protobuf/build/lib/pkgconfig pkg-config --cflags --libs libprotobuf-c protobuf | sed 's/-lprotobuf-c//' | sed 's/-lprotobuf//'`
CCFLAGS=-g -Wextra -Wall 
CPFLAGS=-g -Wextra -Wall -fpermissive --std=c++0x
SFX=../../../protobuf/build/lib/libprotobuf-c.a
SFX += ../../../protobuf/build/lib/libprotobuf.a

all : bench_server bench_client

bench_server: 
	-mkdir build
	g++ $(CPFLAGS) -o build/bench_server.out $(INCL) $(LIBS) ../../protobuf/list2/build/*.pb.cc replica.cpp ../../../diversity/cpp/build/bftsmart-server.o ../../../diversity/c/build/bftsmart-c-server.o ../../../diversity/c/build/wrapper.o $(SFX) -ljvm

bench_client:
	-mkdir build
	g++ $(CPFLAGS) -o build/bench_client.out $(INCL) $(LIBS) ../../protobuf/list2/build/*.pb.cc test_client.cpp ../../../diversity/cpp/build/bftsmart-client.o ../../../diversity/c/build/bftsmart-c-client.o ../../../diversity/c/build/wrapper.o $(SFX) -ljvm


.PHONY: clean

clean:
	rm -rfv *.o *~ *.out build



   


