PASTAJAVA := $(shell ../../scripts/findjava.sh)
INCL=-I$(shell pwd)/../c -I$(PASTAJAVA)/include/ -I$(PASTAJAVA)/include/linux
LIBS=-L $(PASTAJAVA)/jre/lib/amd64/server
CGO_CFLAGS=-g -Wextra -Wall $(INCL)
CGO_LDFLAGS=build/wrapper.o build/bftsmart-c-client.o build/bftsmart-c-server.o $(LIBS) -lstdc++ -ljvm
GOPATH = $(shell pwd)

export CGO_CFLAGS
export CGO_LDFLAGS
export GOPATH

all: 
	-mkdir build
	-mkdir src/bftsmart/build
	-mkdir src/bftsmartserver/build
	-mkdir src/bftsmartclient/build
	cp ../c/build/wrapper.o src/bftsmart/build/
	cp ../c/build/bftsmart-c-client.o src/bftsmart/build/
	cp ../c/build/bftsmart-c-server.o src/bftsmart/build/
	cp ../c/build/wrapper.o src/bftsmartserver/build/
	cp ../c/build/bftsmart-c-client.o src/bftsmartserver/build/
	cp ../c/build/bftsmart-c-server.o src/bftsmartserver/build/
	cp ../c/build/wrapper.o src/bftsmartclient/build/
	cp ../c/build/bftsmart-c-client.o src/bftsmartclient/build/
	cp ../c/build/bftsmart-c-server.o src/bftsmartclient/build/
	go build bftsmart bftsmartclient bftsmartserver
	go install bftsmart bftsmartclient bftsmartserver

.PHONY: clean

clean:
	rm -rf pkg/* bin/* build;



   


