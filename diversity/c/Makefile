PASTAJAVA := $(shell ../../scripts/findjava.sh)
INCL=-I$(PASTAJAVA)/include/ -I$(PASTAJAVA)/include/linux -Ibuild
LIBS=-L $(PASTAJAVA)/jre/lib/amd64/server
CCFLAGS=-g -Wextra -Wall -Werror
CPFLAGS=-g -Wextra -Wall -Werror -std=c++0x


all : headers wrapper lshared

headers :
	-mkdir build
	javah -d build -verbose -classpath ../../lib/BFT-SMaRt.jar:../java/build/bftsmart-diversity.jar bftsmart.diversity.ServerWrapper
	javah -d build -verbose -classpath ../../lib/BFT-SMaRt.jar:../java/build/bftsmart-diversity.jar bftsmart.diversity.ClientWrapper

wrapper : headers
	-mkdir dist
	g++ $(CPFLAGS) -fPIC -o build/wrapper.o $(INCL) $(LIBS) -c bftsmart-wrapper.cpp -ljvm 

lshared: wrapper	
	g++ -shared -Wl,-soname,libbftsmr.so $(LIBS) -o build/libbftsmr.so build/wrapper.o -ljvm

all: wrapper lshared

.PHONY: clean

clean:
	rm -rfv build



   


