PASTAJAVA := $(shell ../../../scripts/findjava.sh)
INCL=-I../../../diversity/c -I$(PASTAJAVA)/include/ -I$(PASTAJAVA)/include/linux -I../protobuf/build
LIBS=-L $(PASTAJAVA)/jre/lib/amd64/server
INCL += `PKG_CONFIG_PATH=../../protobuf/build/lib/pkgconfig pkg-config --cflags --libs libprotobuf-c protobuf | sed 's/-lprotobuf-c//' | sed 's/-lprotobuf//'`

bench_server: 
	-mkdir -p build
	javac -cp "../../../lib/BFT-SMaRt.jar:../../../protobuf/build/protobuf-runtime.jar:../protobuf/build/lista-proto.jar" -d build *.java
	cd build; jar cfe bench_server.jar bftsmart.diversity.demos.list.ServerLaunch bftsmart

.PHONY: clean

clean:
	rm -rfv *.o *~ *.out build



   


