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
	javah -force -d build -verbose -classpath ../../lib/BFT-SMaRt.jar:../java/build/bftsmart-diversity.jar bftsmart.diversity.ServerWrapper
	javah -force -d build -verbose -classpath ../../lib/BFT-SMaRt.jar:../java/build/bftsmart-diversity.jar bftsmart.diversity.ClientWrapper




build/wrapper.o : bftsmart-wrapper.cpp build/bftsmart_diversity_ClientWrapper.h build/bftsmart_diversity_ServerWrapper.h
	g++ $(CPFLAGS) -fPIC -o build/wrapper.o $(INCL) $(LIBS) -c bftsmart-wrapper.cpp -ljvm 

build/libbftsmr.so : build/wrapper.o	
	g++ -shared -Wl,-soname,libbftsmr.so $(LIBS) -o build/libbftsmr.so build/wrapper.o -ljvm


clean:
	rm -rfv build

FORCE : ;	

   

