INCL=-I$(PASTATCC)/wrapperC -I$(PASTAJAVA)/include/ -I$(PASTAJAVA)/include/linux
LIBS=-L $(PASTAJAVA)/jre/lib/amd64/server
INCL += `pkg-config --cflags --libs protobuf | sed 's/-I/-isystem /' | sed 's/-lprotobuf//'`
CCFLAGS=-g -Wextra -Wall --std=c++0x
CPFLAGS=-g -Wextra -Wall --std=c++0x
SFX=$(PASTATCC)/prefix/lib/libprotobuf.a

wrapper:
	g++ $(CPFLAGS) -o wrapper.o $(INCL) $(LIBS) -c ../bftsmart-wrapper.cpp -ljvm

test_client: wrapper
	g++ $(CPFLAGS) -o test_client.out $(INCL) $(LIBS) testcount.pb.cc test_client.cpp wrapper.o $(SFX) -ljvm

test_server: wrapper
	g++ $(CPFLAGS) -o test_server.out $(INCL) $(LIBS) testcount.pb.cc test_server.cpp wrapper.o $(SFX) -ljvm


bench_server: wrapper
	g++ $(CPFLAGS) -o bench_server.out $(INCL) $(LIBS) Vazio.pb.cc bench_server.cpp wrapper.o $(SFX) -ljvm


tests: test_server test_client

all: wrapper bench_server

.PHONY: clean

clean:
	rm -fv *.o *~ *.out



   


