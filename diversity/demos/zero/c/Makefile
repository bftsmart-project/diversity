INCL=-I$(PASTATCC)/wrapperC -I$(PASTAJAVA)/include/ -I$(PASTAJAVA)/include/linux
LIBS=-L $(PASTAJAVA)/jre/lib/amd64/server
INCL += `pkg-config --cflags --libs libprotobuf-c protobuf | sed 's/-lprotobuf-c//' | sed 's/-lprotobuf//'`
CCFLAGS=-g -Wextra -Wall -fpermissive --std=c++0x
CPFLAGS=-g -Wextra -Wall -fpermissive --std=c++0x
SFX=$(PASTATCC)/prefix/lib/libprotobuf-c.a
SFX += $(PASTATCC)/prefix/lib/libprotobuf.a

wrapper:
	g++ $(CPFLAGS) -o wrapper.o $(INCL) $(LIBS) -c ../bftsmart-wrapper.cpp -ljvm

#test_client: wrapper
#	gcc $(CCFLAGS) -o test_client.out $(INCL) $(LIBS) testcount.pb-c.c test_client.c wrapper.o $(SFX) -ljvm

#test_server: wrapper
#	gcc $(CCFLAGS) -o test_server.out $(INCL) $(LIBS) testcount.pb-c.c test_server.c wrapper.o $(SFX) -ljvm

bench_server: wrapper
	g++ $(CCFLAGS) -o bench_server.out $(INCL) $(LIBS) Vazio.pb-c.c bench_server.c wrapper.o $(SFX) -ljvm


#tests: test_server test_client

all: wrapper bench_server

.PHONY: clean

clean:
	rm -fv *.o *~ *.out



   


