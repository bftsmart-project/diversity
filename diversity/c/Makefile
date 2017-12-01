PASTAJAVA := $(shell ../../scripts/findjava.sh)
INCL=-I$(PASTAJAVA)/include/ -I$(PASTAJAVA)/include/linux -Ibuild
LIBS=-L $(PASTAJAVA)/jre/lib/amd64/server
CCFLAGS=-g -Wextra -Wall -Werror
CPFLAGS=-g -Wextra -Wall -Werror -std=c++0x

.PHONY: clean all headers

all : build/libbftsmr.so

../java/build/bftsmart-diversity.jar : FORCE
	$(MAKE) -C ../java 

build/bftsmart_diversity_ClientWrapper%h build/bftsmart_diversity_ServerWrapper%h : ../java/build/bftsmart-diversity.jar
	-mkdir build
	javah -force -d build -verbose -classpath ../../bftsmart/bin/BFT-SMaRt.jar:../java/build/bftsmart-diversity.jar bftsmart.diversity.ServerWrapper
	javah -force -d build -verbose -classpath ../../bftsmart/bin/BFT-SMaRt.jar:../java/build/bftsmart-diversity.jar bftsmart.diversity.ClientWrapper

build/bftsmart-c-client.o : build/wrapper.o build/bftsmart_diversity_ClientWrapper.h bftsmart-c-client.cpp
	g++ $(CPFLAGS) -fPIC -o build/bftsmart-c-client.o $(INCL) $(LIBS) -c bftsmart-c-client.cpp -ljvm 

build/bftsmart-c-server.o : build/wrapper.o build/bftsmart_diversity_ServerWrapper.h bftsmart-c-server.cpp
	g++ $(CPFLAGS) -fPIC -o build/bftsmart-c-server.o $(INCL) $(LIBS) -c bftsmart-c-server.cpp -ljvm 

build/wrapper.o : bftsmart.cpp
	-mkdir build
	g++ $(CPFLAGS) -fPIC -o build/wrapper.o $(INCL) $(LIBS) -c bftsmart.cpp -ljvm 

build/libbftsmr.so : build/bftsmart-c-client.o build/bftsmart-c-server.o build/wrapper.o
	g++ -shared -Wl,-soname,libbftsmr.so $(LIBS) -o build/libbftsmr.so build/wrapper.o build/bftsmart-c-client.o build/bftsmart-c-server.o -ljvm


clean:
	rm -rfv build

FORCE : ;	

   


