PASTAJAVA := $(shell ../../../scripts/findjava.sh)
INCL=-I../../../../diversity/c -I$(PASTAJAVA)/include/ -I$(PASTAJAVA)/include/linux
LIBS=-L $(PASTAJAVA)/jre/lib/amd64/server
GOPATH = $(shell pwd)/../protobuf/build/go:$(shell pwd)/../../../protobuf/protobuf-go:$(shell pwd)/../../../diversity/go:$(shell pwd)
CGO_CFLAGS=-g -Wextra -Wall $(INCL)
CGO_LDFLAGS=$(shell pwd)/build/wrapper.o $(shell pwd)/build/bftsmart-c-client.o $(shell pwd)/build/bftsmart-c-server.o $(LIBS) -lstdc++ -ljvm


export CGO_CFLAGS
export CGO_LDFLAGS
export GOPATH



.PHONY: all clean

all:	
	-mkdir build
	gcc -o build/wrapper.o -c dummy.c
	gcc -o build/bftsmart-c-client.o -c dummy.c
	gcc -o build/bftsmart-c-server.o -c dummy.c
	go build -x main
	go install main

clean:
	rm -rf pkg/* bin/* build;


