PASTAJAVA := $(shell ../../../../util/findjava.sh)
INCL=-I../../../bft-smart/intermediate/src -I$(PASTAJAVA)/include/ -I$(PASTAJAVA)/include/linux -I../protobuf/build
LIBS=-L $(PASTAJAVA)/jre/lib/amd64/server
INCL += `PKG_CONFIG_PATH=../../protobuf/build/lib/pkgconfig pkg-config --cflags --libs libprotobuf-c protobuf | sed 's/-lprotobuf-c//' | sed 's/-lprotobuf//'`
CCFLAGS=-g -Wextra -Wall -fpermissive --std=c++0x
CPFLAGS=-g -Wextra -Wall -fpermissive --std=c++0x
SFX=../../protobuf/build/lib/libprotobuf-c.a
SFX += ../../protobuf/build/lib/libprotobuf.a

bench_server: 
	-mkdir -p build
	javac -cp "../../../lib/BFT-SMaRt.jar:../../protobuf/build/protobuf-runtime.jar:../protobuf/build/vazio-proto.jar" -d build bftsmart/diversity/demos/zero/*.java
	cd build; jar cfe bench_server.jar bftsmart.diversity.demos.zero.ServerLaunch bftsmart

.PHONY: clean

clean:
	rm -fv *.o *~ *.out build



   


